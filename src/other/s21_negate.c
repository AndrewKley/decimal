#include "../s21_decimal.h"

int s21_negate(s21_decimal value, s21_decimal *result) {
  int status = S21_OK;

  if (result) {
    s21_init_decimal(result);
    memcpy(result, &value, sizeof(value));
    s21_get_bit(value, 127) ? s21_clear_bit(result, 127)
                            : s21_set_negative_bit(result);
  } else {
    status = S21_ERROR;
  }

  return status;
}
