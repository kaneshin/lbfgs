/*
 * File:        linesearch.h
 * Version:     0.1.0
 * Maintainer:  Shintaro Kaneko <kaneshin0120@gmail.com>
 * Last Change: 05-Sep-2012.
 */

#ifndef OPTIMIZATION_LINESEARCH_H
#define OPTIMIZATION_LINESEARCH_H

#include "../../include/nlp_component.h"

#define LINESEARCH_ARMIJO 1
#define LINESEARCH_WOLFE 2
#define LINESEARCH_STRONG_WOLFE 3
#define LINESEARCH_WOLFE_WITH_BACKTRACKING 4
#define LINESEARCH_STRONG_WOLFE_WITH_BACKTRACKING 5

#ifndef LINESEARCH_DEFAULT_METHOD
#define LINESEARCH_DEFAULT_METHOD LINESEARCH_WOLFE_WITH_BACKTRACKING
#endif /* LINESEARCH_DEFAULT_METHOD */

#if LINESEARCH_DEFAULT_METHOD == LINESEARCH_ARMIJO
#define __LINESEARCH_ARMIJO
#elif LINESEARCH_DEFAULT_METHOD == LINESEARCH_WOLFE
#define __LINESEARCH_WOLFE
#elif LINESEARCH_DEFAULT_METHOD == LINESEARCH_STRONG_WOLFE
#define __LINESEARCH_STRONG_WOLFE
#elif LINESEARCH_DEFAULT_METHOD == LINESEARCH_WOLFE_WITH_BACKTRACKING
#define __LINESEARCH_WOLFE
#define __LINESEARCH_WITH_BACKTRACKING
#elif LINESEARCH_DEFAULT_METHOD == LINESEARCH_STRONG_WOLFE_WITH_BACKTRACKING
#define __LINESEARCH_STRONG_WOLFE
#define __LINESEARCH_WITH_BACKTRACKING
#else
#warning "Use wolfe condition for linesearch."
#define __LINESEARCH_WOLFE
#define __LINESEARCH_WITH_BACKTRACKING
#endif

enum
{
    LINESEARCH_VALUE_NAN = -8,
    LINESEARCH_SATISFIED = 0,
    LINESEARCH_FAILED,
    LINESEARCH_STEPWIDTH_FAILED,
};

typedef struct _linesearch_parameter
{
    char *method;
    int maxit;
    nlp_float minw;
    nlp_float maxw;
    nlp_float width;
    nlp_float beta;
    nlp_float xi;
    nlp_float sigma;
#ifdef __LINESEARCH_WITH_BACKTRACKING
    nlp_float dec;
    nlp_float inc;
#endif /* __LINESEARCH_WITH_BACKTRACKING */
} linesearch_parameter;

void
default_linesearch_parameter(
    linesearch_parameter *parameter
);

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
);

#endif /* OPTIMIZATION_LINE_SEARCH_COMPONENT_H */

