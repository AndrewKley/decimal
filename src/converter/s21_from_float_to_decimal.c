#include "../s21_decimal.h"

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int status = S21_OK;

  if (dst) {
    s21_init_decimal(dst);
  } else {
    status = S21_ERROR;
  }

  if (status || (0 < fabsf(src) && fabsf(src) < 1e-28) || isnan(src) ||
      isinf(src) || 79228162514264337593543950335.f < src) {
    status = S21_ERROR;
  } else {
    char part[128] = {0};
    int scale = 0;
    int decimal_part = 0;
    int length = 0;

    sprintf(part, "%.7g", src);
    s21_get_float_part(part, &scale, &length, &decimal_part);

    if (scale < 0 && 28 < abs(scale) + length) {
      memset(part, 0, 128);
      sprintf(part, "%.*g", 29 - abs(scale), src);
      s21_get_float_part(part, &scale, &length, &decimal_part);
    }

    int number = abs(atoi(part));
    for (int i = 0; i < length; ++i) {
      number *= 10;
    }
    number += decimal_part;

    if (src < 0) {
      s21_set_negative_bit(dst);
    }
    if (scale < 0) {
      s21_set_scale(dst, abs(scale) + length);
    } else if (!scale) {
      s21_set_scale(dst, length);
    }

    for (int i = 0; number; ++i, number /= 2) {
      if (number % 2) {
        s21_set_bit(dst, i);
      }
    }

    // original
    if (0 < scale - length) {
      s21_decimal s21_mul_by_10 = {{10, 0, 0, 0}};
      for (int i = 0; !status && i < scale - length; ++i) {
        status = s21_mul(*dst, s21_mul_by_10, dst);
      }
    }

    // temporary option to add 10 times
    // if (0 < scale - length) {
    //   for (int i = 0; !status && i < scale - length; ++i) {
    //     s21_decimal s21_summary_by_10 = {{
    //         0,
    //         0,
    //         0,
    //         0,
    //     }};
    //     for (int j = 0; !status && j < 10; ++j) {
    //       status = s21_add(s21_summary_by_10, *dst, &s21_summary_by_10);
    //     }
    //     memcpy(dst, &s21_summary_by_10, sizeof(s21_summary_by_10));
    //   }
    // }
  }

  return status;
}
