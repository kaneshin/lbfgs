/*
 * File:        test_exmath.c
 * Version:     0.1.0
 * Maintainer:  Shintaro Kaneko <kaneshin0120@gmail.com>
 * Last Change: 05-Sep-2012.
 */

#include "../lbfgs/exmath/include/exmath.h"

#include <CUnit/CUnit.h>
#include <CUnit/Console.h>
#include <stdlib.h>

#include "../lbfgs/include/nlp_component.h"

static int n;
static nlp_float *x, *y;

static void
test_dot_product(void)
{
    int i;

    for (i = 0; i < n; ++i)
    {
        x[i] = (i - 3) * 1.;
        y[i] = 1.;
    }
    CU_ASSERT_EQUAL(15., dot_product(x, y, n));

    for (i = 0; i < n; ++i)
    {
        x[i] = 1.;
        y[i] = i;
    }
    CU_ASSERT_EQUAL(45., dot_product(x, y, n));
}

static void
test_infinity_norm(void)
{
    int i;

    for (i = 0; i < n; ++i)
    {
        x[i] = 0.;
        y[i] = 0.;
    }
    x[5] = 1.;
    y[1] = -2.;
    CU_ASSERT_EQUAL(1., infinity_norm(x, n));
    CU_ASSERT_EQUAL(2., infinity_norm(y, n));
}

int
main(int argc, char* argv[])
{
    int i;

    n = 10;
    x = (nlp_float *)malloc(sizeof(nlp_float) * n);
    y = (nlp_float *)malloc(sizeof(nlp_float) * n);

    CU_pSuite testSuite;
    CU_initialize_registry();
    testSuite = CU_add_suite("exmath.c TestSuite", NULL, NULL);

    CU_add_test(testSuite, "dot_product Test", test_dot_product);
    CU_add_test(testSuite, "infinity_norm Test", test_infinity_norm);

    CU_console_run_tests();
    CU_cleanup_registry();

    free(x);
    free(y);

    return 0;
}

