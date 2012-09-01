/*
 * File:        exmath.h
 * Version:     0.1.0
 * Maintainer:  Shintaro Kaneko <kaneshin0120@gmail.com>
 * Last Change: 01-Sep-2012.
 */

#ifndef EXMATH_H
#define EXMATH_H

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

double
dot_product(
    const double *x,
    const double *y,
    int n
);

double
infinity_norm(
    const double *x,
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
