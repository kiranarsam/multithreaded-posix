
#include <stdio.h>
#include <pthread.h>

#define NUM_EMPLOYESS 2

pthread_mutex_t a_mutex = PTHREAD_MUTEX_INITIALIZER;

struct employee {
    int number;
    int id;
    char first_name[20];
    char last_name[20];
    char department[30];
    int room_number;
};

/* global variable - our employees array, with 2 employees */
struct employee employees[] = {
    {1, 12345678, "ABC", "CBA", "sky", 101},
    {2, 87654321, "DEF", "FED", "ivp", 202}
};

/* global variable - employee of the day */
struct employee employee_of_the_day;

/* function to copy one employee struct into another */
void copy_employee(struct employee *from, struct employee *to)
{
    int rc; /* contain mutex lock/unlock results */

    rc = pthread_mutex_lock(&a_mutex); /* lock the mutex, to assure exclusive access to a and b */

    to->number = from->number;
    to->id = from->id;
    strcpy(to->first_name, from->first_name);
    strcpy(to->last_name, from->last_name);
    strcpy(to->department, from->department);
    to->room_number = from->room_number;

    /* unlock the mutex */
    rc = pthread_mutex_unlock(&a_mutex);
}


/* function to be executed by the variable setting threads thread */
void * do_loop(void * data) 
{
    int my_num = *((int *) data);

    printf("In thread = %d\n", my_num);

    while(1) {
        /* set employee of the day to be the one with number 'my_num' */
        copy_employee(&employees[my_num-1], &employee_of_the_day);
    }
}

int main(int argc, char* argv[])
{
    int i;
    int thr_id1;
    int thr_id2;
    pthread_t  p_thread1;
    pthread_t  p_thread2;
    int num1 = 1;
    int num2 = 2;

    struct employee eotd;
    struct employee* worker;

    /* initialize employee of the day to first 1. */
    printf("In Main Thread, initalizing the employee of the day\n");
    copy_employee(&employees[0], &employee_of_the_day);

    thr_id1 = pthread_create(&p_thread1, NULL, do_loop, (void *)&num1);

    thr_id2 = pthread_create(&p_thread2, NULL, do_loop, (void *)&num2);

    for (i = 0; i < 3; i++) {
        /* save contents of 'employee of the day' to the local 'worker' */
        printf("In Main Thread\n");
        copy_employee(&employee_of_the_day, &eotd);
        worker = &employees[eotd.number - 1];

        if(eotd.id != worker->id) {
            printf("mismatching 'id', %d != %d (loop '%d')\n", eotd.id, worker->id, i);
            exit(0);
        }

        if(strcmp(eotd.last_name, worker->last_name) != 0) {
            printf("mismatching 'last_name', %s != %s (loop '%d')\n", eotd.last_name, worker->last_name, i);
            exit(0);
        }

        if(strcmp(eotd.first_name, worker->first_name) != 0) {
            printf("mismatching 'first_name', %s != %s (loop '%d')\n", eotd.first_name, worker->first_name, i);
            exit(0);
        }

        if(strcmp(eotd.department, worker->department) != 0) {
            printf("mismatching 'department', %s != %s (loop '%d')\n", eotd.department, worker->department, i);
            exit(0);
        }

        if(eotd.room_number != worker->room_number) {
            printf("mismatching 'room_number', %d != %d (loop '%d')\n", eotd.room_number, worker->room_number, i);
            exit(0);
        }
    }

    printf("Glory, employees contents was always consistent\n");

    return 0;
}