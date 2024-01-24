#include "../s21_decimal.h"

int s21_is_less(s21_decimal value_1, s21_decimal value_2) {
  int res = S21_FALSE;
  int neg_zero = check_neg_zero(value_1) || check_neg_zero(value_2);
  if (get_sign(value_1) != get_sign(value_2) && !neg_zero) {
    if (get_sign(value_1) == 1 && get_sign(value_2) == 0) {
      res = S21_TRUE;
    }
  } else {
    normalize(&value_1, &value_2);
    int check_out = 1;
    for (int i = 95; check_out && 0 <= i; --i) {
      if (s21_get_bit(value_1, i) != s21_get_bit(value_2, i)) {
        if (!s21_get_bit(value_1, i) && s21_get_bit(value_2, i)) {
          res = S21_TRUE;
        } else {
          check_out = 0;
        }
      }
    }
  }
  if (get_sign(value_1) && get_sign(value_2) && !neg_zero &&
      s21_is_not_equal(value_1, value_2)) {
    res = !res;
  }
  return res;
}
