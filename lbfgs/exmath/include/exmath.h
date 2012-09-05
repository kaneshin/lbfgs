/*
 * File:        exmath.h
 * Version:     0.1.0
 * Maintainer:  Shintaro Kaneko <kaneshin0120@gmail.com>
 * Last Change: 05-Sep-2012.
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

#ifdef ISINF
#undef ISINF
#endif /* ISINF */
#define ISINF(a) (((INF) == (a)) || ((-INF) == (a)))

#ifdef ISNAN
#undef ISNAN
#endif /* ISNAN */
#define ISNAN(a) ((a) != (a))

#ifdef FABS
#undef FABS
#endif /* FABS */
#define FABS(a) (((a) > (0)) ? (a) : (-a))

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
