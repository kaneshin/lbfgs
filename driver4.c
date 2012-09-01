/*
 * File:        driver4.c
 * Maintainer:  Shintaro Kaneko <kaneshin0120@gmail.com>
 * Last Change: 01-Sep-2012.
 *
 * Problem:     Sphere function
 */

#include "lbfgs/include/lbfgs.h"

#include <stdlib.h>

#include "lbfgs/include/nlp_component.h"
#include "lbfgs/linesearch/include/linesearch.h"
#include "lbfgs/linesearch/include/backtracking_wolfe.h"
#include "lbfgs/exmath/include/exmath.h"

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

int
main(int argc, char* argv[])
{
    int i, n;
    nlp_float *x;
    function_object func_obj;

    n = 10;
    x = (nlp_float *)malloc(n * sizeof(nlp_float));
    for (i = 0; i < n; ++i)
        x[i] = 1.1 * i;

    func_obj.func = func;
    func_obj.grad = grad;

    lbfgs(
        x,
        n,
        &func_obj,
        backtracking_wolfe,
        NULL
    );

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
    nlp_float f = 0.;
    for (i = 0; i < n; ++i)
        f += x[i] * x[i];
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
    for (i = 0; i < n; ++i)
        g[i] = 2. * x[i];
}

