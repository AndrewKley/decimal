#include "../s21_decimal.h"

int s21_truncate(s21_decimal value, s21_decimal *result) {
  int status = S21_OK;
  if (result) {
    s21_init_decimal(result);
    *result = div10_n(value, s21_get_scale(value), 0);
    if (!result->bits[0] && !result->bits[1] && !result->bits[2]) {
      s21_clear_bit(result, 127);
    }
  } else {
    status = S21_ERROR;
  }

  return status;
}
