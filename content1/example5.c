
#include <stdio.h>      /* standard I/O routines */
#include <pthread.h>    /* pthread funtions and data structures */
#include <stdlib.h>     /* rand() and srand() functions */

/* number of threads used to service requests */
#define NUM_HANDLER_THREADS 3

/* global mutex for our program. assignment initializes it. */
/* Note that we use a RECURSIVE mutex, since a handler
thread might try to lock it twice consecutively */
pthread_mutex_t request_mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;

/* global conditional variable for our program. assignment initializes it */
pthread_cond_t got_request = PTHREAD_COND_INITIALIZER;

/* number of pending requests, initially none */
int num_requests = 0;
/* CHANGE - definition of the flag variable */
int done_creating_requests = 0;     /* are we done creating new requests? */

/* format of a signal request */
struct request {
    int number;             /* number of the request */
    struct request* next;   /* pointer to next request, NULL if none. */
};

struct request* requests = NULL;        /* head of linked list of requests */
struct request* last_request = NULL;    /* pointer to last request */

/* Add a request to the requests list */
void add_request(int request_num, pthread_mutex_t* p_mutex, pthread_cond_t* p_cond_var)
{
    int rc;
    struct request* a_request;  /* pointer to newly added request */

    /* create structure with new request */
    a_request = (struct request*) malloc(sizeof(struct request));
    if(!a_request) { /* malloc failed?? */
        fprintf(stderr, "add_request: out of memory\n");
        exit(1);
    }
    a_request->number = request_num;
    a_request->next = NULL;

    /* lock the mutex */
    rc = pthread_mutex_lock(p_mutex);

    /* add new request to the end of the list, updating list pointers as required */
    if(num_requests == 0) { /* check list is empty */
        requests = a_request;
        last_request = a_request;
    }
    else {
        last_request->next = a_request;
        last_request = a_request;
    }

    /* increase total number of pending requests by one */
    num_requests++;

    /* unlock the mutex */
    rc = pthread_mutex_unlock(p_mutex);

    /* Signal the condition variable - there is a new request to handle */
    rc = pthread_cond_signal(p_cond_var);
}

/* Gets the first pending request from the requests list, removing it from the list. */
struct request* get_request(pthread_mutex_t* p_mutex)
{
    int rc;
    struct request * a_request; /* pointer to request */

    /* lock the mutex */
    rc = pthread_mutex_lock(p_mutex);

    if(num_requests > 0) {
        a_request = requests;
        requests = a_request->next;
        if(requests == NULL) { /* this was the last request on the list */
            last_request = NULL;
        }
        num_requests--; /* decrease the total number of pending requests */
    }
    else { /* requests list is empty */
        a_request = NULL;
    }

    /* unlock the mutex */
    rc = pthread_mutex_unlock(p_mutex);

    /* return the request to the caller */
    return a_request;
}

/* handle a single given request */
void handle_request(struct request* a_request, int thread_id)
{
    if(a_request) {
        printf("HANDLE_REQUEST: thread(%d) request_numner (%d)\n", thread_id, a_request->number);
        fflush(stdout);
    }
}

/* infinite loop of requests handling,
 algorithm: forever, if there are requests to handle, take the first and handle it. Then wait 
            on the given condition variable, and when it is signaled, re-do the loop.
            increases number of pending requests by one.
*/
void *handle_requests_loop(void * data)
{
    int rc;
    struct request* a_request;      /* pointer to a request */
    int thread_id = *((int *)data); /* thread identifying number */

    /* lock the mutex */
    rc  = pthread_mutex_lock(&request_mutex); // 1

    /* do forever .... */
    while(1) {

        if(num_requests > 0) {  /* a request is pending */
            a_request = get_request(&request_mutex);

            if(a_request) { /* got a request - handle it and free it */
                /* unlock the mutex */
                rc = pthread_mutex_unlock(&request_mutex);

                handle_request(a_request, thread_id);
                free(a_request);

                /* and lock the mutex again */
                rc = pthread_mutex_lock(&request_mutex);
            }
        }
        else {
            /* CHANGE 2- the thread checks the flag before waiting on the condition variable. */
            /* if no new requests are going to be generated, exit. */
            if(done_creating_requests) {
                rc = pthread_mutex_unlock(&request_mutex);
                printf("third(%id) exiting \n", thread_id);
                pthread_exit(NULL);
            }
            else {
                /* Thread entered into sleep */
            }

            /* wait for a request to arrive. note the mutex will be unlocked here,
               thus allowing other threads access to requests list.
            */
            rc = pthread_cond_wait(&got_request, &request_mutex);
            /* and after we return from pthread_cond_wait, the mutex is locked again,
               so we don't need to lock it ourselves
            */
        }
    }
}

/* Test the program */
int main(int argc, char *argv[]) {

    int i;
    int thr_id[NUM_HANDLER_THREADS];            /* thread IDs*/
    pthread_t p_threads[NUM_HANDLER_THREADS];   /* threads structures */
    struct timespec delay;                      /* used for wasting time */

    /* create the request-handling threads */
    for(i = 0; i < NUM_HANDLER_THREADS; i++) {
        thr_id[i] = i;
        pthread_create(&p_threads[i], NULL, handle_requests_loop, (void *)&thr_id[i]);
    }

    /* run a loop that generates requests */
    for (i = 0; i < 6; i++) {
        add_request(i, &request_mutex, &got_request);
        /*if(rand() > 3 * (RAND_MAX/4)) {
            delay.tv_sec = 0;
            delay.tv_nsec = 1000000;
            nanosleep(&delay, NULL);
        }*/
        /* pause the execution for a little bit */
        usleep(200000); //200ms 
    }

    /* now wait till there are no more requests to process */
    sleep(5);
    /*CHANGE 1 - the main thread modifies the flag to tell its handler threads 
                 no new requests will be generated. 
    notify our threads we are done creating requests.
    */
   {
       int rc;
       rc = pthread_mutex_lock(&request_mutex);
       done_creating_requests = 1;
       rc = pthread_cond_broadcast(&got_request);
       rc = pthread_mutex_unlock(&request_mutex);
   }

   /*CHANGE 3 - use pthread_join() to wait for all threads to terminate.*/
   /*now wait till there are no more requests to process */
    for(i = 0; i < NUM_HANDLER_THREADS; i++) {
        void *thr_retval;

        pthread_join(p_threads[i], &thr_retval);
    }

    printf("Glory, we are done.\n");
    return 0;
}

