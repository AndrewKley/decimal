#include "../s21_decimal.h"

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int status = S21_OK;
  if (result) {
    int is_negative = 0;
    int v1_sign = get_sign(value_1);
    int v2_sign = get_sign(value_2);
    int is_equal_sign = !(v1_sign ^ v2_sign);

    s21_init_decimal(result);
    if (get_exp(value_1) != get_exp(value_2)) normalize(&value_1, &value_2);

    if (is_equal_sign) {
      s21_decimal mod1 = _mod(value_1);
      s21_decimal mod2 = _mod(value_2);
      if (s21_is_less(mod1, mod2)) {
        value_1 = _sub_mantissas(value_1);
        is_negative = 1;
      } else {
        value_2 = _sub_mantissas(value_2);
      }
      s21_add(value_1, value_2, result);
    } else {
      status = s21_add(value_1, invert_sign(value_2), result);
    }
    if (is_negative) *result = invert_sign(*result);
  } else {
    status = S21_ERROR;
  }
  return status;
}
