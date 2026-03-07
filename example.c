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
 * @brief 使用宏函数进行映射
 */
#define GET_COLOR(index) \
    ((index) == 0 ? COLOR_GREEN : \
     (index) == 1 ? COLOR_RED : \
     (index) == 2 ? COLOR_BLUE : \
     (index) == 3 ? COLOR_ORANGE : \
     (index) == 4 ? COLOR_WHITE : \
     (index) == 5 ? COLOR_YELLOW : \
     COLOR_WHITE)  // 默认返回白色

static uint8_t cube[48];

void cube_color_init() {
  for(int i = 0; i < 6; ++i) {
    memset(&cube[i * 8], i, 8);
  }
}

/**
 * @brief 打印魔方展开图
 * @param cube 魔方结构体指针
 * @return 无
 */
void print_cube_as_num(uint8_t* cube) {
  printf("print rubik's cube as num:\n\n");

  // U
  printf("                +-----------+\n");
  printf("                | %d | %d | %d |\n", cube[4*8+0], cube[4*8+1], cube[4*8+2]);
  printf("                +-----------+\n");
  printf("                | %d | %d | %d |\n", cube[4*8+7], 4, cube[4*8+3]);
  printf("                +-----------+\n");
  printf("                | %d | %d | %d |\n", cube[4*8+6], cube[4*8+5], cube[4*8+4]);
  printf("                +-----------+\n");
  printf("\n");

  // F L R B
  printf("+-----------+   +-----------+   +-----------+   +-----------+\n");
  printf("| %d | %d | %d |   | %d | %d | %d |   | %d | %d | %d |   | %d | %d | %d |\n", 3, 3, 3, 0, 0, 0, 1, 1, 1, 2, 2, 2);
  printf("+-----------+   +-----------+   +-----------+   +-----------+\n");
  printf("| %d | %d | %d |   | %d | %d | %d |   | %d | %d | %d |   | %d | %d | %d |\n", 3, 3, 3, 0, 0, 0, 1, 1, 1, 2, 2, 2);
  printf("+-----------+   +-----------+   +-----------+   +-----------+\n");
  printf("| %d | %d | %d |   | %d | %d | %d |   | %d | %d | %d |   | %d | %d | %d |\n", 3, 3, 3, 0, 0, 0, 1, 1, 1, 2, 2, 2);
  printf("+-----------+   +-----------+   +-----------+   +-----------+\n");
  printf("\n");

  // D
  printf("                +-----------+\n");
  printf("                | %d | %d | %d |\n", 5, 5, 5);
  printf("                +-----------+\n");
  printf("                | %d | %d | %d |\n", 5, 5, 5);
  printf("                +-----------+\n");
  printf("                | %d | %d | %d |\n", 5, 5, 5);
  printf("                +-----------+\n");
}

/**
 * @brief 打印魔方彩色展开图
 * @param cube 魔方结构体指针
 * @return 无
 */
void print_cube_with_color(uint8_t* cube) {
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
  printf("         %s  %s  %s  "RESET"\n", GET_COLOR(cube[5*8+7]), GET_COLOR(5), GET_COLOR(cube[5*8+3]));
  printf("         %s  %s  %s  "RESET"\n", GET_COLOR(cube[5*8+6]), GET_COLOR(cube[5*8+5]), GET_COLOR(cube[5*8+4]));
  printf("\n");
}

int main(void) {
  cube_color_init(); // 初始化魔方颜色信息
  print_cube_with_color(cube);
  // print_cube(cube_get_color());
  return 0;
}