#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
struct handler
{
    int x;
    int y;
};
int main(int argc, char *argv[])
{
    int count, final = 0;
    // Used to store two ends of first pipe
    printf("Please enter the number of child processes: "); // Input value
    scanf("%d", &count);
    int n = count * 2;
    int *arr = (int *)malloc((n) * sizeof(int));
    printf("The element of arrays are: ");
    //
    // Initialising array
    //
    for (int i = 0; i < n; i++)
    {
        arr[i] = rand() % 11;
        printf("%d ", arr[i]);
    }
    printf("\n===========================\n");
    int fdA[2], fdB[2];
    // Creating pipes descriptors

    //
    //      Creating childs
    //
    int resp, j = 0;
    for (int i = 0; i < count; i++) // loop will run n times (n=5)
    {
        pipe(fdA);
        pipe(fdB);
        if (fork() > 0)
        {
            // Parent
            struct handler hndlr;
            resp = 0;
            hndlr.x = arr[j];
            hndlr.y = arr[j + 1];
            // printf("Sending in 1: %d\n", hndlr.x);
            // printf("Sending in 2: %d\n", hndlr.y);
            close(fdA[0]); // Close reading end of first pipe
            write(fdA[1], &hndlr, sizeof(hndlr));
            close(fdA[1]); // close writing
            wait(NULL);
            close(fdB[1]);                     // Close writing end of third pipe
            read(fdB[0], &resp, sizeof(resp)); // reading from child
            // printf("------------Returned Sum: %d\n", resp);
            final += resp; // aggregated sum
            close(fdB[0]);
            j += 2;
        }
        else
        {
            // child
            struct handler temp;
            int sum = 0;
            printf("Parent: %d created child: %d\n", getppid(), getpid());
            //////  Reading from pipes
            close(fdA[1]); // Close writing end of first pipe
            read(fdA[0], &temp, sizeof(temp));
            // printf("Received in 1: %d\n", temp.x);
            // printf("Received in 2: %d\n", temp.y);
            sum = temp.x + temp.y;
            printf("Child: %d Local Sum: %d+%d=%d\n", getpid(), temp.x, temp.y, sum);
            fflush(NULL);
            close(fdA[0]);                    // close writing
            close(fdB[0]);                    // Close reading end of third pipe
            write(fdB[1], &sum, sizeof(sum)); // writing to child
            close(fdB[1]);

            exit(0);
        }
    }
    printf("Parent: %d Total Sum: %d\n", getpid(), final);
    return 0;
}
