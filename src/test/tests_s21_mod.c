#include "../tests.h"

START_TEST(mod_1_test) {
  s21_decimal num1 = {{0, 0, 0, 0}};
  s21_decimal num2 = {{2, 0, 0, 0}};
  s21_decimal res = {{0, 0, 0, 0}};
  int status = s21_mod(num1, num2, &res);
  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(res.bits[0], 0);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0);
}
END_TEST

START_TEST(mod_2_test) {
  s21_decimal num1 = {{2, 0, 0, 0}};
  s21_decimal num2 = {{0, 0, 0, 0}};
  s21_decimal res = {{0, 0, 0, 0}};
  int status = s21_mod(num1, num2, &res);
  ck_assert_int_eq(status, 3);
  ck_assert_int_eq(res.bits[0], 0);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0);
}
END_TEST

START_TEST(mod_3_test) {
  s21_decimal num1 = {{134, 0, 0, 0}};
  s21_decimal num2 = {{0, 12, 0, 0}};
  s21_decimal res = {{0, 0, 0, 0}};
  int status = s21_mod(num1, num2, &res);
  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(res.bits[0], 134);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0);
}
END_TEST

START_TEST(mod_4_test) {
  s21_decimal num1 = {{256, 0, 0, -2147483648}};
  s21_decimal num2 = {{0, 0, 10, 0}};
  s21_decimal res = {{0, 0, 0, 0}};
  int status = s21_mod(num1, num2, &res);
  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(res.bits[0], 256);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], -2147483648);
}
END_TEST

START_TEST(mod_5_test) {
  s21_decimal num1 = {{25, 0, 0, 1638400}};
  s21_decimal num2 = {{345, 0, 0, -2147352576}};
  s21_decimal res = {{0, 0, 0, 0}};
  int status = s21_mod(num1, num2, &res);
  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(res.bits[0], 25);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 1638400);
}
END_TEST

START_TEST(mod_6_test) {
  s21_decimal num1 = {{25, 0, 0, 65536}};
  s21_decimal num2 = {{345, 0, 0, -2147352576}};
  s21_decimal res = {{0, 0, 0, 0}};
  int status = s21_mod(num1, num2, &res);
  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(res.bits[0], 25);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 65536);
}
END_TEST

START_TEST(mod_7_test) {
  s21_decimal num1 = {{1, 245, 0, -2147418112}};
  s21_decimal num2 = {{0, 0, 456, -2147287040}};
  s21_decimal res = {{0, 0, 0, 0}};
  int status = s21_mod(num1, num2, &res);
  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(res.bits[0], 1);
  ck_assert_int_eq(res.bits[1], 245);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], -2147418112);
}
END_TEST

START_TEST(mod_8_test) {
  s21_decimal num1 = {{136, 0, 0, 0}};
  s21_decimal num2 = {{5, 0, 0, 0}};
  s21_decimal res = {{0, 0, 0, 0}};
  int status = s21_mod(num1, num2, &res);
  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(res.bits[0], 1);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0);
}
END_TEST

START_TEST(mod_9_test) {
  s21_decimal num1 = {{137, 0, 0, 0}};
  s21_decimal num2 = {{5, 0, 0, -2147483648}};
  s21_decimal res = {{0, 0, 0, 0}};
  int status = s21_mod(num1, num2, &res);
  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(res.bits[0], 2);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0);
}
END_TEST

START_TEST(mod_10_test) {
  s21_decimal num1 = {{138, 0, 0, -2147483648}};
  s21_decimal num2 = {{5, 0, 0, 0}};
  s21_decimal res = {{0, 0, 0, 0}};
  int status = s21_mod(num1, num2, &res);
  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(res.bits[0], 3);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], -2147483648);
}
END_TEST

START_TEST(mod_11_test) {
  s21_decimal num1 = {{0, 139, 0, -2147483648}};
  s21_decimal num2 = {{0, 5, 0, -2147483648}};
  s21_decimal res = {{0, 0, 0, 0}};
  int status = s21_mod(num1, num2, &res);
  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(res.bits[0], 0);
  ck_assert_int_eq(res.bits[1], 4);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], -2147483648);
}
END_TEST

START_TEST(mod_12_test) {
  s21_decimal num1 = {{200000, 0, 0, -2147287040}};
  s21_decimal num2 = {{2, 5, 100, 0}};
  s21_decimal res = {{0, 0, 0, 0}};
  int status = s21_mod(num1, num2, &res);
  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(res.bits[0], 200);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], -2147483648);
}
END_TEST

START_TEST(mod_13_test) {
  s21_decimal num1 = {{22, 0, 0, 983040}};
  s21_decimal num2 = {{3, 0, 0, 1179648}};
  s21_decimal res = {{0, 0, 0, 0}};
  int status = s21_mod(num1, num2, &res);
  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(res.bits[0], 1);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 1179648);
}
END_TEST

START_TEST(mod_14_test) {
  s21_decimal num1 = {{1241513984, 370409800, 542101, 0}};
  s21_decimal num2 = {{3, 0, 0, 1179648}};
  s21_decimal res = {{0, 0, 0, 0}};
  int status = s21_mod(num1, num2, &res);
  ck_assert_int_eq(status, 2);
  ck_assert_int_eq(res.bits[0], 0);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0);
}
END_TEST

START_TEST(mod_error) {
  s21_decimal src1 = {{0, 0, 0, 0}};
  s21_decimal src2 = {{0, 0, 0, 0}};
  s21_decimal *result_our = NULL;
  int val = s21_mod(src1, src2, result_our);
  ck_assert_int_eq(val, S21_ERROR);
}
END_TEST

TCase *tests_s21_mod(void) {
  TCase *tcase = tcase_create(TCASE_NAME);

  tcase_add_test(tcase, mod_1_test);
  tcase_add_test(tcase, mod_2_test);
  tcase_add_test(tcase, mod_3_test);
  tcase_add_test(tcase, mod_4_test);
  tcase_add_test(tcase, mod_5_test);
  tcase_add_test(tcase, mod_6_test);
  tcase_add_test(tcase, mod_7_test);
  tcase_add_test(tcase, mod_8_test);
  tcase_add_test(tcase, mod_9_test);
  tcase_add_test(tcase, mod_10_test);
  tcase_add_test(tcase, mod_11_test);
  tcase_add_test(tcase, mod_12_test);
  tcase_add_test(tcase, mod_13_test);
  tcase_add_test(tcase, mod_14_test);
  tcase_add_test(tcase, mod_error);

  return tcase;
}
