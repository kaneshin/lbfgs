/*
 * File:        nlp_component.h
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

#ifndef OPTIMIZATION_NLP_COMPONENT_H
#define OPTIMIZATION_NLP_COMPONENT_H

/*
 * #define NLP_PRECISE_FLOAT_32_BIT
 * #define NLP_PRECISE_FLOAT_64_BIT
 */

#define NLP_PRECISE_FLOAT_64_BIT

#if defined(NLP_PRECISE_FLOAT_64_BIT)
    typedef double nlp_float;
#elif defined(NLP_PRECISE_FLOAT_32_BIT)
    typedef float nlp_float;
#else
#warning "Use Single-precision floating-point format (float: 32 bits) for nlp_float."
    typedef double nlp_float;
#endif

enum nlp_status
{
    NLP_VALUE_NAN = -8,
    NLP_SATISFIED = 0,
};

typedef struct _function_object
{
    nlp_float (*func)(
        const nlp_float *,
        int
    );
    void (*grad)(
        nlp_float *,
        const nlp_float *,
        int
    );
} function_object;

typedef struct _nlp_component
{
    char *method;
    int iter_f;
    int iter_g;
    nlp_float fx;
    nlp_float alpha;
    function_object *func_obj;
} nlp_component;

typedef struct _evaluate_object
{
    int (*func)(
        const nlp_float *,
        int,
        nlp_component *
    );
    int (*grad)(
        nlp_float *,
        const nlp_float *,
        int,
        nlp_component *
    );
    int (*func_grad)(
        nlp_float *,
        const nlp_float *,
        int,
        nlp_component *
    );
    function_object *func_obj;
} evaluate_object;

void
initialize_nlp_component(
    char *method_name,
    function_object *func_obj,
    evaluate_object *eval_obj,
    nlp_component *component
);

#endif /* OPTIMIZATION_NLP_COMPONENT_H */
