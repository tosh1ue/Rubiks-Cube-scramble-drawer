#ifndef DRAW_SCRAMBLE_H
#define DRAW_SCRAMBLE_H

#include <stdint.h>

/*
用于根据打乱公式生成打乱图案
*/

#define CUBE_FACE_ROW_NUM 3 // 魔方每面行数，即魔方阶数
#define CUBE_FACE_STICKER_NUM (CUBE_FACE_ROW_NUM * CUBE_FACE_ROW_NUM - 1) // 魔方每面色块数，省略中心块
#define CUBE_FACE_NUM 6 // 魔方面数
#define CUBE_TURN_CYCLE 4 // 魔方转动周期

/*
枚举魔方转动角度
*/
typedef enum {
  CUBE_TURN_90 = 1, // F
  CUBE_TURN_180 = 2, // F2
  CUBE_TURN_270 = 3, // F'
}Cube_Turn;

/*
定义魔方面的颜色存储结构，用一维数组表示，由于中心块不变，可以节省一个字节
*/
typedef uint8_t cube_face_t[CUBE_FACE_STICKER_NUM];

/*
定义魔方颜色结构
*/
// 取值用 &cube->face[FACE_IDX][STICKER_IDX]
typedef struct {
  cube_face_t face[CUBE_FACE_NUM];
}cube_t;

void cube_color_init(cube_t *cube);

void cube_scramble_alg_parse(cube_t *cube, char *scarmble_alg);

#endif