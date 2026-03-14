/**
 * @file draw_scramble.h
 * @brief 用于根据打乱公式计算打乱后的颜色信息
 * @author tosh1ue
 * @date 2026-02-03
 */

#ifndef DRAW_SCRAMBLE_H
#define DRAW_SCRAMBLE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CUBE_ORDER 3 // 魔方阶数
#define CUBE_FACE_NUM 6 // 魔方面数
#define STICKER_PER_FACE (CUBE_ORDER * CUBE_ORDER - 1) // 魔方每面色块数，省略中心块
#define CUBE_TURN_CYCLE 4 // 魔方转动周期
#define CUBE_STICKER_NUM (CUBE_FACE_NUM * STICKER_PER_FACE) // 魔方总色块数

/**
 * @brief 魔方转动角度
 */
#define CUBE_TURN_90 1 // F
#define CUBE_TURN_180 2 // F2
#define CUBE_TURN_270 3 // F'
typedef uint8_t cube_turn_t;

/**
 * @brief 魔方颜色
 */
#define CUBE_COLOR_GREEN 0
#define CUBE_COLOR_WHITE 1
#define CUBE_COLOR_RED 2
#define CUBE_COLOR_BLUE 3
#define CUBE_COLOR_YELLOW 4
#define CUBE_COLOR_ORANGE 5
typedef uint8_t cube_color_t;

/**
 * @brief 魔方面索引值
 */
#define CUBE_FACE_F_IDX 0
#define CUBE_FACE_U_IDX 1
#define CUBE_FACE_R_IDX 2
#define CUBE_FACE_B_IDX 3
#define CUBE_FACE_D_IDX 4
#define CUBE_FACE_L_IDX 5
typedef uint8_t cube_face_t;

/**
 * @brief 相邻面在MAP中的索引
 */
#define CUBE_SIDE_U 0
#define CUBE_SIDE_R 1
#define CUBE_SIDE_D 2
#define CUBE_SIDE_L 3

/**
 * @brief 魔方颜色结构，由于中心块不变，无需存储
 * @details 用face_idx * sticker_per_face + sticker_idx 取值
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

/**
 * @brief 面内色块颜色交换相关宏定义
 */
#define STICKER_CORNER 0
#define STICKER_EDGE 1
#define STICKER_GAP 2

#define STICKER_OFFSET_R 2
#define STICKER_OFFSET_L 6
#define OFFSET_R(sticker_idx) ((sticker_idx + STICKER_OFFSET_R) % STICKER_PER_FACE)
#define OFFSET_L(sticker_idx) ((sticker_idx + STICKER_OFFSET_L) % STICKER_PER_FACE)

void cube_reset_color(void);

void cube_update_color(char *scramble_alg);

const cube_color_t* cube_get_color(void);

#ifdef __cplusplus
}
#endif

#endif