#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#ifdef __MSVCRT__
#define sleep(x) _sleep((x) * 1000);
#endif // __MINGW64__

int WakeUp_Status = 0, Spirit_Status = 100, c_agmal = 0, c_iraj = 0;
bool agmal_sleep = false, iraj_sleep = false;

void* agmal(void* args)
{
    while (true)
    {
        if (c_iraj >= 3)
        {
            puts("Fitur Agmal Ayo Bangun disabled 10 s");
            agmal_sleep = true;
            sleep(10);
            agmal_sleep = false;
            c_iraj = 0;
        }
    }
    return NULL;
}

void* iraj(void* args)
{
    while (true)
    {
        if (c_agmal >= 3)
        {
            puts("Fitur Iraj Ayo Tidur disabled 10 s");
            iraj_sleep = true;
            sleep(10);
            iraj_sleep = false;
            c_agmal = 0;
        }
    }
    return NULL;
}

int main()
{
    pthread_t tid[2];
    int ret;
    char buf[256], c;

    if ((ret = pthread_create(&tid[0], NULL, &agmal, NULL)) != 0)
    {
        printf("Thread creation failed\n");
        exit(EXIT_FAILURE);
    }

    if ((ret = pthread_create(&tid[1], NULL, &iraj, NULL)) != 0)
    {
        printf("Thread creation failed\n");
        exit(EXIT_FAILURE);
    }

    while (true)
    {
        gets(buf);
        if (strcmp(buf, "Agmal Ayo Bangun") == 0)
        {
            if (!agmal_sleep)
            {
                WakeUp_Status += 15;
                c_agmal++;
            }
            else
            {
                puts("Fitur Agmal Ayo Bangun disabled 10 s");
            }
        }
        else if (strcmp(buf, "Iraj Ayo Tidur") == 0)
        {
            if (!iraj_sleep)
            {
                Spirit_Status -= 20;
                c_iraj++;
            }
            else
            {
                puts("Fitur Iraj Ayo Tidur disabled 10 s");
            }
        }
        else if (strcmp(buf, "All Status") == 0)
        {
            printf("Agmal WakeUp_Status = %d\nIraj Spirit_Status = %d\n", WakeUp_Status, Spirit_Status);
        }
        if ((WakeUp_Status >= 100) || (Spirit_Status <= 0))
            break;
    }

    if (WakeUp_Status >= 100)
        puts("Agmal Terbangun, mereka bangun pagi dan berolahraga");
    else
        puts("Iraj ikut tidur, dan bangun kesiangan bersama Agmal");
    return 0;
}
