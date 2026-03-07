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
#define CUBE_COLOR_RED 1
#define CUBE_COLOR_BLUE 2
#define CUBE_COLOR_ORANGE 3
#define CUBE_COLOR_WHITE 4
#define CUBE_COLOR_YELLOW 5
typedef uint8_t cube_color_t;

typedef uint8_t cube_t;

void cube_reset_color(void);

void cube_scramble_alg_parse(char *scramble_alg);

const cube_t* cube_get_color(void);

#ifdef __cplusplus
}
#endif

#endif