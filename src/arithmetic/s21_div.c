#include "../s21_decimal.h"

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int status = S21_OK;
  int neg = get_sign(value_1) ^ get_sign(value_2);
  int exp = get_exp(value_1) - get_exp(value_2);
  if (result) {
    s21_init_decimal(result);
    if (!decimal_is_zero(value_1)) {
      status = _common_div(value_1, value_2, result, &value_1);
      if (!decimal_is_zero(value_1) && !status)
        status = _find_part_dec(&value_1, value_2, result, &exp);
    }
  } else {
    status = S21_ERROR;
  }
  if (!status) {
    status = post_div(value_1, result, status, neg, exp);
  }
  return status;
}

int _common_div(s21_decimal num1, s21_decimal num2, s21_decimal *res,
                s21_decimal *remainder) {
  s21_init_decimal(res);
  num1 = clear_prop(num1);
  num2 = clear_prop(num2);
  int status = S21_OK;
  s21_decimal one = {{1, 0, 0, 0}};
  if (decimal_is_zero(num2)) {
    status = S21_ERROR_ZERO_DIVISION;
  } else if (!decimal_is_zero(num1) && !s21_is_less(num1, num2)) {
    s21_decimal tmp = {0};
    int count_shift = _normalize_for_div(num1, num2, &tmp);
    while (count_shift >= 0) {
      if (s21_is_less(num1, tmp) && count_shift) {
        // if sub is unpossible
        *res = shift_left(*res);
      } else if (!s21_is_less(num1, num2)) {
        // subtraction in a column
        s21_sub(num1, tmp, &num1);
        s21_add(*res, one, res);
        if (s21_is_less_or_equal(num2, num1))
          *res = shift_left(*res);
        else if (count_shift)  // subtraction completed but significant zeros
                               // remain
          *res = shift(*res, 1,
                       count_shift);  // write significant zeros in the result
      }
      if (s21_is_less(num1, num2)) count_shift = 0;  // division completed
      tmp = shift(num2, 1, --count_shift);           // shift num2 further
    }
  }
  if (!status)
    *remainder = num1;
  else
    s21_init_decimal(remainder);
  return status;
}

int _find_part_dec(s21_decimal *n1, s21_decimal n2, s21_decimal *integer_result,
                   int *exp) {
  int status = S21_OK;
  *n1 = clear_prop(*n1);
  n2 = clear_prop(n2);
  if (s21_is_less(_mod(*n1), _mod(n2))) {
    int n1_big = 0;
    if (get_overhead(n2) < 4) {
      if (get_overhead(*n1) != 96) *n1 = div10_n(*n1, 1, 1);
      n1_big = 1;
      n2 = div10_n(n2, 1, 1);
    }
    while (!decimal_is_zero(*n1) && get_overhead(*integer_result) > 4 &&
           *exp < 28) {
      *n1 = exp10_mantissa(*n1);  // add one zero after point
      *exp += 1;
      *integer_result = exp10_mantissa(*integer_result);
      *integer_result = clear_prop(*integer_result);
      s21_decimal temp;
      status = _common_div(*n1, n2, &temp, n1);
      s21_add(*integer_result, temp,
              integer_result);  // sum previous result and current result(temp)
    }
    if (n1_big) {
      *n1 = exp10_mantissa(*n1);
      n2 = exp10_mantissa(n2);
    }
    if (!decimal_is_zero(*n1) && !decimal_is_zero(*integer_result) && !status)
      *integer_result = _round_remainder(*n1, n2, *integer_result);
  } else {
    status = S21_ERROR_TOO_BIG;
  }
  return status;
}

s21_decimal _round_remainder(s21_decimal num1, s21_decimal num2,
                             s21_decimal res) {
  s21_decimal remains, n1 = exp10_mantissa(num1);
  s21_decimal one = {{1, 0, 0, 0}};
  int remander = 4;
  _common_div(n1, num2, &num1, &remains);
  if (num1.bits[0] > remander) s21_add(res, one, &res);
  return res;
}

int post_div(s21_decimal n1, s21_decimal *res, int status, int neg, int exp) {
  if (!status) {
    // if it's not divide by zero
    int zero = decimal_is_zero(*res);
    if (neg && !zero) *res = invert_sign(*res);
    *res = remove_zero(*res);
    if (exp >= 0 && !zero) {
      *res = set_exp(*res, exp);
    } else if ((int)(get_overhead(*res) / 3.3) >= -exp && !zero) {
      *res = max_exp10(*res, -exp);
      *res = set_exp(*res, 0);
    } else if (!zero) {  // if too big or +-infinity
      status = (neg) ? 2 : 1;
    } else if (!decimal_is_zero(n1)) {
      status = S21_ERROR_TOO_SMALL;  // if too small
    }
  }
  return status;
}

int _normalize_for_div(s21_decimal num1, s21_decimal num2,
                       s21_decimal *result) {
  int count_shift = get_overhead(num2) - get_overhead(num1);
  s21_decimal tmp = shift(num2, 1, count_shift);
  if (s21_is_less(num1, tmp)) {
    // if shift is strong
    tmp = shift_right(tmp);
    count_shift--;
  }
  *result = tmp;
  return count_shift;
}
