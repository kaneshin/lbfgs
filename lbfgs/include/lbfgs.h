/*
 * File:        lbfgs.h
 * Version:     0.9.0
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

#ifndef OPTIMIZATION_LBFGS_H
#define OPTIMIZATION_LBFGS_H

#include "nlp_component.h"
#include "../linesearch/include/linesearch.h"

enum lbfgs_status
{
    LBFGS_VALUE_NAN = -8,
    LBFGS_VALUE_INF,
    LBFGS_NO_FUNCTION,
    LBFGS_SATISFIED = 0,
    LBFGS_CONVERGENCE,
    LBFGS_NO_CONVERGENCE,
    LBFGS_LINESEARCH_NO_PARAMETER,
    LBFGS_LINESEARCH_FAILED,
    LBFGS_DIRECTION_SEARCH_FAILED,
    LBFGS_OUT_OF_MEMORY,
};

typedef struct _lbfgs_parameter
{
    nlp_float tolerance;
    int upper_iter;
} lbfgs_parameter;

void
default_lbfgs_parameter(
    lbfgs_parameter *parameter
);

int
lbfgs(
    nlp_float *x,
    int n,
    function_object *func_obj,
    linesearch_parameter *ls_parameter,
    lbfgs_parameter *parameter
);

#endif /* OPTIMIZATION_LBFGS_H */
