#include "draw_scramble.h"

/*
用于根据打乱公式生成打乱图案
*/

/*
魔方颜色值，这部分仅供参考，实际应该在显示端（如LVGL）定义
#define CUBE_COLOR_GREEN 0x07E0
#define CUBE_COLOR_RED 0xF800
#define CUBE_COLOR_BLUE 0x001F
#define CUBE_COLOR_ORANGE 0xFD20
#define CUBE_COLOR_WHITE 0xFFFF
#define CUBE_COLOR_YELLOW 0xFFE0
*/

/*
魔方颜色索引值
*/
#define CUBE_COLOR_GREEN_IDX 0
#define CUBE_COLOR_RED_IDX 1
#define CUBE_COLOR_BLUE_IDX 2
#define CUBE_COLOR_ORANGE_IDX 3
#define CUBE_COLOR_WHITE_IDX 4
#define CUBE_COLOR_YELLOW_IDX 5

/*
魔方面索引值，白顶绿前，正视面对应绿色面，以此类推
*/
#define CUBE_FACE_F_IDX CUBE_COLOR_GREEN_IDX
#define CUBE_FACE_R_IDX CUBE_COLOR_RED_IDX
#define CUBE_FACE_B_IDX CUBE_COLOR_BLUE_IDX
#define CUBE_FACE_L_IDX CUBE_COLOR_ORANGE_IDX
#define CUBE_FACE_U_IDX CUBE_COLOR_WHITE_IDX
#define CUBE_FACE_D_IDX CUBE_COLOR_YELLOW_IDX

/*
枚举魔方正视面
*/
typedef enum {
  CUBE_FACING_FRONT = CUBE_FACE_F_IDX,
  CUBE_FACING_RIGHT = CUBE_FACE_R_IDX,
  CUBE_FACING_BACK = CUBE_FACE_B_IDX,
  CUBE_FACING_LEFT = CUBE_FACE_L_IDX,
  CUBE_FACING_UP = CUBE_FACE_U_IDX,
  CUBE_FACING_DOWN = CUBE_FACE_D_IDX
}Cube_Facing;

/*
魔方正视面映射关系，以枚举魔方正视面顺序排列，通过当前正视面获取四个相邻面的索引及顺序
结构：CUBE_FACING_MAP[Cube_Facing idx] = {CUBE_FACING_LEFT, CUBE_FACING_UP, CUBE_FACING_RIGHT, CUBE_FACING_DOWN};
*/
static const uint8_t CUBE_FACING_MAP[CUBE_FACE_NUM][4] = {
  {CUBE_FACE_L_IDX, CUBE_FACE_U_IDX, CUBE_FACE_R_IDX, CUBE_FACE_D_IDX}, // CUBE_FACING_FRONT
  {CUBE_FACE_F_IDX, CUBE_FACE_U_IDX, CUBE_FACE_B_IDX, CUBE_FACE_D_IDX}, // CUBE_FACING_RIGHT
  {CUBE_FACE_R_IDX, CUBE_FACE_U_IDX, CUBE_FACE_L_IDX, CUBE_FACE_D_IDX}, // CUBE_FACING_BACK
  {CUBE_FACE_B_IDX, CUBE_FACE_U_IDX, CUBE_FACE_F_IDX, CUBE_FACE_D_IDX}, // CUBE_FACING_LEFT
  {CUBE_FACE_B_IDX, CUBE_FACE_R_IDX, CUBE_FACE_F_IDX, CUBE_FACE_L_IDX}, // CUBE_FACING_UP
  {CUBE_FACE_B_IDX, CUBE_FACE_L_IDX, CUBE_FACE_F_IDX, CUBE_FACE_R_IDX}  // CUBE_FACING_DOWN
};

/*************************************************************************************************************************
一维数组设计  ↓↓↓
*************************************************************************************************************************/

/*
定义魔方色块的索引值（数组下标），用于一维数组数据结构，中心块无需定义，以FRU为起始点，顺时针顺序排列
*/
#define CUBE_STICKER_FRU_IDX 0
#define CUBE_STICKER_FR_IDX 1
#define CUBE_STICKER_FRD_IDX 2
#define CUBE_STICKER_FD_IDX 3
#define CUBE_STICKER_FLD_IDX 4
#define CUBE_STICKER_FL_IDX 5
#define CUBE_STICKER_FLU_IDX 6
#define CUBE_STICKER_FU_IDX 7

/*
定义整体转动色块映射，以魔方色块的索引值顺序排列
弃用，只需偏移量即可，无需查表
*/
// static const uint8_t CUBE_STICKER_MAP[3][CUBE_FACE_STICKER_NUM] = {
//   {CUBE_STICKER_FLD_IDX, CUBE_STICKER_FL_IDX, CUBE_STICKER_FLU_IDX,
//    CUBE_STICKER_FD_IDX,                         CUBE_STICKER_FU_IDX,
//    CUBE_STICKER_FRD_IDX, CUBE_STICKER_FR_IDX, CUBE_STICKER_FRU_IDX}, // y

//   {CUBE_STICKER_FRD_IDX, CUBE_STICKER_FD_IDX, CUBE_STICKER_FLD_IDX,
//    CUBE_STICKER_FR_IDX,                         CUBE_STICKER_FL_IDX,
//    CUBE_STICKER_FRU_IDX, CUBE_STICKER_FU_IDX, CUBE_STICKER_FLU_IDX}, // y2

//   {CUBE_STICKER_FRU_IDX, CUBE_STICKER_FR_IDX, CUBE_STICKER_FRD_IDX,
//    CUBE_STICKER_FU_IDX,                         CUBE_STICKER_FD_IDX,
//    CUBE_STICKER_FLU_IDX, CUBE_STICKER_FL_IDX, CUBE_STICKER_FLD_IDX} // y'
// };

/*
说明：正视面————交换色块的下标索引
正视F R B L时，交换FRU FR FRD，需要做映射
正视U时，交换FLU FU FRU，不需要做映射
正视D时，交换FLD FD FRD，不需要做映射
@param Cube_Facing facing_idx 魔方当前正视面的下标
@param uint8_t *idxs 交换色块的下标索引
return 无
*/
void get_sticker_idxs(Cube_Facing facing_idx, uint8_t *idxs) {
  switch (facing_idx) {
    case CUBE_FACING_FRONT:
    case CUBE_FACING_RIGHT:
    case CUBE_FACING_BACK:
    case CUBE_FACING_LEFT:
      idxs[0] = CUBE_STICKER_FRU_IDX;
      idxs[1] = CUBE_STICKER_FR_IDX;
      idxs[2] = CUBE_STICKER_FRD_IDX;
      break;
    case CUBE_FACING_UP:
      idxs[0] = CUBE_STICKER_FLU_IDX;
      idxs[1] = CUBE_STICKER_FU_IDX;
      idxs[2] = CUBE_STICKER_FRU_IDX;
      break;
    case CUBE_FACING_DOWN:
      idxs[0] = CUBE_STICKER_FLD_IDX;
      idxs[1] = CUBE_STICKER_FD_IDX;
      idxs[2] = CUBE_STICKER_FRD_IDX;
      break;
    default:
      break;
  }
}

/*
说明：魔方颜色初始化，即还原魔方六面的颜色
@param cube_t *cube 魔方颜色结构体指针
@return 无
*/
void cube_color_init(cube_t *cube) {
  for(int i = 0; i < CUBE_FACE_NUM; ++i) {
    memset(cube->face[i], i, sizeof(cube_face_t));
  }
}

/*
说明：根据单步打乱，更新魔方颜色
Cube_Turn turn_degree; 
(CUBE_TURN_CYCLE - turn_degree) * 2即为需要交换的色块索引偏移，
转动90度，turn = 1，索引偏移量为6
转动180度，turn = 2，索引偏移量为4
转动270度，turn = 3，索引偏移量为2
CUBE_TURN_CYCLE - turn_degree即为需要交换的面索引偏移，
转动90度，turn = 1，索引偏移量为3
转动180度，turn = 2，索引偏移量为2
转动270度，turn = 3，索引偏移量为1
@param cube_t *cube 魔方颜色结构体指针
@param Cube_Facing facing_idx 魔方当前正视面的下标
@param Cube_Turn turn 魔方转动角度
@return 无
*/
void cube_color_update(cube_t *cube, Cube_Facing facing_idx, Cube_Turn turn_degree) {
  uint8_t tempColorIdx; // 临时颜色索引，用于处理循环交换
  uint8_t faceIdxOffset; // 魔方面索引偏移
  uint8_t stickerIdxOffset; // 魔方色块索引偏移
  uint8_t stickerIdxs[3]; // 需要交换的色块下标索引
  // 相邻面面颜色更新，从正视面的L面开始
  get_sticker_idxs(facing_idx, stickerIdxs);
  faceIdxOffset = CUBE_TURN_CYCLE - turn_degree;
  /*
  下标0对应当前正视面左邻面，无需映射
  下标1对应当前正视面上邻面，映射和当前正视面有关
  下标2对应当前正视面右邻面，固定映射180度旋转偏移
  下标3对应当前正视面下邻面，映射和当前正视面有关
  tempColorIdx = cube->face[CUBE_FACING_MAP[facing_idx][0]][stickerIdxs[i]];
  cube->face[CUBE_FACING_MAP[facing_idx][0]][stickerIdxs[i]] = cube->face[CUBE_FACING_MAP[facing_idx][0 + faceIdxOffset]][stickerIdxs[i]];
  cube->face[CUBE_FACING_MAP[facing_idx][1]][stickerIdxs[i] + turn_degree] = cube->face[CUBE_FACING_MAP[facing_idx][1 + faceIdxOffset]][stickerIdxs[i]];
  cube->face[CUBE_FACING_MAP[facing_idx][2]][stickerIdxs[i] + CUBE_TURN_180 * 2] = cube->face[CUBE_FACING_MAP[facing_idx][2 + faceIdxOffset]][stickerIdxs[i]];
  cube->face[CUBE_FACING_MAP[facing_idx][3]][stickerIdxs[i] - turn_degree] = tempColorIdx;

  勘误：面下标做偏移后，需要分情况讨论色块下标是否需要映射
  turn_degree = 90度时，
  0(无映射)----------------------------1(映射CUBE_TURN_CYCLE - facing_idx)
  1(映射CUBE_TURN_CYCLE - facing_idx)--2(映射y2)
  2(映射y2)----------------------------3(映射facing_idx)
  3(映射facing_idx)--------------------0(无映射，tempColorIdx)
  左侧下同
  turn_degree = 180度时，0--2互换，1--3互换，tempColorIdx需要特殊处理，不能只记录0下标对应颜色索引
  0------------------------------------2(映射y2)
  1------------------------------------3(映射facing_idx)
  2------------------------------------0(无映射)
  3------------------------------------1(映射CUBE_TURN_CYCLE - facing_idx)
  turn_degree = 270度时，
  0------------------------------------3(映射facing_idx)
  1------------------------------------0(无映射)
  2------------------------------------1(映射CUBE_TURN_CYCLE - facing_idx)
  3------------------------------------2(映射y2)
  */
  switch (turn_degree) {
    case CUBE_TURN_90: {
      // 相邻面颜色更新
      for(int i = 0; i < sizeof(stickerIdxs); ++i) {
        tempColorIdx = cube->face[CUBE_FACING_MAP[facing_idx][0]][stickerIdxs[i]];
        // 正视前四个面时需要映射
        if(facing_idx == CUBE_FACING_FRONT || facing_idx == CUBE_FACING_RIGHT || facing_idx == CUBE_FACING_BACK || facing_idx == CUBE_FACING_LEFT) {
          cube->face[CUBE_FACING_MAP[facing_idx][0]][stickerIdxs[i]] = cube->face[CUBE_FACING_MAP[facing_idx][3]][(stickerIdxs[i] + facing_idx * 2) % CUBE_FACE_STICKER_NUM];
          cube->face[CUBE_FACING_MAP[facing_idx][3]][(stickerIdxs[i] + facing_idx * 2) % CUBE_FACE_STICKER_NUM] = cube->face[CUBE_FACING_MAP[facing_idx][2]][(stickerIdxs[i] + CUBE_TURN_180 * 2) % CUBE_FACE_STICKER_NUM];
          cube->face[CUBE_FACING_MAP[facing_idx][2]][(stickerIdxs[i] + CUBE_TURN_180 * 2) % CUBE_FACE_STICKER_NUM] = cube->face[CUBE_FACING_MAP[facing_idx][1]][(stickerIdxs[i] + (CUBE_TURN_CYCLE - facing_idx) * 2) % CUBE_FACE_STICKER_NUM];
          cube->face[CUBE_FACING_MAP[facing_idx][1]][(stickerIdxs[i] + (CUBE_TURN_CYCLE - facing_idx) * 2) % CUBE_FACE_STICKER_NUM] = tempColorIdx;
        } else if(facing_idx == CUBE_FACING_UP || facing_idx == CUBE_FACING_DOWN) { // 正视顶面和底面时不需要映射
          cube->face[CUBE_FACING_MAP[facing_idx][0]][stickerIdxs[i]] = cube->face[CUBE_FACING_MAP[facing_idx][3]][stickerIdxs[i]];
          cube->face[CUBE_FACING_MAP[facing_idx][3]][stickerIdxs[i]] = cube->face[CUBE_FACING_MAP[facing_idx][2]][stickerIdxs[i]];
          cube->face[CUBE_FACING_MAP[facing_idx][2]][stickerIdxs[i]] = cube->face[CUBE_FACING_MAP[facing_idx][1]][stickerIdxs[i]];
          cube->face[CUBE_FACING_MAP[facing_idx][1]][stickerIdxs[i]] = tempColorIdx;
        }
      }
      // 正视面颜色更新
      // 角块颜色更新
      tempColorIdx = cube->face[facing_idx][CUBE_STICKER_FRU_IDX];
      cube->face[facing_idx][CUBE_STICKER_FRU_IDX] = cube->face[facing_idx][CUBE_STICKER_FLU_IDX];
      cube->face[facing_idx][CUBE_STICKER_FLU_IDX] = cube->face[facing_idx][CUBE_STICKER_FLD_IDX];
      cube->face[facing_idx][CUBE_STICKER_FLD_IDX] = cube->face[facing_idx][CUBE_STICKER_FRD_IDX];
      cube->face[facing_idx][CUBE_STICKER_FRD_IDX] = tempColorIdx;
      // 棱块颜色更新
      tempColorIdx = cube->face[facing_idx][CUBE_STICKER_FR_IDX];
      cube->face[facing_idx][CUBE_STICKER_FR_IDX] = cube->face[facing_idx][CUBE_STICKER_FU_IDX];
      cube->face[facing_idx][CUBE_STICKER_FU_IDX] = cube->face[facing_idx][CUBE_STICKER_FL_IDX];
      cube->face[facing_idx][CUBE_STICKER_FL_IDX] = cube->face[facing_idx][CUBE_STICKER_FD_IDX];
      cube->face[facing_idx][CUBE_STICKER_FD_IDX] = tempColorIdx;
      break;
    }
    case CUBE_TURN_180: { // 两两互换，tempColorIdx需要做两次交换中间值
      // 相邻面颜色更新
      for(int i = 0; i < sizeof(stickerIdxs); ++i) {
        tempColorIdx = cube->face[CUBE_FACING_MAP[facing_idx][0]][stickerIdxs[i]];
        // 正视前四个面时需要映射
        if(facing_idx == CUBE_FACING_FRONT || facing_idx == CUBE_FACING_RIGHT || facing_idx == CUBE_FACING_BACK || facing_idx == CUBE_FACING_LEFT) {
          cube->face[CUBE_FACING_MAP[facing_idx][0]][stickerIdxs[i]] = cube->face[CUBE_FACING_MAP[facing_idx][2]][(stickerIdxs[i] + CUBE_TURN_180 * 2) % CUBE_FACE_STICKER_NUM];
          cube->face[CUBE_FACING_MAP[facing_idx][2]][(stickerIdxs[i] + CUBE_TURN_180 * 2) % CUBE_FACE_STICKER_NUM] = tempColorIdx;

          tempColorIdx = cube->face[CUBE_FACING_MAP[facing_idx][1]][stickerIdxs[i]];
          cube->face[CUBE_FACING_MAP[facing_idx][1]][(stickerIdxs[i] + (CUBE_TURN_CYCLE - facing_idx) * 2) % CUBE_FACE_STICKER_NUM] = cube->face[CUBE_FACING_MAP[facing_idx][3]][(stickerIdxs[i] + facing_idx * 2) % CUBE_FACE_STICKER_NUM];
          cube->face[CUBE_FACING_MAP[facing_idx][3]][(stickerIdxs[i] + facing_idx * 2) % CUBE_FACE_STICKER_NUM] = tempColorIdx;
        } else if(facing_idx == CUBE_FACING_UP || facing_idx == CUBE_FACING_DOWN) { // 正视顶面和底面时不需要映射
          cube->face[CUBE_FACING_MAP[facing_idx][0]][stickerIdxs[i]] = cube->face[CUBE_FACING_MAP[facing_idx][2]][stickerIdxs[i]];
          cube->face[CUBE_FACING_MAP[facing_idx][2]][stickerIdxs[i]] = tempColorIdx;

          tempColorIdx = cube->face[CUBE_FACING_MAP[facing_idx][1]][stickerIdxs[i]];
          cube->face[CUBE_FACING_MAP[facing_idx][1]][stickerIdxs[i]] = cube->face[CUBE_FACING_MAP[facing_idx][3]][stickerIdxs[i]];
          cube->face[CUBE_FACING_MAP[facing_idx][3]][stickerIdxs[i]] = tempColorIdx;
        }
      }
      // 正视面颜色更新
      // 角块颜色更新
      tempColorIdx = cube->face[facing_idx][CUBE_STICKER_FRU_IDX];
      cube->face[facing_idx][CUBE_STICKER_FRU_IDX] = cube->face[facing_idx][CUBE_STICKER_FLD_IDX];
      cube->face[facing_idx][CUBE_STICKER_FLD_IDX] = tempColorIdx;
      tempColorIdx = cube->face[facing_idx][CUBE_STICKER_FRD_IDX];
      cube->face[facing_idx][CUBE_STICKER_FRD_IDX] = cube->face[facing_idx][CUBE_STICKER_FLU_IDX];
      cube->face[facing_idx][CUBE_STICKER_FLU_IDX] = tempColorIdx;
      // 棱块颜色更新
      tempColorIdx = cube->face[facing_idx][CUBE_STICKER_FR_IDX];
      cube->face[facing_idx][CUBE_STICKER_FR_IDX] = cube->face[facing_idx][CUBE_STICKER_FL_IDX];
      cube->face[facing_idx][CUBE_STICKER_FL_IDX] = tempColorIdx;
      tempColorIdx = cube->face[facing_idx][CUBE_STICKER_FD_IDX];
      cube->face[facing_idx][CUBE_STICKER_FD_IDX] = cube->face[facing_idx][CUBE_STICKER_FU_IDX];
      cube->face[facing_idx][CUBE_STICKER_FU_IDX] = tempColorIdx;
      break;
    }
    case CUBE_TURN_270: {
      // 相邻面颜色更新
      for(int i = 0; i < sizeof(stickerIdxs); ++i) {
        tempColorIdx = cube->face[CUBE_FACING_MAP[facing_idx][0]][stickerIdxs[i]];
        // 正视前四个面时需要映射
        if(facing_idx == CUBE_FACING_FRONT || facing_idx == CUBE_FACING_RIGHT || facing_idx == CUBE_FACING_BACK || facing_idx == CUBE_FACING_LEFT) {
          cube->face[CUBE_FACING_MAP[facing_idx][0]][stickerIdxs[i]] = cube->face[CUBE_FACING_MAP[facing_idx][1]][(stickerIdxs[i] + (CUBE_TURN_CYCLE - facing_idx) * 2) % CUBE_FACE_STICKER_NUM];
          cube->face[CUBE_FACING_MAP[facing_idx][1]][(stickerIdxs[i] + (CUBE_TURN_CYCLE - facing_idx) * 2) % CUBE_FACE_STICKER_NUM] = cube->face[CUBE_FACING_MAP[facing_idx][2]][(stickerIdxs[i] + CUBE_TURN_180 * 2) % CUBE_FACE_STICKER_NUM];
          cube->face[CUBE_FACING_MAP[facing_idx][2]][(stickerIdxs[i] + CUBE_TURN_180 * 2) % CUBE_FACE_STICKER_NUM] = cube->face[CUBE_FACING_MAP[facing_idx][3]][(stickerIdxs[i] + facing_idx * 2) % CUBE_FACE_STICKER_NUM];
          cube->face[CUBE_FACING_MAP[facing_idx][3]][(stickerIdxs[i] + facing_idx * 2) % CUBE_FACE_STICKER_NUM] = tempColorIdx;
        } else if(facing_idx == CUBE_FACING_UP || facing_idx == CUBE_FACING_DOWN) { // 正视顶面和底面时不需要映射
          cube->face[CUBE_FACING_MAP[facing_idx][0]][stickerIdxs[i]] = cube->face[CUBE_FACING_MAP[facing_idx][1]][stickerIdxs[i]];
          cube->face[CUBE_FACING_MAP[facing_idx][1]][stickerIdxs[i]] = cube->face[CUBE_FACING_MAP[facing_idx][2]][stickerIdxs[i]];
          cube->face[CUBE_FACING_MAP[facing_idx][2]][stickerIdxs[i]] = cube->face[CUBE_FACING_MAP[facing_idx][3]][stickerIdxs[i]];
          cube->face[CUBE_FACING_MAP[facing_idx][3]][stickerIdxs[i]] = tempColorIdx;
        }
      }
      // 正视面颜色更新
      // 角块颜色更新
      tempColorIdx = cube->face[facing_idx][CUBE_STICKER_FRU_IDX];
      cube->face[facing_idx][CUBE_STICKER_FRU_IDX] = cube->face[facing_idx][CUBE_STICKER_FRD_IDX];
      cube->face[facing_idx][CUBE_STICKER_FRD_IDX] = cube->face[facing_idx][CUBE_STICKER_FLD_IDX];
      cube->face[facing_idx][CUBE_STICKER_FLD_IDX] = cube->face[facing_idx][CUBE_STICKER_FLU_IDX];
      cube->face[facing_idx][CUBE_STICKER_FLU_IDX] = tempColorIdx;
      // 棱块颜色更新
      tempColorIdx = cube->face[facing_idx][CUBE_STICKER_FR_IDX];
      cube->face[facing_idx][CUBE_STICKER_FR_IDX] = cube->face[facing_idx][CUBE_STICKER_FD_IDX];
      cube->face[facing_idx][CUBE_STICKER_FD_IDX] = cube->face[facing_idx][CUBE_STICKER_FL_IDX];
      cube->face[facing_idx][CUBE_STICKER_FL_IDX] = cube->face[facing_idx][CUBE_STICKER_FU_IDX];
      cube->face[facing_idx][CUBE_STICKER_FU_IDX] = tempColorIdx;
      break;
    }
    default:
      break;
  }
}

/*
说明：魔方执行单步转动，根据打乱，翻译为facing和turn_degree
@param cube_t *cube 魔方颜色结构体指针
@param char *scarmble_step 单步打乱字符串
@return 无
*/
void cube_turn_one_step(cube_t *cube, char *scarmble_step) {
  // 判断正视面
  uint8_t facing_idx;
  switch (scarmble_step[0]) {
    case 'F':
      facing_idx = CUBE_FACING_FRONT;
      break;
    case 'R':
      facing_idx = CUBE_FACING_RIGHT;
      break;
    case 'B':
      facing_idx = CUBE_FACING_BACK;
      break;
    case 'L':
      facing_idx = CUBE_FACING_LEFT;
      break;
    case 'U':
      facing_idx = CUBE_FACING_UP;
      break;
    case 'D':
      facing_idx = CUBE_FACING_DOWN;
      break;
    default:
      break;
  }

  // 判断转动角度
  Cube_Turn turn_degree;
  switch (scarmble_step[1]) {
    case '\0':
      turn_degree = CUBE_TURN_90;
      break;
    case '2':
      turn_degree = CUBE_TURN_180;
      break;
    case '\'':
      turn_degree = CUBE_TURN_270;
      break;
    default:
      break;
  }

  // 执行颜色更新
  cube_color_update(cube, facing_idx, turn_degree);
}

/*
说明：魔方解析打乱字符串，分步调用单步打乱方法
需要注意的是，如果scramble_alg在定义时已经指定了大小，末尾不会有\0
可以用char *scarmble_alg;或char scramble_alg[];（不指定大小）
@param cube_t *cube 魔方颜色结构体指针
@param char *scarmble_alg 打乱公式字符串
@return 无
*/
void cube_scramble_alg_parse(cube_t *cube, char *scarmble_alg) {
  char *ptr = scarmble_alg;
  char step[2];
  Cube_Turn turn_degree;
  while(*ptr != '\0') {
    //while(*ptr == ' ') ptr++; // 跳过空格，这一步在内部调用时可以省略，因为存储结构不包含空格
    if(*ptr == 'F' || *ptr == 'R' || *ptr == 'B' || *ptr == 'L' || *ptr == 'U' || *ptr == 'D') {
      step[0] = *ptr;
      switch(*(ptr + 1)){       
        case '2':{
          ptr++; // 这个单步包含两个字符，ptr额外移动一位
          step[1] = '2';
          cube_turn_one_step(cube, step);
          break;
        }
        case '\'':{
          ptr++; // 这个单步包含两个字符，ptr额外移动一位
          step[1] = '\'';
          cube_turn_one_step(cube, step);
          break;
        }
        case '\0':
        case ' ':
        default: // 包含 F R B L U D
          step[1] = '\0';
          cube_turn_one_step(cube, step);
          break;
      }
    }
    ptr++;
  }
}

/*************************************************************************************************************************
一维数组设计  ↑↑↑
*************************************************************************************************************************/