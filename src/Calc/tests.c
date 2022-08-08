#include <string.h>
#include <stdio.h>
#include <check.h>
#include "s21_calc.h"
#define EPS 1e-7

START_TEST(Test1) {
    char expression[] = {"(2+3)*5"};
    char* pexp = &expression[0];
    double result = 0;
    get_calculated(pexp, &result);
    ck_assert_float_eq(result, 25.0);
}
END_TEST

START_TEST(Test2) {
    char expression[] = {"1/2 + (2+3)/(sin(9-2)^2-6/7)"};
    char* pexp = &expression[0];
    double result = 0;
    double result2 = -11.2506;
    get_calculated(pexp, &result);
    ck_assert_float_lt(EPS, fabs(result2 - result));
}
END_TEST

START_TEST(Test3) {
    char expression[] = {"1/2+(2+3"};
    char* pexp = &expression[0];
    double result = 0;
    ck_assert_int_eq(get_calculated(pexp, &result), 1);
}
END_TEST

START_TEST(Test4) {
    char expression[] = {"1/2+(2.3.5+3)"};
    char* pexp = &expression[0];
    double result = 0;
    ck_assert_int_eq(get_calculated(pexp, &result), 1);
}
END_TEST

START_TEST(Test5) {
    char expression[] = {"**"};
    char* pexp = &expression[0];
    double result = 0;
    ck_assert_int_eq(get_calculated(pexp, &result), 1);
}
END_TEST

START_TEST(Test6) {
    char expression[] = {"sin(2) * cos(5) + tan(67^2) / sin(128) - atan(sqrt(9)) - (4 mod 2) * log(5)"};
    char* pexp = &expression[0];
    double result = 0;
    double result2 = -1.47532;
    get_calculated(pexp, &result);
    ck_assert_float_lt(EPS, fabs(result2 - result));
}
END_TEST

Suite *s21_math_tests_create() {
    Suite *s21_math = suite_create("s21_math");
    TCase *s21_math_tests = tcase_create("S21_MATH");
    tcase_add_test(s21_math_tests, Test1);
    tcase_add_test(s21_math_tests, Test2);
    tcase_add_test(s21_math_tests, Test3);
    tcase_add_test(s21_math_tests, Test4);
    tcase_add_test(s21_math_tests, Test5);
    tcase_add_test(s21_math_tests, Test6);
    suite_add_tcase(s21_math, s21_math_tests);
    return s21_math;
}

int main() {
    Suite *s21_math = s21_math_tests_create();
    SRunner *s21_math_runner = srunner_create(s21_math);
    int number_failed;
    srunner_run_all(s21_math_runner, FP_NORMAL);
    number_failed = srunner_ntests_failed(s21_math_runner);
    srunner_free(s21_math_runner);

    return number_failed == 0 ? 0 : 1;
}
