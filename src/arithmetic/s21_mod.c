#include "../s21_decimal.h"

int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int status = S21_OK;
  if (result) {
    s21_init_decimal(result);
    int v1_sign = get_sign(value_1);
    if (decimal_is_zero(value_2)) {
      status = S21_ERROR_ZERO_DIVISION;
      s21_init_decimal(&value_1);
    } else {
      normalize(&value_1, &value_2);
      if (decimal_is_zero(value_2)) {  // value_2 is too small
        status = S21_ERROR_TOO_SMALL;
        value_1 = value_2;
      } else {
        status = _common_div(value_1, value_2, result, &value_1);
      }
    }
    *result = value_1;
    *result = set_exp(*result, get_exp(value_2));
    if (v1_sign && !decimal_is_zero(*result)) *result = invert_sign(*result);
    *result = remove_zero(*result);
  } else {
    status = S21_ERROR;
  }
  return status;
}
