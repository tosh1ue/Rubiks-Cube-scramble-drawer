/**
 * @file draw_scramble.c
 * @brief 用于根据打乱公式计算打乱后的颜色信息
 * @author tosh1ue
 * @date 2026-02-03
 */

#include "draw_scramble.h"
#include <string.h>

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

/**
 * @brief 魔方旋转面与相邻面的映射关系，以魔方面顺序排列，通过当前旋转面获取四个相邻面的索引及顺序
 * @details CUBE_NEIGHBOR_FACE_MAP[turn_face][CUBE_SIDE_U/R/D/L] 分别表示当前旋转面的上/右/下/左相邻面索引
 */
static const cube_face_t CUBE_NEIGHBOR_FACE_MAP[CUBE_FACE_NUM][CUBE_TURN_CYCLE] = {
  {CUBE_FACE_U_IDX, CUBE_FACE_R_IDX, CUBE_FACE_D_IDX, CUBE_FACE_L_IDX}, // CUBE_TURN_FRONT
  {CUBE_FACE_R_IDX, CUBE_FACE_F_IDX, CUBE_FACE_L_IDX, CUBE_FACE_B_IDX}, // CUBE_TURN_UP
  {CUBE_FACE_B_IDX, CUBE_FACE_D_IDX, CUBE_FACE_F_IDX, CUBE_FACE_U_IDX}, // CUBE_TURN_RIGHT
  {CUBE_FACE_D_IDX, CUBE_FACE_R_IDX, CUBE_FACE_U_IDX, CUBE_FACE_L_IDX}, // CUBE_TURN_BACK
  {CUBE_FACE_L_IDX, CUBE_FACE_F_IDX, CUBE_FACE_R_IDX, CUBE_FACE_B_IDX}, // CUBE_TURN_DOWN
  {CUBE_FACE_F_IDX, CUBE_FACE_D_IDX, CUBE_FACE_B_IDX, CUBE_FACE_U_IDX}  // CUBE_TURN_LEFT
};

/**
 * @brief 魔方颜色信息数组
 */
static cube_color_t cube[CUBE_STICKER_NUM];

/**
 * @brief 以当前旋转面获取交换色块的下标索引
 * @param turn_face 魔方当前旋转面的下标
 * @param idx 交换色块的下标索引指针
 */
static void cube_get_sticker_idx(const cube_face_t turn_face, cube_sticker_t *idx) {
  const uint8_t is_even = turn_face % 2 == 0;
  if (is_even) {
    idx[0] = CUBE_STICKER_RU_IDX;
    idx[1] = CUBE_STICKER_R_IDX;
    idx[2] = CUBE_STICKER_RD_IDX;
  } else {
    idx[0] = CUBE_STICKER_LD_IDX;
    idx[1] = CUBE_STICKER_L_IDX;
    idx[2] = CUBE_STICKER_LU_IDX;
  }
}

/**
 * @brief 魔方颜色初始化，即还原魔方六面的颜色
 */
void cube_reset_color(void) {
  for (cube_face_t face_idx = 0; face_idx < CUBE_FACE_NUM; ++face_idx) {
    memset(&cube[face_idx * STICKER_PER_FACE], (cube_color_t)face_idx, STICKER_PER_FACE * sizeof(cube_color_t));
  }
}

/**
 * @brief 根据旋转面和旋转角度，执行单步打乱颜色更新
 * @details 以U为基准，4个相邻面的偏移分别是0 y' 0 y
 * @param turn_face 魔方旋转面索引
 * @param turn_degree 旋转角度
 */
static void cube_turn(const cube_face_t turn_face, const cube_turn_t turn_degree) {
  cube_color_t temp_sticker_color; // 临时颜色索引，用于处理循环交换
  const uint8_t sticker_idx_offset = (CUBE_TURN_CYCLE - turn_degree) * 2; // 魔方面内色块索引偏移，用于处理旋转面颜色交换
  const uint8_t sticker_idx_prefix = turn_face * STICKER_PER_FACE; // 色块索引前缀，用于处理旋转面颜色交换
  const cube_face_t *adj_faces = CUBE_NEIGHBOR_FACE_MAP[turn_face]; // 需要交换的面下标索引
  cube_sticker_t adj_stickers[3]; // 需要交换的色块下标索引
  cube_get_sticker_idx(turn_face, adj_stickers);
  switch (turn_degree) {
    case CUBE_TURN_90: {
      // 相邻面颜色更新
      for (uint8_t i = 0; i < ARRAY_SIZE(adj_stickers); ++i) {
        temp_sticker_color = cube[adj_faces[CUBE_SIDE_U] * STICKER_PER_FACE + adj_stickers[i]];
        cube[adj_faces[CUBE_SIDE_U] * STICKER_PER_FACE + adj_stickers[i]] = cube[adj_faces[CUBE_SIDE_L] * STICKER_PER_FACE + OFFSET_L(adj_stickers[i])];
        cube[adj_faces[CUBE_SIDE_L] * STICKER_PER_FACE + OFFSET_L(adj_stickers[i])] = cube[adj_faces[CUBE_SIDE_D] * STICKER_PER_FACE + adj_stickers[i]];
        cube[adj_faces[CUBE_SIDE_D] * STICKER_PER_FACE + adj_stickers[i]] = cube[adj_faces[CUBE_SIDE_R] * STICKER_PER_FACE + OFFSET_R(adj_stickers[i])];
        cube[adj_faces[CUBE_SIDE_R] * STICKER_PER_FACE + OFFSET_R(adj_stickers[i])] = temp_sticker_color;
      }
      break;
    }
    case CUBE_TURN_180: { // 180度是两两互换，需要特殊处理
      // 相邻面颜色更新
      for (uint8_t i = 0; i < ARRAY_SIZE(adj_stickers); ++i) {
        temp_sticker_color = cube[adj_faces[CUBE_SIDE_U] * STICKER_PER_FACE + adj_stickers[i]];
        cube[adj_faces[CUBE_SIDE_U] * STICKER_PER_FACE + adj_stickers[i]] = cube[adj_faces[CUBE_SIDE_D] * STICKER_PER_FACE + adj_stickers[i]];
        cube[adj_faces[CUBE_SIDE_D] * STICKER_PER_FACE + adj_stickers[i]] = temp_sticker_color;
        temp_sticker_color = cube[adj_faces[CUBE_SIDE_L] * STICKER_PER_FACE + OFFSET_L(adj_stickers[i])];
        cube[adj_faces[CUBE_SIDE_L] * STICKER_PER_FACE + OFFSET_L(adj_stickers[i])] = cube[adj_faces[CUBE_SIDE_R] * STICKER_PER_FACE + OFFSET_R(adj_stickers[i])];
        cube[adj_faces[CUBE_SIDE_R] * STICKER_PER_FACE + OFFSET_R(adj_stickers[i])] = temp_sticker_color;
      }
      // 旋转面颜色更新（i: 0~角块  1~棱块）
      for (cube_sticker_t i = STICKER_CORNER_START_IDX; i <= STICKER_EDGE_START_IDX; ++i) {
        cube_sticker_t sticker_src_idx = i;
        cube_sticker_t sticker_dest_idx = (sticker_src_idx + sticker_idx_offset) % STICKER_PER_FACE;
        temp_sticker_color = cube[sticker_idx_prefix + sticker_src_idx];
        cube[sticker_idx_prefix + sticker_src_idx] = cube[sticker_idx_prefix + sticker_dest_idx];
        cube[sticker_idx_prefix + sticker_dest_idx] = temp_sticker_color;

        sticker_src_idx += STICKER_GAP;
        sticker_dest_idx += STICKER_GAP;
        temp_sticker_color = cube[sticker_idx_prefix + sticker_src_idx];
        cube[sticker_idx_prefix + sticker_src_idx] = cube[sticker_idx_prefix + sticker_dest_idx];
        cube[sticker_idx_prefix + sticker_dest_idx] = temp_sticker_color;
      }
      return; // 分支已经处理旋转面颜色更新，直接返回
    }
    case CUBE_TURN_270: {
      // 相邻面颜色更新
      for (uint8_t i = 0; i < ARRAY_SIZE(adj_stickers); ++i) {
        temp_sticker_color = cube[adj_faces[CUBE_SIDE_U] * STICKER_PER_FACE + adj_stickers[i]];
        cube[adj_faces[CUBE_SIDE_U] * STICKER_PER_FACE + adj_stickers[i]] = cube[adj_faces[CUBE_SIDE_R] * STICKER_PER_FACE + OFFSET_R(adj_stickers[i])];
        cube[adj_faces[CUBE_SIDE_R] * STICKER_PER_FACE + OFFSET_R(adj_stickers[i])] = cube[adj_faces[CUBE_SIDE_D] * STICKER_PER_FACE + adj_stickers[i]];
        cube[adj_faces[CUBE_SIDE_D] * STICKER_PER_FACE + adj_stickers[i]] = cube[adj_faces[CUBE_SIDE_L] * STICKER_PER_FACE + OFFSET_L(adj_stickers[i])];
        cube[adj_faces[CUBE_SIDE_L] * STICKER_PER_FACE + OFFSET_L(adj_stickers[i])] = temp_sticker_color;
      }
      break;
    }
    default:
      return; // 非法转动角度，忽略
  }
  // 旋转面颜色更新（i: 0~角块  1~棱块）
  for (cube_sticker_t i = STICKER_CORNER_START_IDX; i <= STICKER_EDGE_START_IDX; ++i) {
    cube_sticker_t sticker_src_idx = i;
    cube_sticker_t sticker_dest_idx = (sticker_src_idx + sticker_idx_offset) % STICKER_PER_FACE;
    temp_sticker_color = cube[sticker_idx_prefix + sticker_src_idx];
    cube[sticker_idx_prefix + sticker_src_idx] = cube[sticker_idx_prefix + sticker_dest_idx];
    uint8_t update_times = CUBE_TURN_CYCLE - 2;
    while (update_times--) {
      sticker_src_idx = sticker_dest_idx;
      sticker_dest_idx = (sticker_src_idx + sticker_idx_offset) % STICKER_PER_FACE;
      cube[sticker_idx_prefix + sticker_src_idx] = cube[sticker_idx_prefix + sticker_dest_idx];
    }
    cube[sticker_idx_prefix + sticker_dest_idx] = temp_sticker_color;
  }
}

/**
 * @brief 解析单步打乱字符串，翻译为turn_face和turn_degree
 * @param face_char 单步打乱面字母
 * @param suffix_char 单步打乱后缀
 */
static void cube_parse_step(const char face_char, const char suffix_char) {
  // 判断旋转面
  cube_face_t turn_face;
  switch (face_char) {
    case 'F':
      turn_face = CUBE_FACE_F_IDX;
      break;
    case 'R':
      turn_face = CUBE_FACE_R_IDX;
      break;
    case 'B':
      turn_face = CUBE_FACE_B_IDX;
      break;
    case 'L':
      turn_face = CUBE_FACE_L_IDX;
      break;
    case 'U':
      turn_face = CUBE_FACE_U_IDX;
      break;
    case 'D':
      turn_face = CUBE_FACE_D_IDX;
      break;
    default:
      return;
  }

  // 判断转动角度
  cube_turn_t turn_degree;
  switch (suffix_char) {
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
  cube_turn(turn_face, turn_degree);
}

/**
 * @brief 魔方解析打乱字符串，分步调用单步打乱方法
 * @details 传入的 scramble_alg 必须是以 '\0' 结尾的合法 C 字符串；
 *          若使用字符数组手动构造打乱公式，需确保末尾包含 '\0'。
 * @param scramble_alg 打乱公式字符串
 */
void cube_update_color(const char *scramble_alg) {
  if (scramble_alg == NULL) return; // 空指针保护
  const char *ptr = scramble_alg;
  while (*ptr != '\0') {
    //while (*ptr == ' ') ptr++; // 跳过空格，这一步在内部调用时可以省略，因为存储结构不包含空格
    if (*ptr == 'F' || *ptr == 'R' || *ptr == 'B' || *ptr == 'L' || *ptr == 'U' || *ptr == 'D') {
      const char face_char = *ptr;
      char suffix_char;
      switch (*(ptr + 1)) {
        case '2':
        case '\'': {
          suffix_char = *(ptr + 1);
          ptr++; // 这个单步包含两个字符，ptr额外移动一位
          cube_parse_step(face_char, suffix_char);
          break;
        }
        case '\0':
        case ' ':
        default: { // 包含 F R B L U D
          suffix_char = '\0';
          cube_parse_step(face_char, suffix_char);
          break;
        }
      }
    }
    ptr++;
  }
}

/**
 * @brief 获取魔方颜色信息的接口
 * @return 指向颜色信息数组首元素的只读指针
 */
const cube_color_t *cube_get_color(void) {
  return cube;
}