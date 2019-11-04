#include <stdio.h>       /* standard I/O routines                     */
#include <pthread.h>     /* pthread functions and data structures     */
#include <stdlib.h>      /* malloc() and free()                       */
#include <assert.h>      /* assert()                                  */

#include "requests_queue.h"   /* requests queue routines/structs      */
#include "handler_thread.h"   /* handler thread functions/structs     */

extern int done_creating_requests;   /* are we done creating new requests? */

/*
 * free the mutex, if it's locked.
 */
static void cleanup_free_mutex(void* a_mutex)
{
    pthread_mutex_t* p_mutex = (pthread_mutex_t*)a_mutex;

    if (p_mutex) {
        pthread_mutex_unlock(p_mutex);
	}
}

/*
 * handle/perform a single given request.
 * algorithm: prints a message stating that the given thread handled the given request.
 * input:     request pointer, id of calling thread.
 */
static void handle_request(struct request* a_request, int thread_id)
{
    if (a_request)
	{
		printf("Thread '%d' handled request '%d'\n", thread_id, a_request->number);
		fflush(stdout);
		/* delay some time, for some part of request execution */
		for (int i = 0; i<300000; i++)
			;
    }
}

/*
 * infinite loop of requests handling
 * ::forever, if there are requests to handle, take the first and handle it.
 * Then wait on the given condition variable, and when it is signaled, re-do the loop.
 * increases number of pending requests by one.
 */
void* handle_requests_loop(void* thread_params)
{
    int rc;								/* return code of pthreads functions.  */
    struct request* a_request;			/* pointer to a request.               */
    struct handler_thread_params *data; /* hadler thread's parameters */

    /* sanity check -make sure data isn't NULL */
    data = (struct handler_thread_params*)thread_params;
    assert(data);

    printf("#KA# Starting thread '%d'\n", data->thread_id);
    fflush(stdout);

    /* set my cancel state to 'enabled', and cancel type to 'defered'. */
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

    /* set thread cleanup handler, register  */
    pthread_cleanup_push(cleanup_free_mutex, (void*)data->request_mutex);

    /* lock the mutex, to access the requests list exclusively. */
    rc = pthread_mutex_lock(data->request_mutex);

#if 0
#ifdef DEBUG
    printf("thread '%d' after pthread_mutex_lock\n", data->thread_id);
    fflush(stdout);
#endif /* DEBUG */
#endif

    /* do forever.... */
    while (1) {

        int num_requests = get_requests_number(data->requests);

#if 0
#ifdef DEBUG
    	printf("thread '%d', num_requests =  %d\n",
	       data->thread_id, num_requests);
    	fflush(stdout);
#endif /* DEBUG */
#endif

		if (num_requests > 0) { /* a request is pending */
			a_request = get_request(data->requests);
			if (a_request) { /* got a request - handle the request and free it */
				handle_request(a_request, data->thread_id);
				free(a_request);
			}
		}
		else {
			/* the thread checks the flag before waiting            */
			/* on the condition variable.                           */
			/* if no new requests are going to be generated, exit.  */
			if (done_creating_requests) {
				pthread_mutex_unlock(data->request_mutex);
				printf("thread '%d' exiting\n", data->thread_id);
				fflush(stdout);
				pthread_exit(NULL);
			}
			/* wait for a request to arrive. note the mutex will be unlocked here, */
			/* thus allowing other threads access to requests list.                */
			rc = pthread_cond_wait(data->got_request, data->request_mutex);
			/* and after we return from pthread_cond_wait, the mutex  */
			/* is locked again, so we don't need to lock it ourselves */
		}
    }

    /* remove thread cleanup handler. never reached, but we must use */
    /* it here, according to pthread_cleanup_push's manual page.     */
    pthread_cleanup_pop(0);
}
