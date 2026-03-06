#include "draw_scramble/draw_scramble.h"
#include <stdio.h>
#include <stdint.h>

void main(void)  {
  // U
  printf("                +-----------+\n");
  printf("                | %d | %d | %d |\n", 4, 4, 4);
  printf("                +-----------+\n");
  printf("                | %d | %d | %d |\n", 4, 4, 4);
  printf("                +-----------+\n");
  printf("                | %d | %d | %d |\n", 4, 4, 4);
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
  printf("\033[32mshow color on terminal is available now\033[0m\n");
}