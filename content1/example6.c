
#include <stdlib.h>     /* malloc() and free() */
#include <assert.h>     /* assert() */

#include "requests_queue.h"     /* requests queue functions and structs */

/* create a requests queue */
struct requests_queue* init_requests_queue(pthread_mutex_t *p_mutex, pthread_cond_t *p_cond_var)
{
    struct requests_queue* queue = 
            (struct requests_queue *)malloc(sizeof(struct requests_queue));
    
    if(!queue) {
        fprintf(stderr, "out of memory. exiting\n");
        exit(1);
    }

    /* initialize queue */
    
}

