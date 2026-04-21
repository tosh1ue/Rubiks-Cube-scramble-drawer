#include <stdio.h>
#include "scramble_engine.h"
#include "scramble_generator.h"

/**
 * @brief ANSI颜色代码（背景色）
 */
#define RESET "\033[0m"
#define COLOR_GREEN "\033[48;2;0;221;0m"
#define COLOR_RED "\033[48;2;255;0;0m"
#define COLOR_BLUE "\033[48;2;0;0;255m"
#define COLOR_ORANGE "\033[48;2;255;170;0m"
#define COLOR_WHITE "\033[48;2;255;255;255m"
#define COLOR_YELLOW "\033[48;2;255;255;0m"

/**
 * @brief 使用宏函数将魔方颜色索引映射到对应ANSI颜色代码
 */
#define GET_COLOR(index) \
    ((index) == CUBE_FACE_F_IDX ? COLOR_GREEN : \
     (index) == CUBE_FACE_R_IDX ? COLOR_RED : \
     (index) == CUBE_FACE_B_IDX ? COLOR_BLUE : \
     (index) == CUBE_FACE_L_IDX ? COLOR_ORANGE : \
     (index) == CUBE_FACE_U_IDX ? COLOR_WHITE : \
     (index) == CUBE_FACE_D_IDX ? COLOR_YELLOW : \
     COLOR_WHITE)  // 默认返回白色

/**
 * @brief 打印魔方展开图
 * @param cube 魔方颜色信息数组指针
 * @return 无
 */
void print_cube_as_num(const cube_color_t* cube) {
  printf("print rubik's cube as num:\n\n");

  // U
  printf("                +-----------+\n");
  printf("                | %d | %d | %d |\n", cube[1*8+6],   cube[1*8+7]  , cube[1*8+0]);
  printf("                +-----------+\n");
  printf("                | %d | %d | %d |\n", cube[1*8+5], CUBE_FACE_U_IDX, cube[1*8+1]);
  printf("                +-----------+\n");
  printf("                | %d | %d | %d |\n", cube[1*8+4],   cube[1*8+3]  , cube[1*8+2]);
  printf("                +-----------+\n");
  printf("\n");

  // F L R B
  printf("+-----------+   +-----------+   +-----------+   +-----------+\n");
  printf("| %d | %d | %d |   | %d | %d | %d |   | %d | %d | %d |   | %d | %d | %d |\n", cube[5*8+6],   cube[5*8+7]  , cube[5*8+0], cube[0*8+0],   cube[0*8+1]  , cube[0*8+2], cube[2*8+6],   cube[2*8+7]  , cube[2*8+0], cube[3*8+4],   cube[3*8+5]  , cube[3*8+6]);
  printf("+-----------+   +-----------+   +-----------+   +-----------+\n");
  printf("| %d | %d | %d |   | %d | %d | %d |   | %d | %d | %d |   | %d | %d | %d |\n", cube[5*8+5], CUBE_FACE_L_IDX, cube[5*8+1], cube[0*8+7], CUBE_FACE_F_IDX, cube[0*8+3], cube[2*8+5], CUBE_FACE_R_IDX, cube[2*8+1], cube[3*8+3], CUBE_FACE_B_IDX, cube[3*8+7]);
  printf("+-----------+   +-----------+   +-----------+   +-----------+\n");
  printf("| %d | %d | %d |   | %d | %d | %d |   | %d | %d | %d |   | %d | %d | %d |\n", cube[5*8+4],   cube[5*8+3]  , cube[5*8+2], cube[0*8+6],   cube[0*8+5]  , cube[0*8+4], cube[2*8+4],   cube[2*8+3]  , cube[2*8+2], cube[3*8+2],   cube[3*8+1]  , cube[3*8+0]);
  printf("+-----------+   +-----------+   +-----------+   +-----------+\n");
  printf("\n");

  // D
  printf("                +-----------+\n");
  printf("                | %d | %d | %d |\n", cube[4*8+2],   cube[4*8+3]  , cube[4*8+4]);
  printf("                +-----------+\n");
  printf("                | %d | %d | %d |\n", cube[4*8+1], CUBE_FACE_D_IDX, cube[4*8+5]);
  printf("                +-----------+\n");
  printf("                | %d | %d | %d |\n", cube[4*8+0],   cube[4*8+7]  , cube[4*8+6]);
  printf("                +-----------+\n");
}

/**
 * @brief 打印魔方彩色展开图
 * @param cube 魔方颜色信息数组指针
 * @return 无
 */
void print_cube_with_color(const cube_color_t* cube) {
  printf("print rubik's cube with color:\n\n");

  // U
  printf("         %s  %s  %s  "RESET"\n", GET_COLOR(cube[1*8+6]), GET_COLOR(  cube[1*8+7]  ), GET_COLOR(cube[1*8+0]));
  printf("         %s  %s  %s  "RESET"\n", GET_COLOR(cube[1*8+5]), GET_COLOR(CUBE_FACE_U_IDX), GET_COLOR(cube[1*8+1]));
  printf("         %s  %s  %s  "RESET"\n", GET_COLOR(cube[1*8+4]), GET_COLOR(  cube[1*8+3]  ), GET_COLOR(cube[1*8+2]));
  printf("\n");

  // F R B L
  printf("%s  %s  %s  "RESET"   %s  %s  %s  "RESET"   %s  %s  %s  "RESET"   %s  %s  %s  "RESET"\n", GET_COLOR(cube[5*8+6]), GET_COLOR(  cube[5*8+7]  ), GET_COLOR(cube[5*8+0]), GET_COLOR(cube[0*8+0]), GET_COLOR(  cube[0*8+1]  ), GET_COLOR(cube[0*8+2]), GET_COLOR(cube[2*8+6]), GET_COLOR(  cube[2*8+7]  ), GET_COLOR(cube[2*8+0]), GET_COLOR(cube[3*8+4]), GET_COLOR(  cube[3*8+5]  ), GET_COLOR(cube[3*8+6]));
  printf("%s  %s  %s  "RESET"   %s  %s  %s  "RESET"   %s  %s  %s  "RESET"   %s  %s  %s  "RESET"\n", GET_COLOR(cube[5*8+5]), GET_COLOR(CUBE_FACE_L_IDX), GET_COLOR(cube[5*8+1]), GET_COLOR(cube[0*8+7]), GET_COLOR(CUBE_FACE_F_IDX), GET_COLOR(cube[0*8+3]), GET_COLOR(cube[2*8+5]), GET_COLOR(CUBE_FACE_R_IDX), GET_COLOR(cube[2*8+1]), GET_COLOR(cube[3*8+3]), GET_COLOR(CUBE_FACE_B_IDX), GET_COLOR(cube[3*8+7]));
  printf("%s  %s  %s  "RESET"   %s  %s  %s  "RESET"   %s  %s  %s  "RESET"   %s  %s  %s  "RESET"\n", GET_COLOR(cube[5*8+4]), GET_COLOR(  cube[5*8+3]  ), GET_COLOR(cube[5*8+2]), GET_COLOR(cube[0*8+6]), GET_COLOR(  cube[0*8+5]  ), GET_COLOR(cube[0*8+4]), GET_COLOR(cube[2*8+4]), GET_COLOR(  cube[2*8+3]  ), GET_COLOR(cube[2*8+2]), GET_COLOR(cube[3*8+2]), GET_COLOR(  cube[3*8+1]  ), GET_COLOR(cube[3*8+0]));
  printf("\n");

  // D
  printf("         %s  %s  %s  "RESET"\n", GET_COLOR(cube[4*8+2]), GET_COLOR(  cube[4*8+3]  ), GET_COLOR(cube[4*8+4]));
  printf("         %s  %s  %s  "RESET"\n", GET_COLOR(cube[4*8+1]), GET_COLOR(CUBE_FACE_D_IDX), GET_COLOR(cube[4*8+5]));
  printf("         %s  %s  %s  "RESET"\n", GET_COLOR(cube[4*8+0]), GET_COLOR(  cube[4*8+7]  ), GET_COLOR(cube[4*8+6]));
  printf("\n");
}

int main(void) {
  const uint8_t scramble_len = 25;
  char scramble_alg[scramble_len * 2];
  cube_generate_scramble(scramble_alg, scramble_len);
  printf("generated scramble alg: %s\r\n", scramble_alg);

  cube_reset_color(); // 初始化魔方颜色信息
  const cube_color_t* cube = cube_get_color();
  cube_update_color(scramble_alg);
  //print_cube_with_color(cube); // 部分终端不支持ANSI颜色转换
  print_cube_as_num(cube);
  return 0;
}