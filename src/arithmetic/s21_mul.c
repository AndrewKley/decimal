#include "../s21_decimal.h"

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int status = S21_OK;
  if (result) {
    s21_init_decimal(result);
    int overhead = _round(&value_1, &value_2);
    int exp = get_exp(value_1) + get_exp(value_2);
    int neg = (get_sign(value_1) ^ get_sign(value_2));
    int i = 95;
    value_1 = clear_prop(value_1);  // for sum mantissas
    if (decimal_is_zero(value_1) || decimal_is_zero(value_2)) i = -1;
    if (overhead < 96) status++;
    for (; !status && 0 <= i; --i)
      if (s21_get_bit(value_2, i))
        status = s21_add(*result, shift(value_1, 1, i), result);
    if (status == S21_ERROR && neg)
      status++;  // if the result is less than -max_dec
    if (!decimal_is_zero(*result)) {
      if (neg) *result = invert_sign(*result);
      *result = set_exp(*result, exp);
      *result = remove_zero(*result);
    }
  } else {
    status = S21_ERROR;
  }
  return status;
}

int _round(s21_decimal *value_1, s21_decimal *value_2) {
  int exp1 = get_exp(*value_1), exp2 = get_exp(*value_2);
  int o1 = get_overhead(*value_1), o2 = get_overhead(*value_2);
  int overhead = o1 + o2, flag1_zero = 0, flag2_zero = 0;
  int check = 1;
  // пока будет переполнение при перемножении или слишком маленькое чилсо
  while (check && ((exp1 + exp2 > 28) || (overhead > 190))) {
    // если оба чилса уже округлили до целых или
    // после округления value_1=0 выходим из цикла
    if (!(exp1 + exp2) || decimal_is_zero(*value_1)) {
      check = 0;
      // если value_1 еще можно округлить и value_2 можно или нельзя округлить
    } else if ((exp1 > exp2 && !flag1_zero) || (flag2_zero && !flag1_zero)) {
      s21_decimal temp = div10_n(*value_1, 1, 0);
      // если после окургления уже value_1=0 то ставим флаг что больше округлять
      // нельзя иначе окугляем дальше
      if (decimal_is_zero(temp)) {
        flag1_zero = 1;
      } else {
        *value_1 = temp;
        exp1 = get_exp(*value_1);
        o1 = get_overhead(*value_1);
      }
      // если value_2 еще можно округлить и value_1 можно или нельзя округлить
    } else if ((exp1 <= exp2 && !flag2_zero) || (flag1_zero && !flag2_zero)) {
      s21_decimal temp = div10_n(*value_2, 1, 0);
      // если после окургления уже value_2=0 то ставим флаг что больше округлять
      // нельзя иначе окугляем дальше*/
      if (decimal_is_zero(temp)) {
        flag2_zero = 1;
      } else {
        *value_2 = div10_n(*value_2, 1, 0);
        exp2 = get_exp(*value_2);
        o2 = get_overhead(*value_2);
      }  // если после округлений получается что числа все еще слишком малы то
         // дальнейшая потеря точности не имеет смысла и обунляем одно из чисел
    } else if (flag1_zero && flag2_zero) {
      s21_init_decimal(value_1);
    }
    overhead = o1 + o2;
  }
  return overhead;
}
