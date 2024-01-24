#include "../s21_decimal.h"

int s21_round(s21_decimal value, s21_decimal *result) {
  int status = S21_OK;
  if (result) {
    s21_init_decimal(result);
    s21_decimal s21_one = {{1, 0, 0, 0}};
    s21_decimal s21_half = {{2, 0, 0, 0}};
    s21_decimal s21_trunc = {{0, 0, 0, 0}};
    int sign = s21_get_bit(value, 127);

    s21_div(s21_one, s21_half, &s21_half);

    s21_clear_bit(&value, 127);
    s21_truncate(value, result);
    s21_sub(value, *result, &s21_trunc);

    if (s21_is_less(s21_half, s21_trunc)) {
      s21_add(*result, s21_one, result);
    }

    if (sign) {
      s21_set_bit(result, 127);
    }

    if (!result->bits[0] && !result->bits[1] && !result->bits[2]) {
      s21_clear_bit(result, 127);
    }
  } else {
    status = S21_ERROR;
  }

  return status;
}
