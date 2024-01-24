// Copyright [2023] arianneg
#ifndef SRC_S21_DECIMAL_H_
#define SRC_S21_DECIMAL_H_

#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define S21_OK 0
#define S21_ERROR 1
#define S21_ERROR_TOO_BIG 1
#define S21_ERROR_TOO_SMALL 2
#define S21_ERROR_ZERO_DIVISION 3
#define S21_FALSE 0
#define S21_TRUE 1
#define S21_MINUS 0b10000000000000000000000000000000
#define S21_PLUS 0b00000000000000000000000000000000

typedef struct {
  int bits[4];
} s21_decimal;

typedef struct {
  int bits[7];
} long_decimal;

typedef union {
  s21_decimal num;
  struct {
    unsigned low : 32;
    unsigned middle : 32;
    unsigned high : 32;
    unsigned : 16;
    unsigned exp : 8;
    unsigned : 7;
    unsigned sign : 1;
  } decimal;
} alt_decimal;

#define BITS 4
#define RANK_DEC ((BITS - 1) * 32)
// #define MAX_EXP ((int)(DEC_SIZE / 3.4))

#define SetBit(reg, position) reg |= ((unsigned)1 << position)
#define ClearBit(reg, position) reg &= (~((unsigned)1 << position))
#define InvBit(reg, position) reg ^= ((unsigned)1 << position)
#define BitIsSet(reg, position) ((reg & ((unsigned)1 << position)) != 0)
// #define BitIsClear(reg, position) ((reg & (1 << position)) == 0)

// helper functions (arianneg)
// void s21_get_bit_pattern(s21_decimal value);
void s21_init_decimal(s21_decimal *value);
unsigned int s21_get_bit(s21_decimal value, unsigned int bit);
void s21_set_bit(s21_decimal *value, int bit);
void s21_clear_bit(s21_decimal *value, int bit);
void s21_set_negative_bit(s21_decimal *value);
unsigned int s21_get_scale(s21_decimal value);
void s21_set_scale(s21_decimal *value, unsigned int scale);
void s21_get_float_part(char *part, int *scale, int *length, int *decimal_part);

// helper functions (jupitere)
int get_exp(s21_decimal num);
int get_sign(s21_decimal num);
// s21_decimal change_sign(s21_decimal num);
// void clear_decimal(s21_decimal *num); // s21_init_decimal
s21_decimal sum_mantissas(s21_decimal value_1, s21_decimal value_2);
int normalize(s21_decimal *value_1, s21_decimal *value_2);
// exponent decimal to max possible
s21_decimal max_exp10(s21_decimal num, int exp);
// multiple mantissa by 10 like 2*10=20
s21_decimal exp10_mantissa(s21_decimal num);
s21_decimal div10(s21_decimal num, int *rem);
s21_decimal div10_n(s21_decimal num, int count, int round);
s21_decimal invert_sign(s21_decimal num);
// one - left shift, zero - right shift
s21_decimal shift(s21_decimal num, char direcion, unsigned char count);
s21_decimal shift_left(s21_decimal num);
s21_decimal shift_right(s21_decimal num);
s21_decimal set_exp(s21_decimal num, unsigned exp);
void change_bit(s21_decimal *num, int position, char value);
s21_decimal module_of_decimal(s21_decimal num);
// invert mantisa bits into a negative state
s21_decimal _sub_mantissas(s21_decimal num);
int decimal_is_zero(s21_decimal num);
int check_neg_zero(s21_decimal val);
int check_status(s21_decimal value_1, s21_decimal value_2, s21_decimal result,
                 int status);
long_decimal conversion_dec(s21_decimal num);
//
int decimal_is_zero(s21_decimal num);
s21_decimal clear_prop(s21_decimal num);
s21_decimal remove_zero(s21_decimal num);
int _round(s21_decimal *value_1, s21_decimal *value_2);
//  get max of bits of mantisa part to achive the overhead
int get_overhead(s21_decimal num);
int _common_div(s21_decimal num1, s21_decimal num2, s21_decimal *res,
                s21_decimal *remainder);
// return rounded fractional remainder
s21_decimal _round_remainder(s21_decimal num1, s21_decimal num2,
                             s21_decimal res);
// return module of decimal
s21_decimal _mod(s21_decimal num);
/**
 * @brief get fractional part over divide using decimal column divide
 * @return int status of  divide (0 - ok, 1 - wrong param, 3 - divide by zero)
 */
int _find_part_dec(s21_decimal *n1, s21_decimal n2, s21_decimal *integer_result,
                   int *exp);
// return rounded fractional remainder
s21_decimal _round_remainder(s21_decimal num1, s21_decimal num2,
                             s21_decimal res);
int post_div(s21_decimal n1, s21_decimal *res, int status, int neg, int exp);
int _normalize_for_div(s21_decimal num1, s21_decimal num2, s21_decimal *result);

// jupitere
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

// hodorsta
int s21_is_less(s21_decimal value_1, s21_decimal value_2);
int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_greater(s21_decimal value_1, s21_decimal value_2);
int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2);

// arianneg
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

// exeggcur
int s21_negate(s21_decimal value, s21_decimal *result);
int s21_floor(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);

#endif  // SRC_S21_DECIMAL_H_
