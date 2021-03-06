/*
 * File:        driver3.c
 * Maintainer:  Shintaro Kaneko <kaneshin0120@gmail.com>
 * Last Change: 06-Sep-2012.
 *
 * Problem:     Ackley function
 */

#include "../lbfgs/include/lbfgs.h"

#include <stdio.h>
#include <math.h>

#include "../lbfgs/include/nlp_component.h"
#include "../lbfgs/linesearch/include/linesearch.h"
#include "../lbfgs/exmath/include/exmath.h"

#ifndef PI
#define PI 3.14159265358979323846264338327
#endif /* PI */

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

static double
function(const double *x, int n);

static void
gradient(double *g, const double *x, int n);

int
main(int argc, char* argv[])
{
    int i, n, status;
    nlp_float *x;
    function_object func_obj;
    linesearch_parameter ls_parameter;
    lbfgs_parameter parameter;

    n = 10;
    x = (nlp_float *)malloc_vec(n * sizeof(nlp_float));
    for (i = 0; i < n; ++i)
        x[i] = 1.;

    func_obj.func = func;
    func_obj.grad = grad;

    default_lbfgs_parameter(&parameter);
    default_linesearch_parameter(&ls_parameter);

    status = lbfgs(
        x,
        n,
        &func_obj,
        &ls_parameter,
        &parameter
    );

    if (LBFGS_SATISFIED == status)
    {
        printf("\n=== Optimal x =========================================\n");
        for (i = 0; i < n; ++i)
            printf(" x_%-8d = %12.6e\n", i + 1 , x[i]);
        printf("=======================================================\n");
    }

    free_vec(x);
    return 0;
}

static nlp_float
func(
    const nlp_float *x,
    int n
)
{
    int i;
    nlp_float f = 0., norm, temp;
    norm = 0.;
    temp = 0.;
    for (i = 0; i < n; ++i) {
        norm += x[i] * x[i];
        temp += cos(2 * PI * x[i]);
    }
    norm = sqrt(norm);
    f = 20. * (1. - exp(-norm / (5. * sqrt(n)))) + (exp(1) - exp(temp / n));
    return f;
}

static void
grad(
    nlp_float *g,
    const nlp_float *x,
    int n
)
{
    int i;
    nlp_float norm, temp;
    norm = 0.;
    temp = 0.;
    for (i = 0; i < n; ++i) {
        norm += x[i] * x[i];
        temp += cos(2. * PI * x[i]);
    }
    norm = sqrt(norm);
    for (i = 0; i < n; ++i) {
        g[i] = 4. * x[i] * exp(-norm / (5. * sqrt(n))) / (sqrt(n) * norm)
            + 2. * PI * exp(temp / n) / n * sin(2. * PI * x[i]);
    }
}

