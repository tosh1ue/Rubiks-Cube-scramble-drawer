/**
 * @file draw_scramble.c
 * @brief 用于根据打乱公式计算打乱后的颜色信息
 * @author tosh1ue
 * @date 2026-02-03
 */

#include "draw_scramble.h"
#include <string.h>

/**
 * @brief 魔方正视面映射关系，以魔方正视面顺序排列，通过当前正视面获取四个相邻面的索引及顺序
 * 结构：CUBE_FACING_MAP[cube_face_t idx] = {CUBE_FACING_UP, CUBE_FACING_RIGHT, CUBE_FACING_DOWN, CUBE_FACING_LEFT};
 */
static const cube_face_t CUBE_FACING_MAP[CUBE_FACE_NUM][CUBE_TURN_CYCLE] = {
  {CUBE_FACE_U_IDX, CUBE_FACE_R_IDX, CUBE_FACE_D_IDX, CUBE_FACE_L_IDX}, // CUBE_FACING_FRONT
  {CUBE_FACE_R_IDX, CUBE_FACE_F_IDX, CUBE_FACE_L_IDX, CUBE_FACE_B_IDX}, // CUBE_FACING_UP
  {CUBE_FACE_B_IDX, CUBE_FACE_D_IDX, CUBE_FACE_F_IDX, CUBE_FACE_U_IDX}, // CUBE_FACING_RIGHT
  {CUBE_FACE_D_IDX, CUBE_FACE_R_IDX, CUBE_FACE_U_IDX, CUBE_FACE_L_IDX}, // CUBE_FACING_BACK
  {CUBE_FACE_L_IDX, CUBE_FACE_F_IDX, CUBE_FACE_R_IDX, CUBE_FACE_B_IDX}, // CUBE_FACING_DOWN
  {CUBE_FACE_F_IDX, CUBE_FACE_D_IDX, CUBE_FACE_B_IDX, CUBE_FACE_U_IDX}  // CUBE_FACING_LEFT


};

/**
 * @brief 魔方颜色信息数组
 */
static cube_t cube[CUBE_STICKER_NUM];

/**
 * @brief 以当前正视面获取交换色块的下标索引
 * @param facing_idx 魔方当前正视面的下标
 * @param idx 交换色块的下标索引指针
 */
void get_sticker_idx(const cube_face_t facing_idx, cube_color_t* idx) {
  const uint8_t is_even = facing_idx % 2 == 0;
  if (is_even) {
    idx[0] = CUBE_STICKER_FRU_IDX;
    idx[1] = CUBE_STICKER_FR_IDX;
    idx[2] = CUBE_STICKER_FRD_IDX;
  } else {
    idx[0] = CUBE_STICKER_FLD_IDX;
    idx[1] = CUBE_STICKER_FL_IDX;
    idx[2] = CUBE_STICKER_FLU_IDX;
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

/**
 * @brief 根据正视面和旋转角度，执行单步打乱颜色更新
 * @details 以U为基准，4个相邻面的偏移分别是0 y' 0 y
 * @param facing_idx 魔方正视面索引
 * @param turn_degree 旋转角度
 * @return 无
 */
void cube_turn(const cube_face_t facing_idx, const cube_turn_t turn_degree) {
  cube_color_t tempColorIdx; // 临时颜色索引，用于处理循环交换
  const uint8_t stickerIdxOffset = (CUBE_TURN_CYCLE - turn_degree) * 2; // 魔方面内色块索引偏移，用于处理正视面颜色交换
  const uint8_t stickerIdxPrefix = facing_idx * CUBE_FACE_STICKER_NUM; // 色块索引前缀，用于处理正视面颜色交换
  const cube_face_t *faceIdx = CUBE_FACING_MAP[facing_idx];; // 需要交换的面下标索引
  cube_color_t stickerIdx[3]; // 需要交换的色块下标索引
  get_sticker_idx(facing_idx, stickerIdx);
  switch (turn_degree) {
    case CUBE_TURN_90: {
      // 相邻面颜色更新
      for (cube_color_t i = 0; i < sizeof(stickerIdx); ++i) {
        tempColorIdx = cube[faceIdx[CUBE_SIDE_U] * CUBE_FACE_STICKER_NUM + stickerIdx[i]];
        cube[faceIdx[CUBE_SIDE_U] * CUBE_FACE_STICKER_NUM + stickerIdx[i]] = cube[faceIdx[CUBE_SIDE_L] * CUBE_FACE_STICKER_NUM + OFFSET_L(stickerIdx[i])];
        cube[faceIdx[CUBE_SIDE_L] * CUBE_FACE_STICKER_NUM + OFFSET_L(stickerIdx[i])] = cube[faceIdx[CUBE_SIDE_D] * CUBE_FACE_STICKER_NUM + stickerIdx[i]];
        cube[faceIdx[CUBE_SIDE_D] * CUBE_FACE_STICKER_NUM + stickerIdx[i]] = cube[faceIdx[CUBE_SIDE_R] * CUBE_FACE_STICKER_NUM + OFFSET_R(stickerIdx[i])];
        cube[faceIdx[CUBE_SIDE_R] * CUBE_FACE_STICKER_NUM + OFFSET_R(stickerIdx[i])] = tempColorIdx;
      }
      break;
    }
    case CUBE_TURN_180: { // 180度是两两互换，需要特殊处理
      // 相邻面颜色更新
      for (cube_color_t i = 0; i < sizeof(stickerIdx); ++i) {
        tempColorIdx = cube[faceIdx[CUBE_SIDE_U] * CUBE_FACE_STICKER_NUM + stickerIdx[i]];
        cube[faceIdx[CUBE_SIDE_U] * CUBE_FACE_STICKER_NUM + stickerIdx[i]] = cube[faceIdx[CUBE_SIDE_D] * CUBE_FACE_STICKER_NUM + stickerIdx[i]];
        cube[faceIdx[CUBE_SIDE_D] * CUBE_FACE_STICKER_NUM + stickerIdx[i]] = tempColorIdx;
        tempColorIdx = cube[faceIdx[CUBE_SIDE_L] * CUBE_FACE_STICKER_NUM + OFFSET_L(stickerIdx[i])];
        cube[faceIdx[CUBE_SIDE_L] * CUBE_FACE_STICKER_NUM + OFFSET_L(stickerIdx[i])] = cube[faceIdx[CUBE_SIDE_R] * CUBE_FACE_STICKER_NUM + OFFSET_R(stickerIdx[i])];
        cube[faceIdx[CUBE_SIDE_R] * CUBE_FACE_STICKER_NUM + OFFSET_R(stickerIdx[i])] = tempColorIdx;
      }
      // 正视面颜色更新（i: 0~角块  1~棱块）
      for (uint8_t i = STICKER_CORNER; i <= STICKER_EDGE; ++i) {
        cube_color_t sticker_src_idx = i;
        cube_color_t sticker_dest_idx = (sticker_src_idx + stickerIdxOffset) % CUBE_FACE_STICKER_NUM;
        tempColorIdx = cube[stickerIdxPrefix + sticker_src_idx];
        cube[stickerIdxPrefix + sticker_src_idx] = cube[stickerIdxPrefix + sticker_dest_idx];
        cube[stickerIdxPrefix + sticker_dest_idx] = tempColorIdx;

        sticker_src_idx += STICKER_GAP;
        sticker_dest_idx += STICKER_GAP;
        tempColorIdx = cube[stickerIdxPrefix + sticker_src_idx];
        cube[stickerIdxPrefix + sticker_src_idx] = cube[stickerIdxPrefix + sticker_dest_idx];
        cube[stickerIdxPrefix + sticker_dest_idx] = tempColorIdx;
      }
      return; // 分支已经处理正视面颜色更新，直接返回
    }
    case CUBE_TURN_270: {
      // 相邻面颜色更新
      for (cube_color_t i = 0; i < sizeof(stickerIdx); ++i) {
        tempColorIdx = cube[faceIdx[CUBE_SIDE_U] * CUBE_FACE_STICKER_NUM + stickerIdx[i]];
        cube[faceIdx[CUBE_SIDE_U] * CUBE_FACE_STICKER_NUM + stickerIdx[i]] = cube[faceIdx[CUBE_SIDE_R] * CUBE_FACE_STICKER_NUM + OFFSET_R(stickerIdx[i])];
        cube[faceIdx[CUBE_SIDE_R] * CUBE_FACE_STICKER_NUM + OFFSET_R(stickerIdx[i])] = cube[faceIdx[CUBE_SIDE_D] * CUBE_FACE_STICKER_NUM + stickerIdx[i]];
        cube[faceIdx[CUBE_SIDE_D] * CUBE_FACE_STICKER_NUM + stickerIdx[i]] = cube[faceIdx[CUBE_SIDE_L] * CUBE_FACE_STICKER_NUM + OFFSET_L(stickerIdx[i])];
        cube[faceIdx[CUBE_SIDE_L] * CUBE_FACE_STICKER_NUM + OFFSET_L(stickerIdx[i])] = tempColorIdx;
      }
      break;
    }
    default:
      break;
  }
  // 正视面颜色更新（i: 0~角块  1~棱块）
  for (uint8_t i = STICKER_CORNER; i <= STICKER_EDGE; ++i) {
    cube_color_t sticker_src_idx = i;
    cube_color_t sticker_dest_idx = (sticker_src_idx + stickerIdxOffset) % CUBE_FACE_STICKER_NUM;
    tempColorIdx = cube[stickerIdxPrefix + sticker_src_idx];
    cube[stickerIdxPrefix + sticker_src_idx] = cube[stickerIdxPrefix + sticker_dest_idx];
    uint8_t update_times = CUBE_TURN_CYCLE - 2;
    while (update_times--) {
      sticker_src_idx = sticker_dest_idx;
      sticker_dest_idx = (sticker_src_idx + stickerIdxOffset) % CUBE_FACE_STICKER_NUM;
      cube[stickerIdxPrefix + sticker_src_idx] = cube[stickerIdxPrefix + sticker_dest_idx];
    }
    cube[stickerIdxPrefix + sticker_dest_idx] = tempColorIdx;
  }
}

/**
 * @brief 解析单步打乱字符串，翻译为facing和turn_degree
 * @param scramble_step 单步打乱字符串
 * @return 无
 */
void cube_parse_step(const char* scramble_step) {
  // 判断正视面
  uint8_t facing_idx;
  switch (scramble_step[0]) {
    case 'F':
      facing_idx = CUBE_FACE_F_IDX;
      break;
    case 'R':
      facing_idx = CUBE_FACE_R_IDX;
      break;
    case 'B':
      facing_idx = CUBE_FACE_B_IDX;
      break;
    case 'L':
      facing_idx = CUBE_FACE_L_IDX;
      break;
    case 'U':
      facing_idx = CUBE_FACE_U_IDX;
      break;
    case 'D':
      facing_idx = CUBE_FACE_D_IDX;
      break;
    default:
      return;
  }

  // 判断转动角度
  cube_turn_t turn_degree;
  switch (scramble_step[1]) {
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
      return;
  }

  // 执行颜色更新
  cube_turn(facing_idx, turn_degree);
}

/**
 * @brief 说明：魔方解析打乱字符串，分步调用单步打乱方法
 * 需要注意的是，如果scramble_alg在定义时已经指定了大小，末尾不会有\0
 * 可以用char *scramble_alg;或char scramble_alg[];（不指定大小）
 * @param scramble_alg 打乱公式字符串
 * @return 无
 */
void cube_update_color(char* scramble_alg) {
  char* ptr = scramble_alg;
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
          cube_parse_step(step);
          break;
        }
        case '\'':{
          ptr++; // 这个单步包含两个字符，ptr额外移动一位
          step[1] = '\'';
          cube_parse_step(step);
          break;
        }
        case '\0':
        case ' ':
        default: // 包含 F R B L U D
          step[1] = '\0';
          cube_parse_step(step);
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
  return cube;
}