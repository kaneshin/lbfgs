/*
 * File:        linesearch.h
 * Version:     1.0.0
 * Maintainer:  Shintaro Kaneko <kaneshin0120@gmail.com>
 * Last Change: 06-Sep-2012.
 *
 * License:     The MIT License (MIT)
 *
 * Copyright (c) 2012 Shintaro Kaneko <kaneshin0120@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef OPTIMIZATION_LINESEARCH_H
#define OPTIMIZATION_LINESEARCH_H

#include "../../include/nlp_component.h"

/*
 * NOTE:
 *  If you select a condition for linesearch,
 *  It needs a defined variable such as pre-processor from below;
 *  It will use the wolfe condition with backtracking as default
 *  unless you define one.
 *
 * #define LINESEARCH_ARMIJO
 * #define LINESEARCH_WOLFE
 * #define LINESEARCH_STRONG_WOLFE
 * #define LINESEARCH_WOLFE_WITH_BACKTRACKING
 * #define LINESEARCH_STRONG_WOLFE_WITH_BACKTRACKING
 */

#define LINESEARCH_WOLFE_WITH_BACKTRACKING

#if defined(LINESEARCH_ARMIJO)
#define __LINESEARCH_ARMIJO
#elif defined(LINESEARCH_WOLFE)
#define __LINESEARCH_WOLFE
#elif defined(LINESEARCH_STRONG_WOLFE)
#define __LINESEARCH_STRONG_WOLFE
#elif defined(LINESEARCH_WOLFE_WITH_BACKTRACKING)
#define __LINESEARCH_WOLFE
#define __LINESEARCH_WITH_BACKTRACKING
#elif defined(LINESEARCH_STRONG_WOLFE_WITH_BACKTRACKING)
#define __LINESEARCH_STRONG_WOLFE
#define __LINESEARCH_WITH_BACKTRACKING
#else
#warning "Use Wolfe condition with Backtracking for Linesearch."
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
    char *condition;
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
