#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <omp.h>
#include <time.h>

int makeResponse()
{
    srand(time(0));                    // seed for random generator
    return (rand() % (1 - 0 + 1)) + 0; // response 1 means OKAY, and 0 means NO
}

int Friend_A(char *date, char *time)
{
    int my_response = makeResponse();
    printf("=(A)= I'm Friend A I have %d id I received date & time.\t(Date:%s\t Time:%s)\n", omp_get_thread_num(), date, time);
    // printf("A: My_Resp: %d\n", my_response);
    return my_response;
}

int Friend_B(char *date, char *time)
{
    int my_response = makeResponse();
    printf("=(B)= I'm Friend B I have %d id I received date & time.\t(Date:%s\t Time:%s)\n", omp_get_thread_num(), date, time);
    // printf("B: My_Resp: %d\n", my_response);
    return my_response;
}

int Friend_C(char *date, char *time)
{
    int my_response = makeResponse();
    printf("=(C)= I'm Friend C I have %d id I received date & time.\t(Date:%s\t Time:%s)\n", omp_get_thread_num(), date, time);
    // printf("C: My_Resp: %d\n", my_response);
    return my_response;
}

int main()
{
    // printf("Im main:%d\n", omp_get_thread_num());
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char date[20];
    char time[20];
    snprintf(date, 20, "%d-%02d-%02d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
    snprintf(time, 20, "%02d:%02d:%02ds", tm.tm_hour, tm.tm_min, tm.tm_sec);
    // printf("============Date: %s\t Time:%s\n", date, time);
    int A_resp, B_resp, C_resp;
#pragma omp parallel num_threads(3)
#pragma omp single // Single thread create the task
    {
#pragma omp task // Execute the invitation task
        A_resp = Friend_A(date, time);
#pragma omp task // Execute the invitation task
        B_resp = Friend_B(date, time);
#pragma omp task // Execute the invitation task
        C_resp = Friend_C(date, time);
#pragma omp taskwait // wait until all the tasks have been finished
    }
    printf(".............. Responses; A=%d  B=%d  C=%d ..............\n", A_resp, B_resp, C_resp);
    if (A_resp == 1 && B_resp == 1 && C_resp == 1)
    {
        printf("******\tHoooooray! Lets Enjoy the Party.******\n");
    }
    else
    {
        printf("******\tOhhh NO! Lets call off this concesus and arrange it some time later.******\n");
    }
}