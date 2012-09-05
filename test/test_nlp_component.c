/*
 * File:        test_nlp_component.c
 * Version:     0.1.0
 * Maintainer:  Shintaro Kaneko <kaneshin0120@gmail.com>
 * Last Change: 05-Sep-2012.
 */

#include "../lbfgs/include/nlp_component.h"

#include <CUnit/CUnit.h>
#include <CUnit/Console.h>
#include <stdlib.h>

static int n;
static nlp_float *x, *g;
static nlp_component component;
static char *method_name;
static function_object func_obj;
static evaluate_object eval_obj;

static nlp_float
func(
    const nlp_float *x,
    int n
);

static void
grad(
    nlp_float *g,
    const nlp_float *x,
    int n
);

static void
test_initialize_nlp_component(void)
{
    int i;
    CU_ASSERT_EQUAL(method_name, component.method);
    CU_ASSERT_EQUAL(0, component.iter_f);
    CU_ASSERT_EQUAL(0, component.iter_g);
    CU_ASSERT_EQUAL(0., component.fx);
    CU_ASSERT_EQUAL(0., component.alpha);
    for (i = 0; i < n; ++i)
        x[i] = 0.;
    eval_obj.func(x, n, &component);
    eval_obj.grad(g, x, n, &component);
    CU_ASSERT_EQUAL(1, component.iter_f);
    CU_ASSERT_EQUAL(1, component.iter_g);
    CU_ASSERT_EQUAL(2., component.fx);
    CU_ASSERT_EQUAL(0., g[0]);
    CU_ASSERT_EQUAL(-2., g[1]);
    for (i = 0; i < n; ++i)
        x[i] = 1.;
    eval_obj.func_grad(g, x, n, &component);
    CU_ASSERT_EQUAL(2, component.iter_f);
    CU_ASSERT_EQUAL(2, component.iter_g);
    CU_ASSERT_EQUAL(.5, component.fx);
    CU_ASSERT_EQUAL(0., g[0]);
    CU_ASSERT_EQUAL(-1., g[1]);
}

int
main(int argc, char* argv[])
{
    int i;
    n = 2;
    x = (nlp_float *)malloc(sizeof(nlp_float) * n);
    g = (nlp_float *)malloc(sizeof(nlp_float) * n);

    method_name = "test LBFGS";

    func_obj.func = func;
    func_obj.grad = grad;

    initialize_nlp_component(method_name, &func_obj, &eval_obj, &component);

    CU_pSuite testSuite;
    CU_initialize_registry();
    testSuite = CU_add_suite("nlp_component.c TestSuite", NULL, NULL);

    CU_add_test(testSuite, "initialize_nlp_component Test", test_initialize_nlp_component);

    CU_console_run_tests();
    CU_cleanup_registry();

    free(x);
    free(g);

    return 0;
}

static nlp_float
func(
    const nlp_float *x,
    int n
)
{
    return (x[0] - x[1] * x[1]) * (x[0] - x[1] * x[1]) + (x[1] - 2.) * (x[1] - 2.) / 2.;
}


static void
grad(
    nlp_float *g,
    const nlp_float *x,
    int n
)
{
    g[0] = x[0] - x[1] * x[1];
    g[1] = -2. * x[1] * (x[0] - x[1] * x[1]) + x[1] - 2.;
}

