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

/**
 * @brief 生成随机数的方法
 * @details 目前使用C语言标准库生成，移植到不同平台时需要手动重新实现，同时移除srand()
 * @return 生成的随机数
 */
static uint8_t get_random(void) {
  return (uint8_t)( rand() % (UINT8_MAX + 1) ); // NOLINT(cert-msc50-cpp)
}

/**
 * @brief 生成WCA标准打乱公式
 * @details 使用random-move，基于以下原则过滤：
 *          1.连续的两个旋转不能是同一面，例如：R R'
 *          2.连续的三个旋转不能都是对立面，例如：R L R
 * @param scramble_alg 打乱公式缓冲区指针
 * @param len 要生成的打乱长度
 */
void cube_generate_scramble(char *scramble_alg, uint8_t len) {
  if (scramble_alg == NULL) return;
  if (!IS_SCRAMBLE_LEN_VALID(len)) len = SCRAMBLE_DEFAULT_LEN;
  memset(scramble_alg, '\0', len * 3); // 重置打乱公式缓冲区
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
      // 过滤连续两个旋转都是同一面
      if (ARE_FACES_SAME(move_face_idx, move_last_face_idx)) {
        continue;
      }
      // 过滤连续三个旋转都是对立面
      if (ARE_FACES_SAME(move_face_idx, move_second_last_face_idx) && ARE_FACES_OPPOSITE(move_last_face_idx, move_second_last_face_idx)) {
        continue;
      }
    } else if (step_cnt >= 1) {
      if (ARE_FACES_SAME(move_face_idx, move_last_face_idx)) {
        continue;
      }
    }
    move_second_last_face_idx = move_last_face_idx;
    move_last_face_idx = move_face_idx;
    step_cnt++; // 更新当前已经生成步数长度

    // 添加单步打乱
    switch (move_face_idx) {
      case MOVE_FACE_F:
        *scramble_pointer = 'F';
        break;
      case MOVE_FACE_U:
        *scramble_pointer = 'U';
        break;
      case MOVE_FACE_R:
        *scramble_pointer = 'R';
        break;
      case MOVE_FACE_B:
        *scramble_pointer = 'B';
        break;
      case MOVE_FACE_D:
        *scramble_pointer = 'D';
        break;
      case MOVE_FACE_L:
        *scramble_pointer = 'L';
        break;
      default:
        continue;
    }
    scramble_pointer++;
    switch (move_angle_idx) {
      case MOVE_ANGLE_90:
        break;
      case MOVE_ANGLE_180:
        *scramble_pointer = '2';
        scramble_pointer++;
        break;
      case MOVE_ANGLE_270:
        *scramble_pointer = '\'';
        scramble_pointer++;
        break;
      default:
        break;
    }
    // 单步打乱后添加空格，调试用
    // 正式使用可以注释或直接移除
    *scramble_pointer = ' ';
    scramble_pointer++;
  }
}