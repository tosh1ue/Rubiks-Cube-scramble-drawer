#include "draw_scramble/draw_scramble.h"
#include <stdio.h>
#include <stdint.h>

uint8_t nums[3] = {1, 2, 3};

void test_print(uint8_t* nums) {
  printf("%d %d %d\n", nums[0], nums[1], nums[2]);
}

void main(void)  {

  // F L R B
  // printf("+-----------+\n");
  // printf("| %d | %d | %d |\n", 3, 3, 3);
  // printf("+-----------+\n");
  // printf("| %d | %d | %d |\n", 3, 3, 3);
  // printf("+-----------+\n");
  // printf("| %d | %d | %d |\n", 3, 3, 3);
  // printf("+-----------+\n");
  test_print(nums);

  while (1) {

  }
}