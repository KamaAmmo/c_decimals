#ifndef S21_DECIMAL_H_
#define S21_DECIMAL_H_

#define SIGN 0x80000000
typedef struct decimal {
  int bits[4];
} s21_decimal;

typedef struct b_decimal {
  int bits[7];
} big_decimal;

typedef union {
  int ui;
  float fl;
} floatbits;

enum state { OK = 0, ERROR = 1 };
enum boolean { FALSE = 0, TRUE = 1 };
enum ariphmetic_states { INF_PLUS = 1, INF_MIN = 2, ZERO_DIV = 3 };

// comparison
int s21_is_equal(s21_decimal da, s21_decimal db);
int s21_is_not_equal(s21_decimal da, s21_decimal db);
int s21_is_greater(s21_decimal da, s21_decimal db);
int s21_is_greater_or_equal(s21_decimal da, s21_decimal db);
int s21_is_less(s21_decimal da, s21_decimal db);
int s21_is_less_or_equal(s21_decimal da, s21_decimal db);
int is_equal_abs(s21_decimal da, s21_decimal db);
int is_greater_abs(s21_decimal da, s21_decimal db);

// arithmetic
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

// convert
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

// another
int s21_negate(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);

// other
void setbit(s21_decimal *dec, int bitpos, int bit);  // bitpos: 0 - 127
int getbit(s21_decimal dec, int bitpos);
void setSign(s21_decimal *dec, int sign);
int getSign(s21_decimal dec);
int getBitOfInteger(int num, int i);
int shiftLeft(s21_decimal *dec, int num);
void bits_copy(s21_decimal src, s21_decimal *dest);
int getLastBit(s21_decimal dec);
void doSomeMagic(s21_decimal *da, s21_decimal db, int last_bit, int index);
void init(s21_decimal *dec);
int getFloatExp(float *src);
int getFloatSign(float *src);
void setPower(s21_decimal *dec, int num);
int getPower(s21_decimal dec);
int normalize(s21_decimal *dec1, s21_decimal *dec2);
int add_fun(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
void sub_fun(s21_decimal da, s21_decimal db, s21_decimal *result);
s21_decimal div_only_bits(s21_decimal number_1, s21_decimal number_2,
                          s21_decimal *buf);
// void showDec(s21_decimal dec);
int trunc_div(s21_decimal v1, s21_decimal v2, s21_decimal *result);
void init_dec(int sign, int pow, unsigned b2, unsigned b1, unsigned b0,
              s21_decimal *value);
int is_too_large(s21_decimal value);
int is_too_small(s21_decimal value);
int check_dec(s21_decimal value);
#endif  // S21_DECIMAL_H_