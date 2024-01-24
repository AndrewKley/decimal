#include "../s21_decimal.h"

int s21_is_greater(s21_decimal value_1, s21_decimal value_2) {
  int res = S21_FALSE;
  if (!s21_is_less(value_1, value_2) && !s21_is_equal(value_1, value_2))
    res = S21_TRUE;
  return res;
}
