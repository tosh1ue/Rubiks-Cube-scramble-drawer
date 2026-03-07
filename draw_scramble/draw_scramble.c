/**
 * @file draw_scramble.c
 * @brief 用于根据打乱公式计算打乱后的颜色信息
 * @author tosh1ue
 * @date 2026-02-03
 */

#include "draw_scramble.h"
#include <string.h>

#define CUBE_ORDER 3 // 魔方阶数
#define CUBE_FACE_NUM 6 // 魔方面数
#define CUBE_FACE_STICKER_NUM (CUBE_ORDER * CUBE_ORDER - 1) // 魔方每面色块数，省略中心块
#define CUBE_TURN_CYCLE 4 // 魔方转动周期
#define CUBE_STICKER_NUM (CUBE_FACE_NUM * CUBE_STICKER_NUM) // 魔方总色块数

/**
 * @brief 魔方面索引值，白顶绿前，正视面对应绿色面，以此类推
 */
#define CUBE_FACE_F_IDX CUBE_COLOR_GREEN
#define CUBE_FACE_R_IDX CUBE_COLOR_RED
#define CUBE_FACE_B_IDX CUBE_COLOR_BLUE
#define CUBE_FACE_L_IDX CUBE_COLOR_ORANGE
#define CUBE_FACE_U_IDX CUBE_COLOR_WHITE
#define CUBE_FACE_D_IDX CUBE_COLOR_YELLOW

/**
 * @brief 魔方正视面
 */
#define CUBE_FACING_FRONT CUBE_FACE_F_IDX
#define CUBE_FACING_RIGHT CUBE_FACE_R_IDX
#define CUBE_FACING_BACK CUBE_FACE_B_IDX
#define CUBE_FACING_LEFT CUBE_FACE_L_IDX
#define CUBE_FACING_UP CUBE_FACE_U_IDX
#define CUBE_FACING_DOWN CUBE_FACE_D_IDX

/**
 * @brief 魔方正视面映射关系，以枚举魔方正视面顺序排列，通过当前正视面获取四个相邻面的索引及顺序
 * 结构：CUBE_FACING_MAP[Cube_Facing idx] = {CUBE_FACING_LEFT, CUBE_FACING_UP, CUBE_FACING_RIGHT, CUBE_FACING_DOWN};
 */
static const uint8_t CUBE_FACING_MAP[CUBE_FACE_NUM][CUBE_TURN_CYCLE] = {
  {CUBE_FACE_L_IDX, CUBE_FACE_U_IDX, CUBE_FACE_R_IDX, CUBE_FACE_D_IDX}, // CUBE_FACING_FRONT
  {CUBE_FACE_F_IDX, CUBE_FACE_U_IDX, CUBE_FACE_B_IDX, CUBE_FACE_D_IDX}, // CUBE_FACING_RIGHT
  {CUBE_FACE_R_IDX, CUBE_FACE_U_IDX, CUBE_FACE_L_IDX, CUBE_FACE_D_IDX}, // CUBE_FACING_BACK
  {CUBE_FACE_B_IDX, CUBE_FACE_U_IDX, CUBE_FACE_F_IDX, CUBE_FACE_D_IDX}, // CUBE_FACING_LEFT
  {CUBE_FACE_B_IDX, CUBE_FACE_R_IDX, CUBE_FACE_F_IDX, CUBE_FACE_L_IDX}, // CUBE_FACING_UP
  {CUBE_FACE_B_IDX, CUBE_FACE_L_IDX, CUBE_FACE_F_IDX, CUBE_FACE_R_IDX}  // CUBE_FACING_DOWN
};



/**
 * @brief 魔方颜色结构，由于中心块不变，无需存储
 * @details 用face_idx * gap + sticker_idx 取值
 *          +-----------+
 *          | 0 | 1 | 2 |
 *          +-----------+
 *          | 7 |   | 3 |
 *          +-----------+
 *          | 6 | 5 | 4 |
 *          +-----------+
 */
#define CUBE_STICKER_FLU_IDX 0
#define CUBE_STICKER_FU_IDX 1
#define CUBE_STICKER_FRU_IDX 2
#define CUBE_STICKER_FR_IDX 3
#define CUBE_STICKER_FRD_IDX 4
#define CUBE_STICKER_FD_IDX 5
#define CUBE_STICKER_FLD_IDX 6
#define CUBE_STICKER_FL_IDX 7

static uint8_t cube[CUBE_STICKER_NUM];

/**
 * @brief 以当前正视面获取交换色块的下标索引
 * @param facing_idx 魔方当前正视面的下标
 * @param *idxs 交换色块的下标索引指针
 */
void get_sticker_idxs(cube_facing_t facing_idx, uint8_t *idxs) {
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

/**
 * @brief 魔方颜色初始化，即还原魔方六面的颜色
 * @param 无
 * @return 无
 */
void cube_reset_color(void) {
  for(int face_idx = 0; face_idx < CUBE_FACE_NUM; ++face_idx) {
    memset(&cube[face_idx * CUBE_FACE_STICKER_NUM], face_idx, CUBE_FACE_STICKER_NUM * sizeof(uint8_t));
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
void cube_color_update(cube_facing_t facing_idx, cube_turn_t turn_degree) {
  uint8_t tempColorIdx; // 临时颜色索引，用于处理循环交换
  uint8_t faceIdxOffset; // 魔方面索引偏移
  uint8_t stickerIdxOffset; // 魔方色块索引偏移
  uint8_t stickerIdxs[3]; // 需要交换的色块下标索引
  // 相邻面面颜色更新，从正视面的L面开始
  get_sticker_idxs(facing_idx, stickerIdxs);
  faceIdxOffset = CUBE_TURN_CYCLE - turn_degree;
  switch (turn_degree) {
    
  }
}

/*
说明：魔方执行单步转动，根据打乱，翻译为facing和turn_degree
@param cube_t *cube 魔方颜色结构体指针
@param char *scarmble_step 单步打乱字符串
@return 无
*/
void cube_turn_one_step(char *scarmble_step) {
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
  cube_turn_t turn_degree;
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
      turn_degree = CUBE_TURN_90;
      break;
  }

  // 执行颜色更新
  cube_color_update(cube, facing_idx, turn_degree);
}

/**
 * @brief 说明：魔方解析打乱字符串，分步调用单步打乱方法
 * 需要注意的是，如果scramble_alg在定义时已经指定了大小，末尾不会有\0
 * 可以用char *scarmble_alg;或char scramble_alg[];（不指定大小）
 * @param scarmble_alg 打乱公式字符串
 * @return 无
 */
void cube_scramble_alg_parse(char* scarmble_alg) {
  char *ptr = scarmble_alg;
  char step[2];
  cube_turn_t turn_degree;
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

/**
 * @brief 获取魔方颜色信息的接口
 * @param 无
 * @return 魔方颜色结构体指针，即指向存储颜色信息的数组的指针
 */
const cube_t* cube_get_color(void) {
  return *cube;
}