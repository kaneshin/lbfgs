/*
 * File:        backtracking_wolfe.c
 * Version:     0.1.0
 * Maintainer:  Shintaro Kaneko <kaneshin0120@gmail.com>
 * Last Change: 02-Sep-2012.
 */

#include "include/backtracking_wolfe.h"

#include "include/linesearch.h"
#include "../exmath/include/exmath.h"

void
default_backtracking_wolfe_parameter(
    linesearch_parameter *parameter
)
{
    parameter->maxit = 500;
    parameter->minw = 0.;
    parameter->maxw = 50.;
    parameter->width = .5;
    parameter->beta = 1.;
    parameter->xi = .001;
    parameter->sigma = .5;
    parameter->dec = .5;
    parameter->inc = 2.1;
}

int
backtracking_wolfe(
    nlp_float *work,
    const nlp_float *x,
    const nlp_float *g,
    const nlp_float *d,
    int n,
    evaluate_object *eval_obj,
    linesearch_parameter *parameter,
    nlp_component *component
)
{
    int iter, i;
    nlp_float width, beta, fx, gd, gd_next, *x_next, *g_next;

    x_next = work;
    g_next = x_next + n;

    width = parameter->width;
    beta = parameter->beta;
    if (NLP_VALUE_NAN == eval_obj->func(x, n, component))
        return LINESEARCH_VALUE_NAN;
    fx = component->fx;
    gd = dot_product(g, d, n);
    for (iter = 1; iter <= parameter->maxit; ++iter)
    {
        for (i = 0; i < n; ++i)
            x_next[i] = x[i] + beta * d[i];
        if (NLP_VALUE_NAN == eval_obj->func(x_next, n, component))
            return LINESEARCH_VALUE_NAN;
        if (component->fx <= fx + parameter->xi * beta * gd)
        {
            if (NLP_VALUE_NAN == eval_obj->grad(g_next, x_next, n, component))
                return LINESEARCH_VALUE_NAN;
            gd_next = dot_product(g_next, d, n);
            if (parameter->sigma * gd <= gd_next)
            {
                component->alpha = beta;
                return LINESEARCH_SATISFIED;
            }
            else
            {
                width = parameter->inc;
            }
        }
        else
        {
            width = parameter->dec;
        }
        beta *= width;
        if (beta < parameter->minw || beta > parameter->maxw)
            return LINESEARCH_STEPWIDTH_FAILED;
    }
    return LINESEARCH_FAILED;
}

