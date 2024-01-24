// Copyright [2023] arianneg
#include "../s21_decimal.h"

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int status = S21_OK;
  unsigned int scale = s21_get_scale(src);

  if (dst) {
    *dst = 0;
  } else {
    status = S21_ERROR;
  }
  if (!status && scale < 29 && !src.bits[2] && !(src.bits[1] >> (scale - 1))) {
    int result = ((unsigned int)src.bits[0] >> scale) |
                 ((unsigned int)src.bits[1] << (32 - scale));
    if (s21_get_bit(src, 127)) {
      result *= -1;
    }
    *dst = result;
  } else {
    status = S21_ERROR;
  }

  return status;
}
