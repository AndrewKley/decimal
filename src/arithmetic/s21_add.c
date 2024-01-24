#include "../s21_decimal.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int v1_sign = get_sign(value_1);
  int v2_sign = get_sign(value_2);
  int is_equal_sign = !(v1_sign ^ v2_sign);
  int status = S21_OK;

  if (result) {
    s21_init_decimal(result);
    if (get_exp(value_1) != get_exp(value_2)) normalize(&value_1, &value_2);
    if (is_equal_sign) {
      *result = sum_mantissas(value_1, value_2);
      *result = set_exp(*result, get_exp(value_1));
      if (v1_sign) *result = invert_sign(*result);
      status = check_status(value_1, value_2, *result, status);
    } else if (v1_sign) {
      // if 1 number is negative
      status = s21_sub(value_2, invert_sign(value_1), result);
    } else {
      // if 2 number is negative
      status = s21_sub(value_1, invert_sign(value_2), result);
    }
  } else {
    status = S21_ERROR;
  }
  return status;
}

int check_status(s21_decimal value_1, s21_decimal value_2, s21_decimal result,
                 int status) {
  int v1_high = s21_get_bit(value_1, 95);
  int v2_high = s21_get_bit(value_2, 95);
  int r_high = s21_get_bit(result, 95);
  int v1_sign = get_sign(value_1);

  status = (((v1_high || v2_high) && !r_high) || (v1_high && v2_high));
  if (status && v1_sign) status++;
  return status;
}
