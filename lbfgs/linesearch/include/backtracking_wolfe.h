/*
 * File:        backtracking_wolfe.h
 * Version:     0.1.0
 * Maintainer:  Shintaro Kaneko <kaneshin0120@gmail.com>
 * Last Change: 01-Sep-2012.
 */

#ifndef OPTIMIZATION_BACKTRACKING_WOLFE_H
#define OPTIMIZATION_BACKTRACKING_WOLFE_H

#include "../../include/nlp_component.h"

int
backtracking_wolfe(
    nlp_float *work,
    const nlp_float *x,
    const nlp_float *g,
    const nlp_float *d,
    int n,
    evaluate_object *eval_obj,
    nlp_component *component
);

#endif /* OPTIMIZATION_BACKTRACKING_WOLFE_H */

