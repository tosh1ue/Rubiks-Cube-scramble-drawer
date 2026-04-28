/**
 * @file scramble_generator.h
 * @brief 生成WCA标准打乱公式
 * @author tosh1ue
 * @date 2026-04-21
 */

#ifndef SCRAMBLE_GENERATOR_H
#define SCRAMBLE_GENERATOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdlib.h>

#define SCRAMBLE_WITH_SPACE

#define MOVE_FACE_F       0
#define MOVE_FACE_U       1
#define MOVE_FACE_R       2
#define MOVE_FACE_B       3
#define MOVE_FACE_D       4
#define MOVE_FACE_L       5
#define MOVE_FACE_MAX     MOVE_FACE_L
typedef uint8_t move_face_t;

#define MOVE_ANGLE_90     0
#define MOVE_ANGLE_180    1
#define MOVE_ANGLE_270    2
#define MOVE_ANGLE_MAX    MOVE_ANGLE_270
typedef uint8_t move_angle_t;

#define RAND_RANGE        (RAND_MAX + 1)
#define UINT8_RANGE       (UINT8_MAX + 1)
#define MOVE_FACE_RANGE   (MOVE_FACE_MAX + 1)
#define MOVE_ANGLE_RANGE  (MOVE_ANGLE_MAX + 1)

#define MOVE_FACE_HALF                        ((MOVE_FACE_MAX + 1) / 2)
#define ARE_FACES_SAME(face_a, face_b)        (face_a == face_b)
#define ARE_FACES_OPPOSITE(face_a, face_b)    (face_a % MOVE_FACE_HALF == face_b % MOVE_FACE_HALF)

#define SCRAMBLE_MIN_LEN             20
#define SCRAMBLE_MAX_LEN             25
#define SCRAMBLE_DEFAULT_LEN         SCRAMBLE_MAX_LEN
#define IS_SCRAMBLE_LEN_VALID(len)   (len >= SCRAMBLE_MIN_LEN && len <= SCRAMBLE_MAX_LEN)

#ifdef SCRAMBLE_WITH_SPACE
#define SCRAMBLE_STEP_LEN 3
#else
#define SCRAMBLE_STEP_LEN 2
#endif

void cube_generate_scramble(char *scramble_alg, uint8_t len);

#ifdef __cplusplus
}
#endif

#endif
