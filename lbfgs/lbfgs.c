/*
 * File:        lbfgs.c
 * Version:     0.1.0
 * Maintainer:  Shintaro Kaneko <kaneshin0120@gmail.com>
 * Last Change: 05-Sep-2012.
 */

#include "include/lbfgs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "exmath/include/exmath.h"

#ifndef LBFGS_PRINT_DEBUG
#define LBFGS_PRINT_DEBUG
#endif /* LBFGS_PRINT_DEBUG */

#ifndef LBFGS_PRINT
#define LBFGS_PRINT
#endif /* LBFGS_PRINT */

#ifndef LBFGS_PRINT_VERBOSE
#define LBFGS_PRINT_VERBOSE
#endif /* LBFGS_PRINT_VERBOSE */

#ifdef LBFGS_PRINT
#ifndef LBFGS_PRINT_ITER
#define LBFGS_PRINT_ITER
#endif /* LBFGS_PRINT_ITER */
#ifndef LBFGS_PRINT_RESULT
#define LBFGS_PRINT_RESULT
#endif /* LBFGS_PRINT_RESULT */
#endif /* LBFGS_PRINT */

#ifdef LBFGS_PRINT_VERBOSE
#ifndef LBFGS_PRINT_ITER_VERBOSE
#define LBFGS_PRINT_ITER_VERBOSE
#endif /* LBFGS_PRINT_ITER_VERBOSE */
#ifndef LBFGS_PRINT_RESULT_VERBOSE
#define LBFGS_PRINT_RESULT_VERBOSE
#endif /* LBFGS_PRINT_RESULT_VERBOSE */
#endif /* LBFGS_PRINT_VERBOSE */

static void
default_lbfgs_parameter(
    lbfgs_parameter *parameter
);

static int
convergence_criterion(
    nlp_float g_norm,
    lbfgs_parameter *parameter
);

static int
lbfgs_direction_search_phase_1(
    nlp_float *p,
    nlp_float *gamma,
    const nlp_float *s,
    const nlp_float *y,
    int n
);

static int
lbfgs_direction_search_phase_2(
    nlp_float *q,
    const nlp_float *s,
    const nlp_float *y,
    nlp_float gamma,
    int n
);

static int
lbfgs_direction_search(
    nlp_float *d,
    const nlp_float *g,
    nlp_float **s,
    nlp_float **y,
    nlp_float *gamma,
    int n,
    int hist_nr
);

static void
lbfgs_print_iter(
    int iter,
    nlp_float g_norm,
#ifdef LBFGS_PRINT_ITER_VERBOSE
    const nlp_float *x,
    const nlp_float *g,
    int n,
#endif /* LBFGS_PRINT_ITER_VERBOSE */
    nlp_component *component
);

static void
lbfgs_print_result(
    int status,
    int iter,
#ifdef LBFGS_PRINT_RESULT_VERBOSE
    const nlp_float *x,
    const nlp_float *g,
    int n,
#endif
    nlp_component *component
);

static void
lbfgs_print_verbose(
    nlp_float fx,
    const nlp_float *x,
    const nlp_float *g,
    int n
);


int
lbfgs(
    nlp_float *x,
    int n,
    function_object *func_obj,
    linesearch_t linesearch,
    linesearch_parameter *ls_parameter,
    lbfgs_parameter *parameter
)
{
    char            *method = "Limited Memory BFGS";
    int             status, iter, i, k, memblock_size, storage_nr, work_nr, hist_nr;
    nlp_float       g_norm, *_x, *storage, *work, *d, *g, *x_prev, *g_prev,
                    **s, **y, *gamma;
    nlp_component   component;
    evaluate_object eval_obj;
    lbfgs_parameter _parameter;

    memblock_size = n * sizeof(nlp_float);
    storage_nr = 4;
    work_nr = 2;
    hist_nr = 10;

    if (NULL == x)
    {
        _x = (nlp_float *)malloc(memblock_size);
        if (NULL != _x)
            for (i = 0; i < n; ++i)
                _x[i] = 0.;
        else
            status = LBFGS_OUT_OF_MEMORY;
        x = _x;
    }
    else
    {
        _x = NULL;
    }

    s = (nlp_float **)malloc(hist_nr * sizeof(nlp_float *));
    if (NULL != s)
    {
        *s = (nlp_float *)malloc(hist_nr * memblock_size);
        if (NULL != *s)
            for (i = 1; i < hist_nr; ++i)
                s[i] = s[i - 1] + n;
        else
            status = LBFGS_OUT_OF_MEMORY;
    }
    else
    {
        status = LBFGS_OUT_OF_MEMORY;
    }
    y = (nlp_float **)malloc(hist_nr * sizeof(nlp_float *));
    if (NULL != y)
    {
        *y = (nlp_float *)malloc(hist_nr * memblock_size);
        if (NULL != *y)
            for (i = 1; i < hist_nr; ++i)
                y[i] = y[i - 1] + n;
        else
            status = LBFGS_OUT_OF_MEMORY;
    }
    else
    {
        status = LBFGS_OUT_OF_MEMORY;
    }
    if (LBFGS_OUT_OF_MEMORY != status)
        for (k = 0; k < hist_nr ; ++k)
            for (i = 0; i < n; ++i)
                s[k][i] = y[k][i] = 0.;

    /* allocate memory to storage for d, g, x_prev, g_prev, s and y */
    storage = (nlp_float *)malloc(storage_nr * memblock_size);
    work = (nlp_float *)malloc(work_nr * memblock_size);
    gamma = (nlp_float *)malloc(hist_nr * sizeof(nlp_float));

    if (NULL == storage || NULL == work || NULL == gamma)
        status = LBFGS_OUT_OF_MEMORY;

    /* make sure that f and gf of the problem exist */
    if (NULL == func_obj->func || NULL == func_obj->grad)
        status = LBFGS_NO_FUNCTION;

    switch (status)
    {
        case LBFGS_OUT_OF_MEMORY:
        case LBFGS_NO_FUNCTION:
            goto lbfgs_exit;
            break;
        default:
            break;
    }

    d = storage;
    g = d + n;
    x_prev = g + n;
    g_prev = x_prev + n;

    /* set the component of Non-Linear Programming */
    initialize_nlp_component(method, func_obj, &eval_obj, &component);

    /* set the parameter of Limited Memory BFGS */
    if (NULL == parameter)
        parameter = &_parameter;
    default_lbfgs_parameter(parameter);

    /* parameter of Line Search */
    if (NULL == ls_parameter)
    {
        status = LBFGS_LINESEARCH_NO_PARAMETER;
        goto lbfgs_exit;
    }

    /*
     * start to compute for solving this problem
     */
    if (NLP_VALUE_NAN == eval_obj.grad(g, x, n, &component))
    {
        status = LBFGS_VALUE_NAN;
        goto lbfgs_exit;
    }
    g_norm = infinity_norm(g, n);
    if (LBFGS_CONVERGENCE == convergence_criterion(g_norm, parameter))
    {
        iter = 0;
        status =  LBFGS_SATISFIED;
        goto lbfgs_exit;
    }
    for (iter = 1; iter <= parameter->upper_iter; ++iter)
    {
        /* INFO:
         *  If you'd like to update vector, use memcpy from string.h.
         *  Don't use For statement. It's slow.
         * update x and g to new step */
        memcpy(x_prev, x, memblock_size);
        memcpy(g_prev, g, memblock_size);

#ifdef LBFGS_PRINT_VERBOSE
        lbfgs_print_verbose(component.fx, x, g, n);
#endif /*LBFGS_PRINT_VERBOSE */

        /* compute a direction */
        status = lbfgs_direction_search(d, g, s, y, gamma, n, hist_nr);
        if (status)
            goto lbfgs_exit;
        /* compute step width with a line search algorithm */
        status = linesearch(work, x, g, d, n, &eval_obj, ls_parameter, &component);
        switch (status)
        {
            case LINESEARCH_VALUE_NAN:
                status = LBFGS_VALUE_NAN;
                goto lbfgs_exit;
            case LINESEARCH_FAILED:
            case LINESEARCH_STEPWIDTH_FAILED:
                status = LBFGS_LINESEARCH_FAILED;
                goto lbfgs_exit;
            default:
                break;
        }

        /* update x = x_prev + alpha * d */
        for (i = 0; i < n; ++i)
            x[i] = x_prev[i] + component.alpha * d[i];

        /* update g = grad(x) */
        if (NLP_VALUE_NAN == eval_obj.grad(g, x, n, &component))
        {
            status = LBFGS_VALUE_NAN;
            goto lbfgs_exit;
        }

        /* compute g_norm */
        g_norm = infinity_norm(g, n);
#ifdef LBFGS_PRINT_ITER
        lbfgs_print_iter(
            iter,
            g_norm,
#ifdef LBFGS_PRINT_ITER_VERBOSE
            x,
            g,
            n,
#endif /* LBFGS_PRINT_ITER_VERBOSE */
            &component
        );
#endif
        if (LBFGS_CONVERGENCE == convergence_criterion(g_norm, parameter))
        {
            status = LBFGS_SATISFIED;
            goto lbfgs_exit;
        }

        // shift values
        memmove(s[0], s[1], (hist_nr - 1) * memblock_size);
        memmove(y[0], y[1], (hist_nr - 1) * memblock_size);
        for (i = 0; i < n; ++i)
        {
            s[hist_nr - 1][i] = x[i] - x_prev[i];
            y[hist_nr - 1][i] = g[i] - g_prev[i];
        }
    }
    status = LBFGS_NO_CONVERGENCE;

lbfgs_exit:

#ifdef LBFGS_PRINT_RESULT
    lbfgs_print_result(
        status,
        iter,
#ifdef LBFGS_PRINT_RESULT_VERBOSE
        x,
        g,
        n,
#endif
        &component
    );
#endif

    free_vec(_x);
    free_vec(storage);
    free_vec(work);
    free_vec(gamma);

    free_mat((void **)s);
    free_mat((void **)y);

    return status;
}

static void
default_lbfgs_parameter(
    lbfgs_parameter *parameter
)
{
    parameter->tolerance = 1.e-8;
    parameter->upper_iter = 3000;
}

static int
convergence_criterion(
    nlp_float g_norm,
    lbfgs_parameter *parameter
)
{
    // TODO: nslow brabrabra or something
    if (g_norm < parameter->tolerance)
        return LBFGS_CONVERGENCE;
    return LBFGS_NO_CONVERGENCE;
}

static int
lbfgs_direction_search_phase_1(
    nlp_float *p,
    nlp_float *gamma,
    const nlp_float *s,
    const nlp_float *y,
    int n
)
{
    int i;
    nlp_float _gamma, tau;

    tau = dot_product(s, y, n);
    _gamma = dot_product(s, p, n);
    if (ISNAN(tau) || ISNAN(_gamma))
        return LBFGS_VALUE_NAN;
    if (ISINF(tau) || ISINF(_gamma))
        return LBFGS_VALUE_INF;
    if (tau > 0)
    {
        *gamma = _gamma = _gamma / tau;
        for (i = 0; i < n; ++i)
            p[i] -= _gamma * y[i];
    }
    return LBFGS_SATISFIED;
}

static int
lbfgs_direction_search_phase_2(
    nlp_float *q,
    const nlp_float *s,
    const nlp_float *y,
    nlp_float gamma,
    int n
)
{
    int i;
    nlp_float beta, tau;

    tau = dot_product(s, y, n);
    beta = dot_product(y, q, n);
    if (ISNAN(tau) || ISNAN(beta))
        return LBFGS_VALUE_NAN;
    if (ISINF(tau) || ISINF(beta))
        return LBFGS_VALUE_INF;
    if (tau > 0)
    {
        beta /= tau;
        for (i = 0; i < n; ++i)
            q[i] += (gamma - beta) * s[i];
    }
    return LBFGS_SATISFIED;
}

static int
lbfgs_direction_search(
    nlp_float *d,
    const nlp_float *g,
    nlp_float **s,
    nlp_float **y,
    nlp_float *gamma,
    int n,
    int hist_nr
)
{
    int i, status;
    for (i = 0; i < n; ++i)
        d[i] = g[i];
    for (i = hist_nr - 1; i >= 0; --i)
    {
        status = lbfgs_direction_search_phase_1(d, gamma + i, s[i], y[i], n);
        if (status)
            return status;
    }
    for (i = 0; i < hist_nr; ++i)
    {
        status = lbfgs_direction_search_phase_2(d, s[i], y[i], gamma[i], n);
        if (status)
            return status;
    }
    for (i = 0; i < n; ++i)
        d[i] = -d[i];
    return status;
}

static void
lbfgs_print_iter(
    int iter,
    nlp_float g_norm,
#ifdef LBFGS_PRINT_ITER_VERBOSE
    const nlp_float *x,
    const nlp_float *g,
    int n,
#endif /* LBFGS_PRINT_ITER_VERBOSE */
    nlp_component *component
)
{
    int i;
    printf("\n=== iteration: %-7d ================================\n", iter);
    printf("||gf(x_k+1)||_infinity = %e\n", g_norm);
    printf("=======================================================\n");
#ifdef LBFGS_PRINT_ITER_VERBOSE
    printf("\n-- VERBOSE --------------------------------------------\n");
    printf("step width parameter:  \t%13.6e\n", component->alpha);
    lbfgs_print_verbose(component->fx, x, g, n);
    printf("-------------------------------------------------------\n");
#endif /* LBFGS_PRINT_ITER_VERBOSE */
}

static void
lbfgs_print_result(
    int status,
    int iter,
#ifdef LBFGS_PRINT_RESULT_VERBOSE
    const nlp_float *x,
    const nlp_float *g,
    int n,
#endif /* LBFGS_PRINT_RESULT_VERBOSE */
    nlp_component *component
)
{
    printf("\n=== %s Result ========================\n", component->method);
    printf("%s Status: %3d\n", component->method, status);
    switch (status)
    {
        case LBFGS_VALUE_NAN:
            printf("Error: Value is Not a Number\n");
            break;
        case LBFGS_NO_FUNCTION:
            printf("Error: Function Object is undefined\n");
            break;
        case LBFGS_SATISFIED:
            printf("Satisfied: %s Method is finished\n", component->method);
            break;
        case LBFGS_OUT_OF_MEMORY:
            printf("Failed: Out of memory\n");
            break;
        case LBFGS_NO_CONVERGENCE:
            printf("Failed: %s doesn't converge\n", component->method);
            break;
        case LBFGS_LINESEARCH_FAILED:
            printf("Failed: Line Search is failed\n");
            break;
        case LBFGS_DIRECTION_SEARCH_FAILED:
            printf("Failed: Direction Search is failed\n");
            break;
        default:
            printf("Failed: Something happened\n");
            break;
    }
    if (LBFGS_SATISFIED <= status)
    {
        printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
        printf("iterations:          %12d\n", iter);
        printf("function evaluations:%12d\n", component->iter_f);
        printf("gradient evaluations:%12d\n", component->iter_g);
        printf("=======================================================\n");
#ifdef LBFGS_PRINT_RESULT_VERBOSE
        printf("\n-- VERBOSE --------------------------------------------\n");
        lbfgs_print_verbose(component->fx, x, g, n);
        printf("-------------------------------------------------------\n");
#endif /* LBFGS_PRINT_RESULT_VERBOSE */
    }
}

static void
lbfgs_print_verbose(
    nlp_float fx,
    const nlp_float *x,
    const nlp_float *g,
    int n
)
{
    int i;
    printf("function value:      \t%13.6e\n", fx);
    printf("\nx values\n");
    for (i = 0; i < n; ++i)
        printf(" x_%-8d = %12.6e\n", i + 1 , x[i]);
    printf("\ngradient values\n");
    for (i = 0; i < n; ++i)
        printf(" g_%-8d = %12.6e\n", i + 1 , g[i]);
}

