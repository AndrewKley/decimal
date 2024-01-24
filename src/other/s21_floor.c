#include "../s21_decimal.h"

int s21_floor(s21_decimal value, s21_decimal *result) {
  int status = S21_OK;
  if (result) {
    s21_init_decimal(result);
    s21_decimal s21_one = {{1, 0, 0, 0}};
    s21_decimal s21_trunc = {{0, 0, 0, 0}};

    s21_truncate(value, &s21_trunc);
    s21_sub(value, s21_trunc, result);

    if (!result->bits[0] && !result->bits[1] && !result->bits[2]) {
      s21_clear_bit(result, 127);
    }
    if (s21_get_bit(*result, 127)) {
      s21_sub(value, *result, result);
      s21_sub(*result, s21_one, result);
    } else {
      s21_sub(value, *result, result);
    }
    s21_mul(*result, s21_one, result);
  } else {
    status = S21_ERROR;
  }

  return status;
}
