#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#include "s21_decimal.h"

void run_test(Suite *thesuit) {
  SRunner *therunner = srunner_create(thesuit);

  srunner_set_fork_status(therunner, CK_NOFORK);
  srunner_run_all(therunner, CK_NORMAL);
  srunner_ntests_failed(therunner);
  srunner_free(therunner);

  printf("\n");
}

// ARITHMETIC

// add

START_TEST(part1_add_normal) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  s21_decimal test = {{1535}};
  value_1.bits[0] = 150;
  value_2.bits[0] = 35;
  setPower(&value_2, 1);
  int res = s21_add(value_1, value_2, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_int_eq(getPower(result), 1);
  ck_assert_int_eq(getSign(result), 0);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(part1_add_both_neg) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  s21_decimal test = {{1535}};
  value_1.bits[0] = 150;
  value_2.bits[0] = 35;
  setSign(&value_2, 1);
  setSign(&value_1, 1);
  setPower(&value_2, 1);
  int res = s21_add(value_1, value_2, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_int_eq(getPower(result), 1);
  ck_assert_int_eq(getSign(result), 1);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(part1_add_error) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  value_1.bits[0] = 4294967295;
  value_1.bits[1] = 4294967295;
  value_1.bits[2] = 4294967295;
  value_2.bits[0] = 35;
  int res = s21_add(value_1, value_2, &result);
  ck_assert_int_eq(res, 1);
  setSign(&value_2, 1);
  setSign(&value_1, 1);

  memset(&result, 0, sizeof(s21_decimal));
  res = s21_add(value_1, value_2, &result);
  ck_assert_int_eq(res, 2);
}
END_TEST

START_TEST(part1_add_one_greater) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  value_1.bits[0] = 540;
  value_2.bits[0] = 560;
  s21_decimal test = {{20}};
  setSign(&value_2, 1);
  s21_add(value_1, value_2, &result);
  ck_assert_int_eq(result.bits[0], test.bits[0]);
  ck_assert_int_eq(getSign(result), 1);
}
END_TEST

START_TEST(part1_add_one_sign) {
  s21_decimal value_1 = {{150}};
  s21_decimal value_2 = {{50}};
  s21_decimal result = {0};
  s21_decimal test = {{100}};
  setSign(&value_1, 1);
  s21_add(value_1, value_2, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_int_eq(getPower(result), 0);
  ck_assert_int_eq(getSign(result), 1);
  setSign(&value_1, 0);
  setSign(&value_2, 1);
  memset(&result, 0, sizeof(s21_decimal));
  int res = s21_add(value_1, value_2, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_int_eq(getPower(result), 0);
  ck_assert_int_eq(getSign(result), 0);
  ck_assert_int_eq(res, 0);
}
END_TEST

// sub

START_TEST(part1_sub_normal) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};

  value_1.bits[0] = 155;
  value_2.bits[0] = 55;
  s21_sub(value_1, value_2, &result);
  s21_decimal test = {0};
  test.bits[0] = 100;
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  memset(&result, 0, sizeof(s21_decimal));
  value_1.bits[0] = (unsigned int)50;
  value_2.bits[0] = (unsigned int)150;
  s21_sub(value_1, value_2, &result);
  int sign = getSign(result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_int_eq(sign, 1);

  memset(&value_1, 0, sizeof(s21_decimal));
  memset(&value_2, 0, sizeof(s21_decimal));
  memset(&result, 0, sizeof(s21_decimal));
  value_1.bits[0] = (unsigned int)50;
  value_2.bits[0] = (unsigned int)15;
  memset(&result, 0, sizeof(s21_decimal));
  setPower(&value_2, 1);
  s21_sub(value_1, value_2, &result);
  ck_assert_uint_eq(result.bits[0], 485);
  ck_assert_uint_eq(getPower(result), 1);
  ck_assert_uint_eq(getSign(result), 0);
}
END_TEST

START_TEST(part1_sub_inf) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  value_1.bits[0] = 4294967295;
  value_1.bits[1] = 4294967295;
  value_1.bits[2] = 4294967295;
  value_2.bits[0] = 55;
  setSign(&value_1, 1);
  int res = s21_sub(value_1, value_2, &result);
  ck_assert_uint_eq(res, 2);
}
END_TEST

START_TEST(part1_sub_one_sign) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  value_1.bits[0] = 150;
  value_2.bits[0] = 50;
  setSign(&value_1, 1);
  s21_sub(value_1, value_2, &result);
  s21_decimal test = {0};
  test.bits[0] = 200;
  ck_assert_int_eq(result.bits[0], test.bits[0]);
  int sign = getSign(result);
  ck_assert_int_eq(sign, 1);
  memset(&result, 0, sizeof(s21_decimal));
  setSign(&value_1, 0);
  setSign(&value_2, 1);
  s21_sub(value_1, value_2, &result);
  ck_assert_int_eq(result.bits[0], test.bits[0]);
  int sign1 = getSign(result);
  ck_assert_int_eq(sign1, 0);
}
END_TEST

START_TEST(part1_sub_two_sign) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  value_1.bits[0] = 150;
  value_2.bits[0] = 50;
  setSign(&value_1, 1);
  setSign(&value_2, 1);

  s21_sub(value_1, value_2, &result);
  s21_decimal test = {0};
  test.bits[0] = 100;
  ck_assert_int_eq(result.bits[0], test.bits[0]);
  int sign = getSign(result);
  ck_assert_int_eq(sign, 1);
  memset(&result, 0, sizeof(s21_decimal));
  s21_sub(value_1, value_2, &result);
  ck_assert_int_eq(result.bits[0], test.bits[0]);
  int sign1 = getSign(result);
  ck_assert_int_eq(sign1, 1);
  value_1.bits[0] = 50;
  value_2.bits[0] = 150;
  memset(&result, 0, sizeof(s21_decimal));
  s21_sub(value_1, value_2, &result);
  ck_assert_int_eq(result.bits[0], test.bits[0]);
  sign1 = getSign(result);
  ck_assert_int_eq(sign1, 0);
}
END_TEST

// mult

START_TEST(part1_mult_value_1_is_one) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  s21_decimal test = {0};
  value_1.bits[0] = 1;
  value_2.bits[0] = 10;
  test.bits[0] = 10;
  s21_mul(value_1, value_2, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  setSign(&value_1, 1);
  memset(&result, 0, sizeof(s21_decimal));
  s21_mul(value_1, value_2, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_uint_eq(getSign(result), 1);
}
END_TEST

START_TEST(part1_mult_value_2_is_one) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  s21_decimal test = {0};
  value_1.bits[0] = 15;
  value_2.bits[0] = 1;
  test.bits[0] = 15;
  s21_mul(value_1, value_2, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
}
END_TEST

START_TEST(part1_mult_value_1_is_zero) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  s21_decimal test = {0};
  value_1.bits[0] = 0;
  value_2.bits[0] = 100;
  test.bits[0] = 0;
  s21_mul(value_1, value_2, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
}
END_TEST

START_TEST(part1_mult_value_2_is_zero) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  s21_decimal test = {0};
  value_1.bits[0] = 7;
  value_2.bits[0] = 0;
  test.bits[0] = 0;
  s21_mul(value_1, value_2, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
}
END_TEST

START_TEST(part1_mult_max) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  value_1.bits[0] = 4294967295;
  value_1.bits[1] = 4294967295;
  value_1.bits[2] = 4294967295;
  value_2.bits[0] = 5;
  int res = s21_mul(value_1, value_2, &result);
  ck_assert_uint_eq(res, 1);

  setSign(&value_2, 1);
  res = s21_mul(value_1, value_2, &result);
  ck_assert_uint_eq(res, 2);
}
END_TEST

START_TEST(part1_mult_big_power) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  s21_decimal test = {0};
  value_1.bits[0] = 2147483647;
  value_2.bits[0] = 255;
  test.bits[0] = 2147483393;
  test.bits[1] = 127;
  setPower(&value_2, 10);
  int res = s21_mul(value_1, value_2, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_uint_eq(result.bits[1], test.bits[1]);
  ck_assert_uint_eq(getPower(result), 10);
  ck_assert_uint_eq(getSign(result), 0);
  ck_assert_uint_eq(res, 0);
}
END_TEST

START_TEST(part1_mult_big_negative_over) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  value_1.bits[0] = 2147483647;
  value_1.bits[1] = 2147483647;
  value_1.bits[2] = 2147483647;
  value_2.bits[0] = 15;
  setPower(&value_2, 1);
  setSign(&value_2, 1);
  int res = s21_mul(value_1, value_2, &result);
  ck_assert_uint_eq(res, 2);
}
END_TEST

START_TEST(part1_mult_big_over) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  value_1.bits[0] = 2147483647;
  value_1.bits[1] = 2147483647;
  value_1.bits[2] = 2147483647;
  value_2.bits[0] = 15;
  setPower(&value_2, 1);
  int res = s21_mul(value_1, value_2, &result);
  ck_assert_uint_eq(res, 1);
}
END_TEST

START_TEST(part1_mult_value_1_is_minus) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  s21_decimal test = {0};
  setSign(&value_1, 1);
  value_1.bits[0] = 15;
  value_2.bits[0] = 1;
  test.bits[0] = 15;
  s21_mul(value_1, value_2, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_uint_eq(getSign(result), 1);
}
END_TEST

START_TEST(part1_mult_value_2_is_minus) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  s21_decimal test = {0};
  value_1.bits[0] = 15;
  setSign(&value_2, 1);
  value_2.bits[0] = 1;
  test.bits[0] = 15;
  s21_mul(value_1, value_2, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_uint_eq(getSign(result), 1);
}
END_TEST

START_TEST(part1_mult_value_1_is_zero_minus) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  s21_decimal test = {0};
  setSign(&value_1, 1);
  value_1.bits[0] = 0;
  value_2.bits[0] = 100;
  test.bits[0] = 0;
  s21_mul(value_1, value_2, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_uint_eq(getSign(result), 1);
}
END_TEST

START_TEST(part1_mult_value_2_is_zero_minus) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  s21_decimal test = {0};
  value_1.bits[0] = 7;
  setSign(&value_2, 1);
  value_2.bits[0] = 0;
  test.bits[0] = 0;
  s21_mul(value_1, value_2, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_uint_eq(getSign(result), 1);
}
END_TEST

START_TEST(part1_mult_value_1_2_is_minus) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  s21_decimal test = {0};
  setSign(&value_1, 1);
  setSign(&value_2, 1);
  value_1.bits[0] = 0;
  value_2.bits[0] = 100;
  test.bits[0] = 0;
  s21_mul(value_1, value_2, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_uint_eq(getSign(result), 0);
  setPower(&value_1, 5);
  setPower(&value_2, 29);
  s21_mul(value_1, value_2, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
}
END_TEST

// // div

START_TEST(part1_div_zero) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  s21_decimal test = {0};
  value_1.bits[0] = 150;
  value_2.bits[0] = 0;
  test.bits[0] = 0;
  int res = s21_div(value_1, value_2, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_uint_eq(getSign(result), 0);
  ck_assert_uint_eq(res, 1);

  setSign(&value_1, 1);
  res = s21_div(value_1, value_2, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_uint_eq(getSign(result), 0);
  ck_assert_uint_eq(res, 2);
}
END_TEST

START_TEST(part2_div) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  s21_decimal test = {0};
  setSign(&value_1, 1);
  setSign(&value_2, 1);
  value_1.bits[0] = 0;
  value_2.bits[0] = 0;
  test.bits[0] = 0;
  int res = s21_div(value_1, value_2, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_uint_eq(getSign(result), 0);
  ck_assert_uint_eq(res, 3);
}
END_TEST

START_TEST(part3_div) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  s21_decimal test = {0};
  setSign(&value_1, 1);
  setSign(&value_2, 1);
  value_1.bits[0] = 1000;
  value_2.bits[0] = 100;
  test.bits[0] = 10;
  int res = s21_div(value_1, value_2, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_uint_eq(getSign(result), 0);
  ck_assert_uint_eq(res, 0);
}
END_TEST

START_TEST(part4_div) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  s21_decimal result = {0};
  s21_decimal test = {0};
  setSign(&value_1, 0);
  setSign(&value_2, 0);
  setPower(&value_1, 0);
  setPower(&value_2, 3);
  value_1.bits[0] = 4;
  value_2.bits[0] = 4;
  test.bits[0] = 1000;
  int res = s21_div(value_1, value_2, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_uint_eq(getSign(result), 0);
  ck_assert_uint_eq(getPower(result), 0);
  ck_assert_uint_eq(res, 0);
}
END_TEST

// COMPARISON

// less

START_TEST(part2_less_null) {
  s21_decimal value_1 = {{0, 0, 0, 0}};
  s21_decimal value_2 = {{0, 0, 0, 0}};
  ck_assert_int_eq(s21_is_less(value_1, value_2), FALSE);
  setSign(&value_2, 1);
  ck_assert_int_eq(s21_is_less(value_1, value_2), FALSE);
}
END_TEST

START_TEST(part2_less_normal) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  for (int i = 0; i < 50; i++) {
    for (int j = 2; j < 52; j++) {
      for (int i = 0, j = 2; i < 50 || j < 51; i++, j++) {
        value_1.bits[0] = i;
        value_2.bits[0] = j;
        ck_assert_int_eq(s21_is_less(value_1, value_2), TRUE);
        ck_assert_int_eq(s21_is_less(value_2, value_1), FALSE);
      }
    }
  }
}
END_TEST

START_TEST(part2_less_negative) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  value_1.bits[0] = 20;
  value_2.bits[0] = 5;
  setSign(&value_1, 1);
  ck_assert_int_eq(s21_is_less(value_1, value_2), TRUE);

  setSign(&value_1, 0);
  setSign(&value_2, 1);

  for (int i = 50; i < 0; i++) {
    for (int j = 48; j < 2; j++) {
      value_1.bits[0] = i;
      value_2.bits[0] = j;
      ck_assert_int_eq(s21_is_less(value_1, value_2), TRUE);
      ck_assert_int_eq(s21_is_less(value_2, value_1), FALSE);
    }
  }
}
END_TEST

START_TEST(part2_less_pow) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  value_1.bits[0] = 50;
  value_2.bits[0] = 30;
  setPower(&value_1, 8);
  setPower(&value_2, 8);
  ck_assert_int_eq(s21_is_less(value_1, value_2), FALSE);
  setPower(&value_2, 0);
  ck_assert_int_eq(s21_is_less(value_1, value_2), TRUE);
}
END_TEST

// less or equal

START_TEST(part2_less_or_equal_normal) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  for (int i = 0, j = 1; i < 51 || j < 52; i++, j++) {
    value_1.bits[0] = i;
    value_2.bits[0] = j;
    ck_assert_int_eq(s21_is_less_or_equal(value_1, value_2), TRUE);
  }
  for (int i = 0, j = 0; i < 52 || j < 52; i++, j++) {
    value_1.bits[0] = i;
    value_2.bits[0] = j;
    ck_assert_int_eq(s21_is_less_or_equal(value_1, value_2), TRUE);
  }
}
END_TEST

START_TEST(part2_less_or_equal_with_sign) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  value_1.bits[0] = 123450;
  value_2.bits[0] = 123450;

  setSign(&value_2, 1);
  ck_assert_int_eq(s21_is_less_or_equal(value_1, value_2), FALSE);

  setSign(&value_1, 1);
  ck_assert_int_eq(s21_is_less_or_equal(value_1, value_2), TRUE);

  setSign(&value_2, 0);
  ck_assert_int_eq(s21_is_less_or_equal(value_1, value_2), TRUE);
}
END_TEST

// greater

START_TEST(part2_greater_null) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  ck_assert_int_eq(s21_is_greater(value_1, value_2), FALSE);
  setSign(&value_2, 1);
  ck_assert_int_eq(s21_is_greater(value_1, value_2), FALSE);
}
END_TEST

START_TEST(part2_greater_normal) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  for (int i = 1, j = 2; i < 50 || j < 51; i++, j++) {
    value_1.bits[0] = i;
    value_2.bits[0] = j;
    ck_assert_int_eq(s21_is_greater(value_1, value_2), FALSE);
    ck_assert_int_eq(s21_is_greater(value_2, value_1), TRUE);
  }
}
END_TEST

START_TEST(part2_greater_negative) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  value_1.bits[0] = 20;
  value_2.bits[0] = 5;
  setSign(&value_1, 1);
  ck_assert_int_eq(s21_is_greater(value_1, value_2), FALSE);
  setSign(&value_1, 0);
  setSign(&value_2, 1);
  ck_assert_int_eq(s21_is_greater(value_1, value_2), TRUE);
}
END_TEST

START_TEST(part2_greater_two_negative) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  value_1.bits[0] = 20;
  value_2.bits[0] = 5;
  setSign(&value_1, 1);
  setSign(&value_2, 1);
  ck_assert_int_eq(s21_is_greater(value_1, value_2), FALSE);
}
END_TEST

START_TEST(part2_greater_pow) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  value_1.bits[0] = 50;
  value_2.bits[0] = 30;
  setPower(&value_1, 8);
  setPower(&value_2, 8);
  ck_assert_int_eq(s21_is_greater(value_1, value_2), TRUE);
  setPower(&value_2, 0);
  ck_assert_int_eq(s21_is_greater(value_1, value_2), FALSE);
}
END_TEST

// greater or equal

START_TEST(part2_greater_or_equal_normal) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  for (int i = 0, j = 1; i < 51 || j < 52; i++, j++) {
    value_1.bits[0] = i;
    value_2.bits[0] = j;
    ck_assert_int_eq(s21_is_greater_or_equal(value_1, value_2), FALSE);
  }
  for (int i = 0, j = 0; i < 52 || j < 52; i++, j++) {
    value_1.bits[0] = i;
    value_2.bits[0] = j;
    ck_assert_int_eq(s21_is_greater_or_equal(value_1, value_2), TRUE);
  }
}
END_TEST

START_TEST(part2_greater_or_equal_with_sign) {
  s21_decimal value_1 = {0};
  s21_decimal value_2 = {0};
  value_1.bits[0] = 123450;
  value_2.bits[0] = 123450;
  setSign(&value_2, 1);
  ck_assert_int_eq(s21_is_greater_or_equal(value_1, value_2), TRUE);
  setSign(&value_1, 1);
  ck_assert_int_eq(s21_is_greater_or_equal(value_1, value_2), TRUE);
  setSign(&value_2, 0);
  ck_assert_int_eq(s21_is_greater_or_equal(value_1, value_2), FALSE);
}
END_TEST

// equal

START_TEST(part2_equal_1) {
  s21_decimal a = {{56, 0}}, b = {{56, 0}};
  ck_assert_uint_eq(s21_is_equal(a, b), TRUE);
}
END_TEST

START_TEST(part2_equal_2) {
  s21_decimal a = {{0, 0, 0, 1}}, b = {{0, 0, 0, 2147483647}};
  ck_assert_uint_eq(s21_is_equal(a, b), TRUE);
}
END_TEST

START_TEST(part2_equal_3) {
  s21_decimal a = {{0}}, b = {{0}};
  ck_assert_uint_eq(s21_is_equal(a, b), TRUE);
}
END_TEST

START_TEST(part2_equal_4) {
  s21_decimal a = {{111, 111, 122, 1}}, b = {{111, 111, 122, 1}};
  ck_assert_uint_eq(s21_is_equal(a, b), TRUE);
}
END_TEST

START_TEST(part2_equal_5) {
  s21_decimal a = {{111, 11231, 12332, 12222}}, b = {{111, 111, 122, 1}};
  ck_assert_uint_eq(s21_is_equal(a, b), FALSE);
}
END_TEST

START_TEST(part2_equal_6) {
  s21_decimal a = {{111, 11, 12332, 1222222}},
              b = {{119981, 1100001, 122, 1000001}};
  ck_assert_uint_eq(s21_is_equal(a, b), FALSE);
}
END_TEST

START_TEST(part2_equal_7) {
  s21_decimal a = {{0}}, b = {{0}};
  a.bits[0] = 12;
  b.bits[0] = 120;
  setPower(&a, 1);
  setPower(&b, 2);
  ck_assert_uint_eq(s21_is_equal(a, b), TRUE);
}
END_TEST

// not equal

START_TEST(part2_notequal_1) {
  s21_decimal a = {{56, 0}}, b = {{56, 0}};
  ck_assert_uint_eq(s21_is_not_equal(a, b), FALSE);
}
END_TEST

START_TEST(part2_notequal_2) {
  s21_decimal a = {{0, 0, 0, 2147483647}}, b = {{0, 0, 0, 2147483647}};
  ck_assert_uint_eq(s21_is_not_equal(a, b), FALSE);
}
END_TEST

START_TEST(part2_notequal_3) {
  s21_decimal a = {{0}}, b = {{0}};
  ck_assert_uint_eq(s21_is_not_equal(a, b), FALSE);
}
END_TEST

START_TEST(part2_notequal_4) {
  s21_decimal a = {{111, 111, 122, 1}}, b = {{111, 111, 122, 1}};
  ck_assert_uint_eq(s21_is_not_equal(a, b), FALSE);
}
END_TEST

START_TEST(part2_notequal_5) {
  s21_decimal a = {{111, 11231, 12332, 12222}}, b = {{111, 111, 122, 1}};
  ck_assert_uint_eq(s21_is_not_equal(a, b), TRUE);
}
END_TEST

START_TEST(part2_notequal_6) {
  s21_decimal a = {{111, 11, 12332, 1222222}},
              b = {{119981, 1100001, 122, 1000001}};
  ck_assert_uint_eq(s21_is_not_equal(a, b), TRUE);
}
END_TEST

// CONVERT

// from_int_to_decimal
START_TEST(part1_conv_int) {
  int a = 123123;
  s21_decimal result = {0};
  s21_from_int_to_decimal(a, &result);
  ck_assert_uint_eq(result.bits[0], a);
  ck_assert_int_eq(getSign(result), 0);
  ck_assert_int_eq(getPower(result), 0);
  a = 2147483647;
  s21_from_int_to_decimal(a, &result);
  ck_assert_uint_eq(result.bits[0], a);
  ck_assert_int_eq(getSign(result), 0);
  ck_assert_int_eq(getPower(result), 0);
  a = -2147483648;
  s21_from_int_to_decimal(a, &result);
  ck_assert_uint_eq(result.bits[0], abs(a));
  ck_assert_int_eq(getSign(result), 1);
  ck_assert_int_eq(getPower(result), 0);

  a = 0;
  s21_decimal temp = {0};
  temp.bits[0] = 1;
  setSign(&temp, 1);
  s21_from_decimal_to_int(temp, &a);
  ck_assert_int_eq(a, -1);

  a = 0;
  init(&temp);
  temp.bits[0] = 1U;
  temp.bits[3] = 0;
  s21_from_decimal_to_int(temp, &a);
  ck_assert_int_eq(a, 1);

  a = 0;
  init(&temp);
  temp.bits[0] = 123;
  temp.bits[1] = 123;
  int res = 0;
  res = s21_from_decimal_to_int(temp, &a);
  ck_assert_uint_eq(res, 1);
}
END_TEST

// FLOAT

START_TEST(part1_conv_float) {
  s21_decimal temp;
  init(&temp);
  float a = 0.0;
  s21_from_float_to_decimal(a, &temp);
  ck_assert_int_eq(temp.bits[0], 0);
  ck_assert_int_eq(getSign(temp), 0);

  init(&temp);
  a = 20.0;
  s21_from_float_to_decimal(a, &temp);
  ck_assert_int_eq(temp.bits[0], 20);
  ck_assert_int_eq(getSign(temp), 0);
  ck_assert_int_eq(getPower(temp), 0);

  init(&temp);
  a = -20.0;
  s21_from_float_to_decimal(a, &temp);
  ck_assert_int_eq(temp.bits[0], 20);
  ck_assert_int_eq(getSign(temp), 1);
  ck_assert_int_eq(getPower(temp), 0);

  init(&temp);
  a = 0.0 / 0.0;
  int res = s21_from_float_to_decimal(a, &temp);
  ck_assert_int_eq(res, 1);

  init(&temp);
  a = 1.0 / 0.0;
  res = 0;
  res = s21_from_float_to_decimal(a, &temp);
  ck_assert_int_eq(res, 1);

  init(&temp);
  a = -1.0 / 0.0;
  res = 0;
  res = s21_from_float_to_decimal(a, &temp);
  ck_assert_int_eq(res, 1);

  init(&temp);
  a = 0;
  temp.bits[0] = 45;
  setPower(&temp, 1);
  s21_from_decimal_to_float(temp, &a);
  ck_assert_float_eq(a, 4.5);

  init(&temp);
  a = 0;
  temp.bits[0] = 45;
  setPower(&temp, 1);
  setSign(&temp, 1);
  s21_from_decimal_to_float(temp, &a);
  ck_assert_float_eq(a, -4.5);
}
END_TEST

// ANOTHER

START_TEST(part4_negate_normal) {
  s21_decimal value_1 = {{154345}};
  s21_decimal result = {0};
  s21_decimal test = {{154345}};
  setSign(&value_1, 0);
  s21_negate(value_1, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_int_eq(getSign(result), 1);
}
END_TEST

START_TEST(part4_negate_null) {
  s21_decimal value_1 = {{0}};
  s21_decimal result = {0};
  s21_decimal test = {0};
  s21_negate(value_1, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_int_eq(getSign(result), 1);
}
END_TEST

START_TEST(part4_negate_negative) {
  s21_decimal value_1 = {{123455}};
  s21_decimal result = {0};
  s21_decimal test = {{123455}};
  setSign(&value_1, 1);
  s21_negate(value_1, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_int_eq(getSign(result), 0);
}
END_TEST

START_TEST(part4_negate_pow) {
  s21_decimal value_1 = {{123455}};
  s21_decimal result = {0};
  s21_decimal test = {{123455}};
  setSign(&value_1, 1);
  setPower(&value_1, 3);
  s21_negate(value_1, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_int_eq(getSign(result), 0);
  ck_assert_int_eq(getPower(result), 3);
}
END_TEST

START_TEST(part1_floor) {
  s21_decimal value_1 = {0};
  s21_decimal result = {0};
  s21_decimal test = {0};
  value_1.bits[0] = 97;
  test.bits[0] = 97;
  s21_floor(value_1, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_int_eq(getSign(result), 0);
  ck_assert_int_eq(getPower(result), 0);

  setSign(&value_1, 1);
  s21_floor(value_1, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_int_eq(getSign(result), 1);
  ck_assert_int_eq(getPower(result), 0);
}
END_TEST

START_TEST(part2_floor) {
  s21_decimal value_1 = {0};
  s21_decimal result = {0};
  s21_decimal test = {0};
  value_1.bits[0] = 976546;
  test.bits[0] = 976;
  setPower(&value_1, 3);
  s21_floor(value_1, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_int_eq(getSign(result), 0);
  ck_assert_int_eq(getPower(result), 0);

  test.bits[0] = 977;
  setSign(&value_1, 1);
  s21_floor(value_1, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_int_eq(getSign(result), 1);
  ck_assert_int_eq(getPower(result), 0);
}
END_TEST

START_TEST(part3_floor) {
  s21_decimal value_1 = {0};
  s21_decimal result = {0};
  s21_decimal test = {0};
  value_1.bits[0] = 9765;
  test.bits[0] = 976;
  setPower(&value_1, 1);
  s21_floor(value_1, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_int_eq(getSign(result), 0);
  ck_assert_int_eq(getPower(result), 0);

  test.bits[0] = 977;
  setSign(&value_1, 1);
  s21_floor(value_1, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_int_eq(getSign(result), 1);
  ck_assert_int_eq(getPower(result), 0);
}
END_TEST

START_TEST(part4_floor) {
  s21_decimal value_1 = {0};
  s21_decimal result = {0};
  s21_decimal test = {0};
  test.bits[0] = 0;
  setPower(&value_1, 1);
  s21_floor(value_1, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_int_eq(getSign(result), 0);
  ck_assert_int_eq(getPower(result), 0);

  setSign(&value_1, 1);
  s21_floor(value_1, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_int_eq(getPower(result), 0);
}
END_TEST

START_TEST(part5_floor) {
  s21_decimal value_1 = {0};
  s21_decimal result = {0};
  s21_decimal test = {0};
  test.bits[0] = 0;
  setPower(&value_1, 30);
  s21_floor(value_1, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_int_eq(getSign(result), 0);
  ck_assert_int_eq(getPower(result), 0);

  setSign(&value_1, 30);
  s21_floor(value_1, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_int_eq(getSign(result), 0);
  ck_assert_int_eq(getPower(result), 0);
}
END_TEST

START_TEST(part1_round) {
  s21_decimal value_1 = {0};
  s21_decimal result = {0};
  s21_decimal test = {0};
  value_1.bits[0] = 97;
  test.bits[0] = 97;
  s21_round(value_1, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_int_eq(getSign(result), 0);
  ck_assert_int_eq(getPower(result), 0);

  setSign(&value_1, 1);
  s21_round(value_1, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_int_eq(getSign(result), 1);
  ck_assert_int_eq(getPower(result), 0);
}
END_TEST

START_TEST(part2_round) {
  s21_decimal value_1 = {0};
  s21_decimal result = {0};
  s21_decimal test = {0};
  value_1.bits[0] = 976546;
  test.bits[0] = 977;
  setPower(&value_1, 3);
  s21_round(value_1, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_int_eq(getSign(result), 0);
  ck_assert_int_eq(getPower(result), 0);

  test.bits[0] = 977;
  setSign(&value_1, 1);
  s21_round(value_1, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_int_eq(getSign(result), 1);
  ck_assert_int_eq(getPower(result), 0);
}
END_TEST

START_TEST(part3_round) {
  s21_decimal value_1 = {0};
  s21_decimal result = {0};
  s21_decimal test = {0};
  value_1.bits[0] = 9765;
  test.bits[0] = 977;
  setPower(&value_1, 1);
  s21_round(value_1, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_int_eq(getSign(result), 0);
  ck_assert_int_eq(getPower(result), 0);

  setSign(&value_1, 1);
  s21_round(value_1, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_int_eq(getSign(result), 1);
  ck_assert_int_eq(getPower(result), 0);
}
END_TEST

START_TEST(part4_round) {
  s21_decimal value_1 = {0};
  s21_decimal result = {0};
  s21_decimal test = {0};
  test.bits[0] = 0;
  setPower(&value_1, 1);
  s21_round(value_1, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_int_eq(getSign(result), 0);
  ck_assert_int_eq(getPower(result), 0);

  setSign(&value_1, 1);
  s21_round(value_1, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_int_eq(getPower(result), 0);
}
END_TEST

START_TEST(part5_round) {
  s21_decimal value_1 = {0};
  s21_decimal result = {0};
  s21_decimal test = {0};
  test.bits[0] = 0;
  setPower(&value_1, 30);
  s21_round(value_1, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_int_eq(getSign(result), 0);
  ck_assert_int_eq(getPower(result), 0);

  setSign(&value_1, 1);
  s21_round(value_1, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_int_eq(getPower(result), 0);
}
END_TEST

START_TEST(part1_truncate) {
  s21_decimal value_1 = {0};
  s21_decimal result = {0};
  s21_decimal test = {0};
  value_1.bits[0] = 97;
  test.bits[0] = 97;
  s21_truncate(value_1, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_int_eq(getSign(result), 0);
  ck_assert_int_eq(getPower(result), 0);

  setSign(&value_1, 1);
  s21_truncate(value_1, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_int_eq(getSign(result), 1);
  ck_assert_int_eq(getPower(result), 0);
}
END_TEST

START_TEST(part2_truncate) {
  s21_decimal value_1 = {0};
  s21_decimal result = {0};
  s21_decimal test = {0};
  value_1.bits[0] = 976546;
  test.bits[0] = 976;
  setPower(&value_1, 3);
  s21_truncate(value_1, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_int_eq(getSign(result), 0);
  ck_assert_int_eq(getPower(result), 0);

  setSign(&value_1, 1);
  s21_truncate(value_1, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_int_eq(getSign(result), 1);
  ck_assert_int_eq(getPower(result), 0);
}
END_TEST

START_TEST(part3_truncate) {
  s21_decimal value_1 = {0};
  s21_decimal result = {0};
  s21_decimal test = {0};
  value_1.bits[0] = 9765;
  test.bits[0] = 976;
  setPower(&value_1, 1);
  s21_truncate(value_1, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_int_eq(getSign(result), 0);
  ck_assert_int_eq(getPower(result), 0);

  setSign(&value_1, 1);
  s21_truncate(value_1, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_int_eq(getSign(result), 1);
  ck_assert_int_eq(getPower(result), 0);
}
END_TEST

START_TEST(part4_truncate) {
  s21_decimal value_1 = {0};
  s21_decimal result = {0};
  s21_decimal test = {0};
  test.bits[0] = 0;
  setPower(&value_1, 1);
  s21_truncate(value_1, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_int_eq(getSign(result), 0);
  ck_assert_int_eq(getPower(result), 0);

  setSign(&value_1, 1);
  s21_truncate(value_1, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_int_eq(getPower(result), 0);
}
END_TEST

START_TEST(part5_truncate) {
  s21_decimal value_1 = {0};
  s21_decimal result = {0};
  s21_decimal test = {0};
  test.bits[0] = 0;
  setPower(&value_1, 30);
  s21_truncate(value_1, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_int_eq(getSign(result), 0);
  ck_assert_int_eq(getPower(result), 0);

  setSign(&value_1, 1);
  s21_truncate(value_1, &result);
  ck_assert_uint_eq(result.bits[0], test.bits[0]);
  ck_assert_int_eq(getPower(result), 0);
}
END_TEST

// SUITES

Suite *suite_1() {
  Suite *s = suite_create("suite_1");
  TCase *tc_add = tcase_create("tc_add");
  TCase *tc_sub = tcase_create("tc_sub");
  TCase *tc_mult = tcase_create("tc_mult");
  TCase *tc_div = tcase_create("tc_div");

  tcase_add_test(tc_add, part1_add_normal);
  tcase_add_test(tc_add, part1_add_both_neg);
  tcase_add_test(tc_add, part1_add_error);
  tcase_add_test(tc_add, part1_add_one_sign);
  tcase_add_test(tc_add, part1_add_one_greater);

  tcase_add_test(tc_sub, part1_sub_normal);
  tcase_add_test(tc_sub, part1_sub_inf);
  tcase_add_test(tc_sub, part1_sub_one_sign);
  tcase_add_test(tc_sub, part1_sub_two_sign);

  tcase_add_test(tc_mult, part1_mult_value_1_is_one);
  tcase_add_test(tc_mult, part1_mult_value_2_is_one);
  tcase_add_test(tc_mult, part1_mult_value_1_is_zero);
  tcase_add_test(tc_mult, part1_mult_value_2_is_zero);
  tcase_add_test(tc_mult, part1_mult_max);
  tcase_add_test(tc_mult, part1_mult_big_power);
  tcase_add_test(tc_mult, part1_mult_big_over);
  tcase_add_test(tc_mult, part1_mult_big_negative_over);
  tcase_add_test(tc_mult, part1_mult_value_1_is_minus);
  tcase_add_test(tc_mult, part1_mult_value_2_is_minus);
  tcase_add_test(tc_mult, part1_mult_value_1_is_zero_minus);
  tcase_add_test(tc_mult, part1_mult_value_2_is_zero_minus);
  tcase_add_test(tc_mult, part1_mult_value_1_2_is_minus);

  tcase_add_test(tc_div, part1_div_zero);
  tcase_add_test(tc_div, part2_div);
  tcase_add_test(tc_div, part3_div);
  tcase_add_test(tc_div, part4_div);

  suite_add_tcase(s, tc_add);
  suite_add_tcase(s, tc_sub);
  suite_add_tcase(s, tc_mult);
  suite_add_tcase(s, tc_div);

  return s;
}

Suite *suite_2() {
  Suite *s = suite_create("suite_2");

  TCase *tc_less = tcase_create("tc_less");
  TCase *tc_less_or_equal = tcase_create("tc_less_or_equal");
  TCase *tc_greater = tcase_create("tc_greater");
  TCase *tc_greater_or_equal = tcase_create("tc_greater_or_equal");
  TCase *tc_equal = tcase_create("tc_equal");
  TCase *tc_notequal = tcase_create("tc_notequal");

  tcase_add_test(tc_less, part2_less_null);
  tcase_add_test(tc_less, part2_less_normal);
  tcase_add_test(tc_less, part2_less_negative);
  tcase_add_test(tc_less, part2_less_pow);

  tcase_add_test(tc_less_or_equal, part2_less_or_equal_normal);
  tcase_add_test(tc_less_or_equal, part2_less_or_equal_with_sign);

  tcase_add_test(tc_greater, part2_greater_null);
  tcase_add_test(tc_greater, part2_greater_normal);
  tcase_add_test(tc_greater, part2_greater_negative);
  tcase_add_test(tc_greater, part2_greater_pow);
  tcase_add_test(tc_greater, part2_greater_two_negative);

  tcase_add_test(tc_greater_or_equal, part2_greater_or_equal_normal);
  tcase_add_test(tc_greater_or_equal, part2_greater_or_equal_with_sign);

  tcase_add_test(tc_equal, part2_equal_1);
  tcase_add_test(tc_equal, part2_equal_2);
  tcase_add_test(tc_equal, part2_equal_3);
  tcase_add_test(tc_equal, part2_equal_4);
  tcase_add_test(tc_equal, part2_equal_5);
  tcase_add_test(tc_equal, part2_equal_6);
  tcase_add_test(tc_equal, part2_equal_7);

  tcase_add_test(tc_notequal, part2_notequal_1);
  tcase_add_test(tc_notequal, part2_notequal_2);
  tcase_add_test(tc_notequal, part2_notequal_3);
  tcase_add_test(tc_notequal, part2_notequal_4);
  tcase_add_test(tc_notequal, part2_notequal_5);
  tcase_add_test(tc_notequal, part2_notequal_6);

  suite_add_tcase(s, tc_equal);
  suite_add_tcase(s, tc_notequal);
  suite_add_tcase(s, tc_less);
  suite_add_tcase(s, tc_less_or_equal);
  suite_add_tcase(s, tc_greater);
  suite_add_tcase(s, tc_greater_or_equal);

  return s;
}

Suite *suite_3() {
  Suite *s = suite_create("suite_3");

  TCase *tc_int = tcase_create("tc_int");
  TCase *tc_float = tcase_create("tc_float");

  tcase_add_test(tc_int, part1_conv_int);
  tcase_add_test(tc_float, part1_conv_float);

  suite_add_tcase(s, tc_int);
  suite_add_tcase(s, tc_float);

  return s;
}

Suite *suite_4() {
  Suite *s = suite_create("suite_4");

  TCase *tc_negate = tcase_create("tc_another");
  TCase *tc_floor = tcase_create("tc_floor");
  TCase *tc_round = tcase_create("tc_round");
  TCase *tc_truncate = tcase_create("tc_truncate");

  tcase_add_test(tc_negate, part4_negate_normal);
  tcase_add_test(tc_negate, part4_negate_null);
  tcase_add_test(tc_negate, part4_negate_negative);
  tcase_add_test(tc_negate, part4_negate_pow);

  tcase_add_test(tc_floor, part1_floor);
  tcase_add_test(tc_floor, part2_floor);
  tcase_add_test(tc_floor, part3_floor);
  tcase_add_test(tc_floor, part4_floor);
  tcase_add_test(tc_floor, part5_floor);

  tcase_add_test(tc_round, part1_round);
  tcase_add_test(tc_round, part2_round);
  tcase_add_test(tc_round, part3_round);
  tcase_add_test(tc_round, part4_round);
  tcase_add_test(tc_round, part5_round);

  tcase_add_test(tc_truncate, part1_truncate);
  tcase_add_test(tc_truncate, part2_truncate);
  tcase_add_test(tc_truncate, part3_truncate);
  tcase_add_test(tc_truncate, part4_truncate);
  tcase_add_test(tc_truncate, part5_truncate);

  suite_add_tcase(s, tc_negate);
  suite_add_tcase(s, tc_floor);
  suite_add_tcase(s, tc_round);
  suite_add_tcase(s, tc_truncate);

  return s;
}

int main() {
  printf("~~~~~~~~~\n");
  printf("\n");

  Suite *s_1 = suite_1();
  Suite *s_2 = suite_2();
  Suite *s_3 = suite_3();
  Suite *s_4 = suite_4();

  run_test(s_1);
  run_test(s_2);
  run_test(s_3);
  run_test(s_4);

  printf("\n");
  printf("~~~~~~~~~\n");

  return 0;
}
