/*
 * File:        nlp_component.c
 * Version:     0.1.0
 * Maintainer:  Shintaro Kaneko <kaneshin0120@gmail.com>
 * Last Change: 01-Sep-2012.
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
    if (ISNAN(component->fx))
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
        if (ISNAN(g[i]))
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
    if (ISNAN(component->fx))
        return NLP_VALUE_NAN;
    for (i = 0; i < n; ++i)
        if (ISNAN(g[i]))
            return NLP_VALUE_NAN;
    return NLP_SATISFIED;
}

