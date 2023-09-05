#include "s21_decimal.h"

#include <math.h>
#include <stdlib.h>

/////////// ANOTHER ////////////
int s21_negate(s21_decimal value, s21_decimal *result) {
  *result = value;
  int sign = getSign(value) ? 0 : 1;
  setSign(result, sign);
  return OK;
}

int s21_truncate(s21_decimal value, s21_decimal *result) {
  if (result == NULL || getPower(value) > 28) {
    return ERROR;
  }
  int sign = getSign(value);
  int power = getPower(value);
  if (power == 0) {
    *result = value;
    return OK;
  }
  s21_decimal dec_10 = {{10, 0, 0, 0}};
  while (power != 0) {
    trunc_div(value, dec_10, result);
    value = *result;
    power--;
  }
  setPower(result, 0);
  setSign(result, sign);
  return OK;
}

int trunc_div(s21_decimal v1, s21_decimal v2, s21_decimal *result) {
  s21_decimal value = {0}, diff = {0};
  setSign(&diff, getSign(v2));
  setPower(&diff, getPower(v2));
  for (int i = 95; i >= 0; i--) {
    shiftLeft(&diff, 1);
    setbit(&diff, 0, getbit(v1, i));
    setbit(&value, i, !is_greater_abs(v2, diff));
    if (!is_greater_abs(v2, diff)) {
      s21_sub(diff, v2, &diff);
    }
  }
  setSign(&value, getSign(v1) - getSign(v2));
  setPower(&value, getPower(v1) - getPower(v2));
  *result = value;
  return OK;
}

int s21_floor(s21_decimal value, s21_decimal *result) {
  if (result == NULL || getPower(value) > 28) {
    return ERROR;
  }
  int sign = getSign(value);
  s21_decimal temp = {0};
  s21_truncate(value, &temp);
  if (s21_is_not_equal(value, temp) && sign) {
    s21_decimal dec_1 = {{1, 0, 0, 0}};
    s21_sub(temp, dec_1, &temp);
  }
  *result = temp;
  return OK;
}

int s21_round(s21_decimal value, s21_decimal *result) {
  if (result == NULL || getPower(value) > 28) {
    return ERROR;
  }
  int sign = getSign(value);
  s21_decimal temp = {0}, half = {0}, diff = {0};
  s21_from_float_to_decimal(0.5, &half);
  s21_truncate(value, &temp);
  s21_sub(value, temp, &diff);
  setSign(&diff, 0);
  if (s21_is_greater_or_equal(diff, half) && s21_is_not_equal(value, diff)) {
    s21_decimal dec_1 = {{1, 0, 0, 0}};
    if (!sign) {
      s21_add(temp, dec_1, &temp);
    } else {
      s21_sub(temp, dec_1, &temp);
    }
  }
  *result = temp;
  return OK;
}

/////////// ARITHMETIC ////////////

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int sign1 = getSign(value_1);  // 1 : -  | 0 : +
  int sign2 = getSign(value_2);
  int is_overflow = FALSE;
  int state = OK;
  s21_decimal temp_res = {0};
  normalize(&value_1, &value_2);
  if (sign1 == sign2) {
    is_overflow = add_fun(value_1, value_2, &temp_res);
    setSign(&temp_res, sign1);
    if (is_overflow && !sign1)
      state = INF_PLUS;
    else if (is_overflow && sign1)
      state = INF_MIN;
  } else {
    if (is_equal_abs(value_1, value_2))
      setSign(&temp_res, 0);
    else if (is_greater_abs(value_1, value_2)) {
      sub_fun(value_1, value_2, &temp_res);
      setSign(&temp_res, sign1);
    } else {
      sub_fun(value_2, value_1, &temp_res);
      setSign(&temp_res, sign2);
    }
  }
  setPower(&temp_res, getPower(value_1));
  if (state == OK) *result = temp_res;
  return state;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int sign = !getSign(value_2);
  setSign(&value_2, sign);
  return s21_add(value_1, value_2, result);
}

int add_fun(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int ost = 0;
  int is_overflow = FALSE;
  for (int i = 0; i < 96; ++i) {
    int bit = getbit(value_1, i) + getbit(value_2, i) + ost;
    setbit(result, i, getBitOfInteger(bit, 0));
    ost = getBitOfInteger(bit, 1);
  }
  if (ost > 0) is_overflow = TRUE;
  return is_overflow;
}

void sub_fun(s21_decimal da, s21_decimal db, s21_decimal *result) {
  // da > db
  s21_decimal temp_res = {0};
  if (s21_is_equal(da, db))
    *result = temp_res;
  else if (is_greater_abs(da, db)) {
    int the_biggest_bit = getLastBit(da);

    for (int i = 0; i <= the_biggest_bit; ++i) {
      doSomeMagic(&da, db, the_biggest_bit, i);
      int bit = getbit(da, i) ^ getbit(db, i);
      setbit(&temp_res, i, bit);
    }
    setSign(&temp_res, getSign(da));
    *result = temp_res;
  }
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  s21_decimal temp = {0};
  int state = OK;
  for (int i = 0; i < 96; ++i) {
    if (getbit(value_2, i)) {
      s21_decimal value_1_copy = value_1;
      shiftLeft(&value_1_copy, i);
      state = add_fun(temp, value_1_copy, &temp);
      if (state) break;
    }
  }

  if (getSign(value_1) != getSign(value_2))
    setSign(&temp, 1);
  else
    setSign(&temp, 0);
  int sum_of_powers = getPower(value_1) + getPower(value_2);
  if (sum_of_powers < 29)
    setPower(&temp, sum_of_powers);
  else
    setPower(&temp, 29);
  if (!getSign(temp) && state)
    state = INF_PLUS;
  else if (getSign(temp) && state)
    state = INF_MIN;
  *result = temp;
  return state;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  init(result);
  s21_decimal zero = {{0, 0, 0, 0}};

  if (s21_is_equal(value_2, zero) && s21_is_less(value_1, zero))
    return INF_MIN;
  else if (s21_is_equal(value_2, zero) && s21_is_greater(value_1, zero))
    return INF_PLUS;
  else if (s21_is_equal(value_2, zero) && s21_is_equal(value_1, zero))
    return ZERO_DIV;
  else {
    int beginScale = getPower(value_1) - getPower(value_2);
    int resultSign = getSign(value_1) != getSign(value_2);
    s21_decimal remainder = {0}, tmp = {0}, res_tmp = {0};

    setPower(&value_2, 0);
    setPower(&value_1, 0);
    setSign(&value_2, 0);
    setSign(&value_1, 0);

    tmp = div_only_bits(value_1, value_2, &remainder);
    bits_copy(tmp, result);

    s21_decimal border_value = {{-1, -1, -1, 0}};
    s21_decimal ten = {{10, 0, 0, 0}};

    setPower(&border_value, 1);
    int inside_scale = 0;

    for (; inside_scale <= 27 && s21_is_equal(remainder, zero) == FALSE;) {
      if (s21_is_less(*result, border_value) == FALSE) {
        break;
      }
      s21_mul(remainder, ten, &remainder);
      tmp = div_only_bits(remainder, value_2, &remainder);
      s21_mul(*result, ten, result);
      s21_add(*result, tmp, result);
      inside_scale++;
    }
    s21_decimal off;
    init(&off);
    int endScale = beginScale + inside_scale;
    for (; endScale > 28;) {
      res_tmp = div_only_bits(*result, ten, &off);
      result = &res_tmp;
      endScale--;
    }
    for (; endScale < 0;) {
      s21_mul(*result, ten, result);
      endScale++;
    }
    setPower(result, endScale);
    setSign(result, resultSign);
  }
  return OK;
}

s21_decimal div_only_bits(s21_decimal number_1, s21_decimal number_2,
                          s21_decimal *buf) {
  init(buf);
  s21_decimal res = {{0, 0, 0, 0}};
  int the_biggest_bit = getLastBit(number_1);
  for (int i = the_biggest_bit; i >= 0; i--) {
    if (getbit(number_1, i)) {
      setbit(buf, 0, 1);
    }
    if (s21_is_greater_or_equal(*buf, number_2) == TRUE) {
      s21_sub(*buf, number_2, buf);
      if (i != 0) {
        shiftLeft(buf, 1);
      }
      if (getbit(number_1, i - 1)) {
        setbit(buf, 0, 1);
      }
      shiftLeft(&res, 1);
      setbit(&res, 0, 1);
    } else {
      shiftLeft(&res, 1);
      if (i != 0) {
        shiftLeft(buf, 1);
      }
      if ((i - 1) >= 0 && getbit(number_1, i - 1)) {
        setbit(buf, 0, 1);
      }
    }
  }
  return res;
}

/////////// COMPARE ////////////

int isMantissZero(s21_decimal dec) {
  int result = TRUE;
  for (int i = 0; i < 3; ++i) {
    if (dec.bits[i]) result = FALSE;
  }
  return result;
}

int s21_is_greater(s21_decimal da, s21_decimal db) {
  int sign1 = getSign(da);
  int sign2 = getSign(db);
  int is_da_bigger_than_db = FALSE;
  normalize(&da, &db);

  if ((sign1 == sign2) && (getPower(da) == getPower(db))) {
    for (int i = 95; i >= 0; --i) {
      int bit_da = getbit(da, i);
      int bit_db = getbit(db, i);
      if (bit_da > bit_db) {
        is_da_bigger_than_db = TRUE;
        break;
      } else if (bit_da < bit_db) {
        break;
      }
    }
  } else {
    if (isMantissZero(da) && isMantissZero(db))
      is_da_bigger_than_db = FALSE;
    else if ((sign1 < sign2))
      is_da_bigger_than_db = TRUE;
  }
  if (sign1 && sign2)
    is_da_bigger_than_db = is_da_bigger_than_db ? FALSE : TRUE;
  return is_da_bigger_than_db;
}

int s21_is_equal(s21_decimal da, s21_decimal db) {
  int sign1 = getSign(da);
  int sign2 = getSign(db);
  int is_equal = TRUE;
  normalize(&da, &db);
  if (!isMantissZero(da) || !isMantissZero(db)) {
    if (sign1 == sign2) {
      for (int i = 0; i < 96 && is_equal; ++i) {
        if (getbit(da, i) != getbit(db, i)) is_equal = FALSE;
      }
    } else
      is_equal = FALSE;
  }
  return is_equal;
}

int s21_is_not_equal(s21_decimal da, s21_decimal db) {
  return (s21_is_equal(da, db)) ? FALSE : TRUE;
}

int s21_is_greater_or_equal(s21_decimal da, s21_decimal db) {
  return (s21_is_equal(da, db) || s21_is_greater(da, db)) ? TRUE : FALSE;
}

int s21_is_less(s21_decimal da, s21_decimal db) {
  return (!s21_is_greater(da, db) && !is_equal_abs(da, db)) ? TRUE : FALSE;
}

int s21_is_less_or_equal(s21_decimal da, s21_decimal db) {
  return (s21_is_equal(da, db) || !s21_is_greater(da, db)) ? TRUE : FALSE;
}

int is_greater_abs(s21_decimal da, s21_decimal db) {
  setSign(&da, 0);
  setSign(&db, 0);
  return s21_is_greater(da, db);
}

int is_equal_abs(s21_decimal da, s21_decimal db) {
  setSign(&da, 0);
  setSign(&db, 0);
  return s21_is_equal(da, db);
}

/////////// CONVERT ////////////

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int state = ERROR;
  s21_decimal max_dec = {{2147483647, 0, 0, 0}};
  s21_decimal min_dec = {{-2147483648, 0, 0, 0}};
  setSign(&min_dec, 1);
  if (s21_is_less_or_equal(src, max_dec) &&
      s21_is_greater_or_equal(src, min_dec)) {
    *dst = src.bits[0];
    *dst *= getSign(src) ? -1 : 1;
    *dst = *dst / (int)pow(10.0, (double)getPower(src));
    state = OK;
  }
  return state;
}

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  init(dst);
  if (src < 0) setSign(dst, 1);
  dst->bits[0] = abs(src);
  return OK;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int state = ERROR;
  s21_decimal max_dec = {{2147483647, 0, 0, 0}};
  s21_decimal min_dec = {{-2147483648, 0, 0, 0}};
  setSign(&min_dec, 1);
  if (s21_is_less_or_equal(src, max_dec) &&
      s21_is_greater_or_equal(src, min_dec)) {
    double temp = 0;
    for (int i = 0; i < 96; ++i) {
      if (getbit(src, i)) temp += pow(2.0, i);
    }
    temp /= pow(10.0, getPower(src));
    *dst = (float)temp;
    *dst *= getSign(src) ? -1 : 1;
    state = OK;
  }
  return state;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  if (isinf(src) || isnan(src)) return ERROR;
  int exp = getFloatExp(&src), sign = (src < 0) ? 1 : 0;
  int res = ERROR;
  init(dst);
  if (sign == 1) setSign(dst, 1);
  if (dst && src != 0) {
    double temp = (double)fabs(src);
    int off = 0;
    for (; off < 28 && (int)temp / (int)pow(2, 21) == 0; temp *= 10, off++)
      ;
    temp = round(temp);
    if (off <= 28 && (exp > -94 && exp < 96)) {
      floatbits mant;
      mant.fl = 0.0;
      mant.ui = 0;
      temp = (float)temp;
      for (; fmod(temp, 10) == 0 && off > 0; off--, temp /= 10)
        ;
      mant.fl = temp;
      exp = getFloatExp(&mant.fl);
      dst->bits[exp / 32] |= 1 << exp % 32;
      for (int i = exp - 1, j = 22; j >= 0; i--, j--)
        if ((mant.ui & (1 << j)) != 0) {
          dst->bits[i / 32] |= 1 << i % 32;
        }
      dst->bits[3] = (sign << 31) | (off << 16);
      res = OK;
    }
  }
  return res;
}

/////////// OTHER ////////////

void setbit(s21_decimal *dec, int bitpos, int bit) {
  if (bit == 0)
    dec->bits[bitpos / 32] &= ~(1 << bitpos % 32);
  else if (bit == 1)
    dec->bits[bitpos / 32] |= (1 << bitpos % 32);
}

int getbit(s21_decimal dec, int bitpos) {
  return ((dec.bits[bitpos / 32] & (1 << (bitpos % 32))) != 0);
}
void setSign(s21_decimal *dec, int sign) { setbit(dec, 127, sign); }

int getSign(s21_decimal dec) { return getbit(dec, 127); }

int getBitOfInteger(int num, int i) { return (num & 1 << i) != 0; }

void init(s21_decimal *dec) {
  for (int i = 0; i < 4; i++) dec->bits[i] = 0U;
}

int getFloatExp(float *src) { return ((*(int *)src & ~SIGN) >> 23) - 127; }

int getFloatSign(float *src) { return ((*(int *)src) >> 31); }

int shiftLeft(s21_decimal *dec, int num) {
  s21_decimal temp = *dec;
  for (int i = 95; i >= num; --i) {
    if (getbit(temp, i - num))
      setbit(&temp, i, 1);
    else
      setbit(&temp, i, 0);
  }
  for (int i = num - 1; i >= 0; --i) setbit(&temp, i, 0);
  int is_overflow = check_dec(temp);
  if (!is_overflow) *dec = temp;
  return is_overflow;
}

void bits_copy(s21_decimal src, s21_decimal *dest) {
  dest->bits[0] = src.bits[0];
  dest->bits[1] = src.bits[1];
  dest->bits[2] = src.bits[2];
}

int getLastBit(s21_decimal dec) {
  int result = -1;
  for (int i = 95; i >= 0; --i) {
    if (getbit(dec, i)) {
      result = i;
      break;
    }
  }
  return result;
}

void doSomeMagic(s21_decimal *da, s21_decimal db, int last_bit, int index) {
  if (!getbit(*da, index) &&
      getbit(db, index)) {  // da[i] = 0, db[i] = 1, da[i] - db[i] case
    for (int j = index + 1; j <= last_bit; ++j) {
      if (!getbit(*da, j))
        setbit(da, j, 1);
      else {
        setbit(da, j, 0);
        break;
      }
    }
  }
}

int normalize(s21_decimal *dec1, s21_decimal *dec2) {
  int state = OK;
  int exp1 = getPower(*dec1), exp2 = getPower(*dec2);
  if (exp1 != exp2) {
    int *min_exp = (exp1 <= exp2) ? &exp1 : &exp2;
    int *max_exp = (exp1 > exp2) ? &exp1 : &exp2;
    s21_decimal dec_10 = {{10, 0, 0, 0}};
    s21_decimal *ptr_to_dec = (*min_exp == exp1) ? dec1 : dec2;
    while ((*min_exp < *max_exp) && *min_exp <= 28 && !state) {
      state = s21_mul(*ptr_to_dec, dec_10, ptr_to_dec);
      *min_exp += 1;
      setPower(ptr_to_dec, *min_exp);
    }
  }
  return state;
}

int getPower(s21_decimal dec) {
  int result = 0;
  for (int i = 112, j = 0; i < 120; ++i, ++j) {
    int bit = getbit(dec, i);
    result += bit * pow(2, j);
  }
  return result;
}

void setPower(s21_decimal *dec, int num) {
  for (int i = 112, j = 0; i < 120; i++, j++) {
    int flag = getBitOfInteger(num, j);
    if (flag)
      setbit(dec, i, 1);
    else
      setbit(dec, i, 0);
  }
}

// void showDec(s21_decimal dec) {
//   for (int i = 127; i >= 0; --i) {
//     if (i == 111 || i == 119)
//       printf("%c", '|');
//     printf("%d", getbit(dec, i));
//     if (!(i % 32))
//       printf(" ");
//   }
//   printf("\n");
// }

int check_dec(s21_decimal value) {
  return (is_too_small(value) ? 2 : is_too_large(value) ? 1 : 0);
}

int is_too_small(s21_decimal value) {
  s21_decimal min_minus, min_plus, max_minus;
  init_dec(1, 0, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, &max_minus);
  init_dec(0, 28, 0, 0, 1, &min_plus);
  init_dec(1, 28, 0, 0, 1, &min_minus);
  return s21_is_less(value, max_minus);
}

int is_too_large(s21_decimal value) {
  s21_decimal max_dec;
  init_dec(0, 0, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, &max_dec);
  return s21_is_greater(value, max_dec);
}

void init_dec(int sign, int pow, unsigned b2, unsigned b1, unsigned b0,
              s21_decimal *value) {
  value->bits[3] = 0;
  value->bits[2] = b2;
  value->bits[1] = b1;
  value->bits[0] = b0;
  setSign(value, sign);
  setPower(value, pow);
}
