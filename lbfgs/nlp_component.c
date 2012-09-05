/*
 * File:        nlp_component.c
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

#include "include/nlp_component.h"

#include "exmath/include/exmath.h"

static int
func(
    const nlp_float *x,
    int n,
    nlp_component *component
);

static int
grad(
    nlp_float *g,
    const nlp_float *x,
    int n,
    nlp_component *component
);

static int
func_grad(
    nlp_float *g,
    const nlp_float *x,
    int n,
    nlp_component *component
);

void
initialize_nlp_component(
    char *method,
    function_object *func_obj,
    evaluate_object *eval_obj,
    nlp_component *component
)
{
    component->method = method;
    component->iter_f = 0;
    component->iter_g = 0;
    component->fx = 0.;
    component->alpha = 0.;
    component->func_obj = func_obj;
    eval_obj->func = func;
    eval_obj->grad = grad;
    eval_obj->func_grad = func_grad;
}

static int
func(
    const nlp_float *x,
    int n,
    nlp_component *component
)
{
    component->fx = component->func_obj->func(x, n);
    ++(component->iter_f);
    if (isnan(component->fx))
        return NLP_VALUE_NAN;
    return NLP_SATISFIED;
}

static int
grad(
    nlp_float *g,
    const nlp_float *x,
    int n,
    nlp_component *component
)
{
    int i;
    component->func_obj->grad(g, x, n);
    ++(component->iter_g);
    for (i = 0; i < n; ++i)
        if (isnan(g[i]))
            return NLP_VALUE_NAN;
    return NLP_SATISFIED;
}

static int
func_grad(
    nlp_float *g,
    const nlp_float *x,
    int n,
    nlp_component *component
)
{
    int i;
    component->fx = component->func_obj->func(x, n);
    ++(component->iter_f);
    component->func_obj->grad(g, x, n);
    ++(component->iter_g);
    if (isnan(component->fx))
        return NLP_VALUE_NAN;
    for (i = 0; i < n; ++i)
        if (isnan(g[i]))
            return NLP_VALUE_NAN;
    return NLP_SATISFIED;
}
