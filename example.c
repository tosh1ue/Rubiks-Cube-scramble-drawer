#include "./draw_scramble/draw_scramble.h"
#include <stdio.h>
#include <stdint.h>

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
    ((index) == 0 ? COLOR_GREEN : \
     (index) == 1 ? COLOR_RED : \
     (index) == 2 ? COLOR_BLUE : \
     (index) == 3 ? COLOR_ORANGE : \
     (index) == 4 ? COLOR_WHITE : \
     (index) == 5 ? COLOR_YELLOW : \
     COLOR_WHITE)  // 默认返回白色

//static uint8_t cube[48];
// static uint8_t cube[48] = {
//   2,3,2,0,1,4,0,3,
//   3,1,1,2,1,1,5,4,
//   5,5,3,3,0,3,4,5,
//   5,1,3,4,4,5,4,0,
//   0,1,2,0,4,5,5,4,
//   3,2,0,2,2,2,1,0
// };

/**
 * @brief 打印魔方展开图
 * @param cube 魔方结构体指针
 * @return 无
 */
void print_cube_as_num(const uint8_t* cube) {
  printf("print rubik's cube as num:\n\n");

  // U
  printf("                +-----------+\n");
  printf("                | %d | %d | %d |\n", cube[4*8+0], cube[4*8+1], cube[4*8+2]);
  printf("                +-----------+\n");
  printf("                | %d | %d | %d |\n", cube[4*8+7],      4     , cube[4*8+3]);
  printf("                +-----------+\n");
  printf("                | %d | %d | %d |\n", cube[4*8+6], cube[4*8+5], cube[4*8+4]);
  printf("                +-----------+\n");
  printf("\n");

  // F L R B
  printf("+-----------+   +-----------+   +-----------+   +-----------+\n");
  printf("| %d | %d | %d |   | %d | %d | %d |   | %d | %d | %d |   | %d | %d | %d |\n", cube[3*8+0], cube[3*8+1], cube[3*8+2], cube[0*8+0], cube[0*8+1], cube[0*8+2], cube[1*8+0], cube[1*8+1], cube[1*8+2], cube[2*8+0], cube[2*8+1], cube[2*8+2]);
  printf("+-----------+   +-----------+   +-----------+   +-----------+\n");
  printf("| %d | %d | %d |   | %d | %d | %d |   | %d | %d | %d |   | %d | %d | %d |\n", cube[3*8+7],      3     , cube[3*8+3], cube[0*8+7],      0     , cube[0*8+3], cube[1*8+7],      1     , cube[1*8+3], cube[2*8+7],      2     , cube[2*8+3]);
  printf("+-----------+   +-----------+   +-----------+   +-----------+\n");
  printf("| %d | %d | %d |   | %d | %d | %d |   | %d | %d | %d |   | %d | %d | %d |\n", cube[3*8+6], cube[3*8+5], cube[3*8+4], cube[0*8+6], cube[0*8+5], cube[0*8+4], cube[1*8+6], cube[1*8+5], cube[1*8+4], cube[2*8+6], cube[2*8+5], cube[2*8+4]);
  printf("+-----------+   +-----------+   +-----------+   +-----------+\n");
  printf("\n");

  // D
  printf("                +-----------+\n");
  printf("                | %d | %d | %d |\n", cube[5*8+0], cube[5*8+1], cube[5*8+2]);
  printf("                +-----------+\n");
  printf("                | %d | %d | %d |\n", cube[5*8+7],      5     , cube[5*8+3]);
  printf("                +-----------+\n");
  printf("                | %d | %d | %d |\n", cube[5*8+6], cube[5*8+5], cube[5*8+4]);
  printf("                +-----------+\n");
}

/**
 * @brief 打印魔方彩色展开图
 * @param cube 魔方结构体指针
 * @return 无
 */
void print_cube_with_color(const uint8_t* cube) {
  printf("print rubik's cube with color:\n\n");

  // U
  printf("         %s  %s  %s  "RESET"\n", GET_COLOR(cube[4*8+0]), GET_COLOR(cube[4*8+1]), GET_COLOR(cube[4*8+2]));
  printf("         %s  %s  %s  "RESET"\n", GET_COLOR(cube[4*8+7]), GET_COLOR(     4     ), GET_COLOR(cube[4*8+3]));
  printf("         %s  %s  %s  "RESET"\n", GET_COLOR(cube[4*8+6]), GET_COLOR(cube[4*8+5]), GET_COLOR(cube[4*8+4]));
  printf("\n");

  // F R B L
  printf("%s  %s  %s  "RESET"   %s  %s  %s  "RESET"   %s  %s  %s  "RESET"   %s  %s  %s  "RESET"\n", GET_COLOR(cube[3*8+0]), GET_COLOR(cube[3*8+1]), GET_COLOR(cube[3*8+2]), GET_COLOR(cube[0*8+0]), GET_COLOR(cube[0*8+1]), GET_COLOR(cube[0*8+2]), GET_COLOR(cube[1*8+0]), GET_COLOR(cube[1*8+1]), GET_COLOR(cube[1*8+2]), GET_COLOR(cube[2*8+0]), GET_COLOR(cube[2*8+1]), GET_COLOR(cube[2*8+2]));
  printf("%s  %s  %s  "RESET"   %s  %s  %s  "RESET"   %s  %s  %s  "RESET"   %s  %s  %s  "RESET"\n", GET_COLOR(cube[3*8+7]), GET_COLOR(     3     ), GET_COLOR(cube[3*8+3]), GET_COLOR(cube[0*8+7]), GET_COLOR(     0     ), GET_COLOR(cube[0*8+3]), GET_COLOR(cube[1*8+7]), GET_COLOR(     1     ), GET_COLOR(cube[1*8+3]), GET_COLOR(cube[2*8+7]), GET_COLOR(     2     ), GET_COLOR(cube[2*8+3]));
  printf("%s  %s  %s  "RESET"   %s  %s  %s  "RESET"   %s  %s  %s  "RESET"   %s  %s  %s  "RESET"\n", GET_COLOR(cube[3*8+6]), GET_COLOR(cube[3*8+5]), GET_COLOR(cube[3*8+4]), GET_COLOR(cube[0*8+6]), GET_COLOR(cube[0*8+5]), GET_COLOR(cube[0*8+4]), GET_COLOR(cube[1*8+6]), GET_COLOR(cube[1*8+5]), GET_COLOR(cube[1*8+4]), GET_COLOR(cube[2*8+6]), GET_COLOR(cube[2*8+5]), GET_COLOR(cube[2*8+4]));
  printf("\n");

  // D
  printf("         %s  %s  %s  "RESET"\n", GET_COLOR(cube[5*8+0]), GET_COLOR(cube[5*8+1]), GET_COLOR(cube[5*8+2]));
  printf("         %s  %s  %s  "RESET"\n", GET_COLOR(cube[5*8+7]), GET_COLOR(     5     ), GET_COLOR(cube[5*8+3]));
  printf("         %s  %s  %s  "RESET"\n", GET_COLOR(cube[5*8+6]), GET_COLOR(cube[5*8+5]), GET_COLOR(cube[5*8+4]));
  printf("\n");
}

int main(void) {
  cube_reset_color(); // 初始化魔方颜色信息
  const cube_t* cube = cube_get_color();
  print_cube_with_color(cube);
  //print_cube_as_num(cube);
  return 0;
}