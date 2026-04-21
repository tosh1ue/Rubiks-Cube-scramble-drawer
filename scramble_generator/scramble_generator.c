/**
 * @file scramble_generator.c
 * @brief 生成WCA标准打乱公式
 * @author tosh1ue
 * @date 2026-04-21
 */

#include "scramble_generator.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

static const char move_faces[MOVE_FACE_MAX + 1] = {
  'F', 'U', 'R', 'B', 'D', 'L'
};

/**
 * @brief 生成随机数的方法
 * @details 移植到不同平台时需要手动重新实现
 * @return 生成的随机数
 */
static uint8_t get_random(void) {
  return (uint8_t)( (uint32_t)rand() * UINT8_MAX / RAND_MAX );
}

void cube_generate_scramble(char *scramble_alg, uint8_t len) {
  if (scramble_alg == NULL) return;
  if (!IS_SCRAMBLE_LEN_VALID(len)) len = SCRAMBLE_DEFAULT_LEN;
  // NOLINTNEXTLINE(cert-msc51-cpp)
  srand(time(NULL) ^ (clock() << 16));
  memset(scramble_alg, '\0', len * 2); // 重置打乱公式缓冲区
  uint8_t step_cnt = 0; // 记录当前已经生成步数长度
  char *scramble_pointer = scramble_alg; // 记录当前指针位置
  uint8_t move_face_idx = UINT8_MAX;
  uint8_t move_angle_idx = UINT8_MAX;
  uint8_t move_last_face_idx = UINT8_MAX;
  uint8_t move_second_last_face_idx = UINT8_MAX;
  while (step_cnt < len) {
    move_face_idx = get_random() % (MOVE_FACE_MAX + 1);
    move_angle_idx = get_random() % (MOVE_ANGLE_MAX + 1);

    // 检验生成的单步打乱是否有效
    if (step_cnt >= 2) {
      if (ARE_FACES_OPPOSITE(move_face_idx, move_last_face_idx) && ARE_FACES_OPPOSITE(move_last_face_idx, move_second_last_face_idx)) {
        continue;
      }
    }
    if (step_cnt >= 1) {
      if (ARE_FACES_SAME(move_face_idx, move_last_face_idx)) {
        continue;
      }
    }
    move_second_last_face_idx = move_last_face_idx;
    move_last_face_idx = move_face_idx;
    step_cnt++; // 更新当前已经生成步数长度

    // 添加单步打乱
    *scramble_pointer = move_faces[move_face_idx];
    scramble_pointer++;
    switch (move_angle_idx) {
      case MOVE_ANGLE_90:
        continue;
      case MOVE_ANGLE_180:
        *scramble_pointer = '2';
        break;
      case MOVE_ANGLE_270:
        *scramble_pointer = '\'';
        break;
      default:
        continue;
    }
    scramble_pointer++;
  }
}