/*
 * File:        linesearch.c
 * Version:     0.1.0
 * Maintainer:  Shintaro Kaneko <kaneshin0120@gmail.com>
 * Last Change: 05-Sep-2012.
 */

#include "include/linesearch.h"

#include <string.h>

#include "../exmath/include/exmath.h"

void
default_linesearch_parameter(
    linesearch_parameter *parameter
)
{
#ifdef __LINESEARCH_ARMIJO
    parameter->method = "Armijo";
#endif /* __LINESEARCH_ARMIJO */
#ifdef __LINESEARCH_WOLFE
#if !defined(__LINESEARCH_WITH_BACKTRACKING)
    parameter->method = "Wolfe";
#else
    parameter->method = "Wolfe with Backtracking";
#endif /* __LINESEARCH_WITH_BACKTRACKING */
#endif /* __LINESEARCH_WOLFE */
#ifdef __LINESEARCH_STRONG_WOLFE
#if !defined(__LINESEARCH_WITH_BACKTRACKING)
    parameter->method = "Strong Wolfe";
#else
    parameter->method = "Strong Wolfe with Backtracking";
#endif /* __LINESEARCH_WITH_BACKTRACKING */
#endif /* __LINESEARCH_STRONG_WOLFE */
    parameter->maxit = 500;
    parameter->minw = 0.;
    parameter->maxw = 50.;
    parameter->width = .5;
    parameter->beta = 1.;
#ifdef __LINESEARCH_ARMIJO
    parameter->xi = .5;
#endif /* __LINESEARCH_ARMIJO */
#ifndef __LINESEARCH_ARMIJO
    parameter->xi = .001;
#endif /* __LINESEARCH_ARMIJO */
    parameter->sigma = .5;
#ifdef __LINESEARCH_WITH_BACKTRACKING
    parameter->dec = .5;
    parameter->inc = 2.1;
#endif /* __LINESEARCH_WITH_BACKTRACKING */
}

int
linesearch(
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
    nlp_float width, beta, fx, gd, gd_next, sgd, *x_next, *g_next;

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
#ifdef __LINESEARCH_ARMIJO
            component->alpha = beta;
            return LINESEARCH_SATISFIED;
#endif /* __LINESEARCH_ARMIJO */
            if (NLP_VALUE_NAN == eval_obj->grad(g_next, x_next, n, component))
                return LINESEARCH_VALUE_NAN;
            sgd = parameter->sigma * gd;
            gd_next = dot_product(g_next, d, n);
            if (sgd <= gd_next)
            {
#ifdef __LINESEARCH_WOLFE
                component->alpha = beta;
                return LINESEARCH_SATISFIED;
#endif /* __LINESEARCH_WOLFE */
                if (FABS(gd_next) <= -sgd)
                {
#ifdef __LINESEARCH_STRONG_WOLFE
                    component->alpha = beta;
                    return LINESEARCH_SATISFIED;
#endif /* __LINESEARCH_STRONG_WOLFE */
                }
#ifdef __LINESEARCH_WITH_BACKTRACKING
                else
                {
                    width = parameter->dec;
                }
#endif /* __LINESEARCH_WITH_BACKTRACKING */
            }
#ifdef __LINESEARCH_WITH_BACKTRACKING
            else
            {
                width = parameter->inc;
            }
#endif /* __LINESEARCH_WITH_BACKTRACKING */
        }
#ifdef __LINESEARCH_WITH_BACKTRACKING
        else
        {
            width = parameter->dec;
        }
#endif /* __LINESEARCH_WITH_BACKTRACKING */
        beta *= width;
        if (beta < parameter->minw || beta > parameter->maxw)
            return LINESEARCH_STEPWIDTH_FAILED;
    }
    return LINESEARCH_FAILED;
}

