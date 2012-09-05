/*
 * File:        exmath.h
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

#ifndef EXMATH_H
#define EXMATH_H

#include "../../include/nlp_component.h"

#ifdef INF
#undef INF
#endif /* INF */
#define INF (1./0.)

#ifdef NAN
#undef NAN
#endif /* NAN */
#define NAN (0./0.)

/*
 * NOTE:
 *  math.h may NOT have functions like this;
 *      - isinf
 *      - isnan
 *      - fabs
 */
#ifdef isinf
#undef isinf
#endif /* isinf */
#define isinf(a) (((INF) == (a)) || ((-INF) == (a)))

#ifdef isnan
#undef isnan
#endif /* isnan */
#define isnan(a) ((a) != (a))

#ifdef fabs
#undef fabs
#endif /* fabs */
#define fabs(a) (((a) > (0)) ? (a) : (-a))

nlp_float
dot_product(
    const nlp_float *x,
    const nlp_float *y,
    int n
);

nlp_float
infinity_norm(
    const nlp_float *x,
    int n
);

void
free_vec(
    void *mem
);

void
free_mat(
    void **mem
);

#endif /* EXMATH_H */
