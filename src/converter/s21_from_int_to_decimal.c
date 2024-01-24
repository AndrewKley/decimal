#include "../s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int status = S21_OK;

  if (dst) {
    s21_init_decimal(dst);
    dst->bits[0] = abs(src);
    if (src < 0) {
      s21_set_negative_bit(dst);
    }
  } else {
    status = S21_ERROR;
  }

  return status;
}
