#include "../s21_decimal.h"

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int status = S21_OK;
  unsigned int scale = s21_get_scale(src);

  if (!dst || 28 < scale) {
    status = S21_ERROR;
  } else {
    *dst = 0.f;
    for (int i = 0; i < 96; ++i) {
      if (s21_get_bit(src, i)) {
        *dst += powf(2, i);
      }
    }
    *dst /= powf(10, scale);
    if (s21_get_bit(src, 127)) {
      *dst *= -1;
    }
  }

  return status;
}
