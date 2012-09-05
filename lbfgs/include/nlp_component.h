/*
 * File:        nlp_component.h
 * Version:     0.1.0
 * Maintainer:  Shintaro Kaneko <kaneshin0120@gmail.com>
 * Last Change: 05-Sep-2012.
 */

#ifndef OPTIMIZATION_NLP_COMPONENT_H
#define OPTIMIZATION_NLP_COMPONENT_H

enum {
    NLP_PRECISE_FLOAT_32_BIT,
    NLP_PRECISE_FLOAT_64_BIT,
};

#ifndef NLP_DEFAULT_FLOAT
#define NLP_DEFAULT_FLOAT NLP_PRECISE_FLOAT_64_BIT
#endif /* NLP_DEFAULT_FLOAT */

#if NLP_DEFAULT_FLOAT == NLP_PRECISE_FLOAT_64_BIT
    typedef double nlp_float;
#elif NLP_DEFAULT_FLOAT == NLP_PRECISE_FLOAT_32_BIT
    typedef float nlp_float;
#else
#warning "Use Single-precision floating-point format (float: 32 bits) for nlp_float."
    typedef float nlp_float;
#endif

enum nlp_status {
    NLP_VALUE_NAN = -8,
    NLP_SATISFIED = 0,
};

typedef struct _function_object {
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

typedef struct _nlp_component {
    char *method;
    int iter_f;
    int iter_g;
    nlp_float fx;
    nlp_float alpha;
    function_object *func_obj;
} nlp_component;

typedef struct _evaluate_object {
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

