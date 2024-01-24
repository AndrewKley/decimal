// Copyright [2023] arianneg
#include "s21_decimal.h"

void s21_init_decimal(s21_decimal *value) {
  if (value) {
    value->bits[0] = value->bits[1] = value->bits[2] = value->bits[3] = 0;
  }
}

int get_sign(s21_decimal num) {
  alt_decimal tmp = {num};
  return tmp.decimal.sign;
}

int get_exp(s21_decimal num) {
  alt_decimal tmp = {num};
  return tmp.decimal.exp;
}

unsigned int s21_get_bit(s21_decimal value, unsigned int bit) {
  unsigned mask = 1 << (bit % 32);
  return ((value.bits[bit / 32] & mask) == mask);
}

void s21_set_bit(s21_decimal *value, int bit) {
  value->bits[bit / 32] |= 1 << (bit % 32);
}

void s21_clear_bit(s21_decimal *value, int bit) {
  value->bits[bit / 32] &= ~(1 << (bit % 32));
}

s21_decimal set_exp(const s21_decimal num, unsigned exp) {
  alt_decimal fields = {num};
  // exponent must be less that 29
  fields.decimal.exp = exp % 29;
  return fields.num;
}

void s21_set_negative_bit(s21_decimal *value) { value->bits[3] |= 0x80000000; }

unsigned int s21_get_scale(s21_decimal value) {
  return value.bits[3] << 8 >> 24;
}

void s21_set_scale(s21_decimal *value, unsigned int scale) {
  value->bits[3] = (value->bits[3] & 0x80000000) | (scale << 16);
}

void s21_get_float_part(char *part, int *scale, int *length,
                        int *decimal_part) {
  char *dot = strchr(part, '.');
  char *exponenta = strchr(part, 'e');
  *scale = 0;
  *decimal_part = 0;
  *length = 0;

  if (exponenta) {
    *exponenta++ = '\0';
    *scale = atoi(exponenta);
  }
  if (dot) {
    *dot++ = '\0';
    *length = strlen(dot);
    *decimal_part = atoi(dot);
  }
}

int normalize(s21_decimal *value_1, s21_decimal *value_2) {
  int status = 0;
  int exp1 = get_exp(*value_1), exp2 = get_exp(*value_2);
  if (exp1 < exp2) {
    *value_1 = max_exp10(*value_1, exp2 - exp1);  // shift value_1 left
    exp1 = get_exp(*value_1);
    if (exp2 != exp1) {
      status = 1;  // shift value_2 right with lost accuracy
      *value_2 = div10_n(*value_2, exp2 - exp1, 1);
    }
  } else if (exp1 > exp2) {
    *value_2 = max_exp10(*value_2, exp1 - exp2);  // shift value_2 left
    exp2 = get_exp(*value_2);
    if (exp1 != exp2) {
      status = 1;  // shift value_1 right with lost accuracy
      *value_1 = div10_n(*value_1, exp1 - exp2, 1);
    }
  }
  return status;
}

s21_decimal max_exp10(s21_decimal num, int exp) {
  s21_decimal result = num;
  int check = 1;
  for (int i = 0; check && i < exp; i++) {
    num = exp10_mantissa(num);
    if (!(num.bits[0] == -1 && num.bits[1] == -1 && num.bits[2] == -1))
      result = num;
    else
      check = 0;
  }
  return result;
}

s21_decimal exp10_mantissa(s21_decimal num) {
  s21_decimal sum = {{0, 0, 0, 0}};
  int status = s21_add(shift(num, 1, 1), shift(num, 1, 3), &sum);

  if (status) {
    for (int i = 0; i < BITS - 1; i++) sum.bits[i] = -1;
  } else {
    sum = set_exp(sum, get_exp(num) + 1);
  }

  return sum;
}

s21_decimal div10(s21_decimal num, int *rem) {
  s21_decimal res, remains;
  s21_decimal one = {{1, 0, 0, 0}};
  res = sum_mantissas(shift(num, 0, 1), shift(num, 0, 2));
  for (int i = 4; i < 65; i *= 2) {
    res = sum_mantissas(res, shift(res, 0, i));
  }
  res = shift(res, 0, 3);
  s21_decimal res10 = sum_mantissas(shift(res, 1, 3), shift(res, 1, 1));
  int exp = get_exp(num);
  res10 = set_exp(res10, exp);
  if (get_sign(num)) res10 = invert_sign(res10);
  s21_sub(num, res10, &remains);
  unsigned int r = remains.bits[0];
  if (r > 9) s21_add(res, one, &res);
  *rem = r > 9 ? r - 10 : r;           // остаток от деления
  if (exp) res = set_exp(res, --exp);  // увеличиваем экспонету
  if (get_sign(num)) res = invert_sign(res);  // set orig sign
  return res;
}

s21_decimal div10_n(s21_decimal num, int count, int round) {
  int rem = 0;
  s21_decimal one = {{1, 0, 0, 0}};
  for (; count > 0; count--) num = div10(num, &rem);
  if (round && rem > 4) {
    int exp = get_exp(num);
    int sign = get_sign(num);
    num = sum_mantissas(num, one);
    num = set_exp(num, exp);
    if (sign) num = invert_sign(num);
  }
  return num;
}

s21_decimal invert_sign(s21_decimal num) {
  alt_decimal fields = {num};
  InvBit(fields.decimal.sign, 0);
  return fields.num;
}

s21_decimal shift_left(s21_decimal num) {
  for (int i = BITS - 2; 0 <= i; --i) {
    num.bits[i] = (unsigned)num.bits[i] << 1;
    if (0 < i && BitIsSet((unsigned)num.bits[i - 1], 31))
      SetBit((num.bits[i]), 0);
  }
  return num;
}

s21_decimal shift_right(s21_decimal num) {
  for (int i = 0; i < (BITS - 1); i++) {
    num.bits[i] = (unsigned)num.bits[i] >> 1;
    if (i < (BITS - 2) && BitIsSet((unsigned)num.bits[i + 1], 0))
      SetBit((num.bits[i]), 31);
  }
  return num;
}

s21_decimal shift(s21_decimal num, char direcion, unsigned char count) {
  if (count > RANK_DEC) count = RANK_DEC;
  while (count && count--) {
    if (direcion) {
      num = shift_left(num);
    } else {
      num = shift_right(num);
    }
  }
  return num;
}

s21_decimal _mod(s21_decimal num) {
  return get_sign(num) ? invert_sign(num) : num;
}

s21_decimal _sub_mantissas(s21_decimal num) {
  int sign = get_sign(num);
  int exp = get_exp(num);
  s21_decimal one = {{1, 0, 0, 0}};
  for (int i = 0; i < BITS - 1; i++) num.bits[i] = ~num.bits[i];
  num = sum_mantissas(num, one);  // it returns bit[3]=0
  if (sign) num = invert_sign(num);
  num = set_exp(num, exp);
  return num;
}

s21_decimal sum_mantissas(s21_decimal value_1, s21_decimal value_2) {
  s21_decimal sum = {{0, 0, 0, 0}};
  char overhead = 0;
  for (char i = 0; i < RANK_DEC; i++) {
    char bit1 = s21_get_bit(value_1, i);
    char bit2 = s21_get_bit(value_2, i);
    char d = (bit1 ^ bit2) ^ overhead;
    if (d) {
      s21_set_bit(&sum, i);
    } else {
      s21_clear_bit(&sum, i);
    }
    overhead = (overhead && !d) ? 1 : bit1 & bit2;
  }
  return sum;
}

int get_overhead(s21_decimal num) {
  int i = 0, bit = RANK_DEC;
  for (; !s21_get_bit(num, bit - 1) && i < bit; i++) num = shift_left(num);
  return i + 1;  // make an exactly overhead
}

int decimal_is_zero(s21_decimal num) {
  return (!num.bits[0] && !num.bits[1] && !num.bits[2]);
}

s21_decimal clear_prop(s21_decimal num) {
  num = set_exp(num, 0);
  if (get_sign(num)) num = invert_sign(num);
  return num;
}

s21_decimal remove_zero(s21_decimal num) {
  int remains = 0;
  int check = 1;
  while (check && get_exp(num)) {
    s21_decimal temp = div10(num, &remains);
    if (!remains)
      num = temp;
    else
      check = 0;
  }
  return num;
}

int check_neg_zero(s21_decimal val) {
  int res = 0, one_exist = 0;
  for (int i = 0; i <= 95; i++) {
    if (s21_get_bit(val, i) == 1) one_exist = 1;
  }
  if (one_exist == 0 && get_sign(val) == 1) res = 1;
  return res;
}
