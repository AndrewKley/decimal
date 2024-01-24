#include "../s21_decimal.h"

int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2) {
  int res = S21_FALSE;
  int neg_zero = check_neg_zero(value_1) || check_neg_zero(value_2);
  if ((get_sign(value_1) != get_sign(value_2)) && !neg_zero) {
    res = S21_TRUE;
  } else if (get_exp(value_1) != get_exp(value_2)) {
    res = S21_TRUE;
  } else {
    for (int i = 0; i <= 95; i++) {
      if (s21_get_bit(value_1, i) != s21_get_bit(value_2, i)) {
        res = S21_TRUE;
      }
    }
  }
  return res;
}
