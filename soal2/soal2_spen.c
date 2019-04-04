#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <netinet/in.h>
#include <pthread.h>
#define BUF_SIZE 256
#define PORT 4747
#define SHMKEY 11037

void* print_stok(void *args)
{
    int stok = *(int*)args;
    while (1)
    {
        stok = *(int*)args;
        printf("%d\n", stok);
        sleep(5);
    }
    return NULL;
}

int main()
{
    //Variabel
    int serv_sock, cli_sock, n;
    char buffer[BUF_SIZE];
    socklen_t cli_len;
    struct sockaddr_in serv_addr, cli_addr;

    //Buat dan attach ke shared memory
    key_t key = SHMKEY;
    int shmid = shmget(key, sizeof key, IPC_CREAT | 0666);
    if (shmid == -1)
    {
        perror("Gagal membuat shared memory");
        exit(EXIT_FAILURE);
    }
    int *value = shmat(shmid, NULL, 0);
    if (value == (void*)-1)
    {
        perror("Gagal meng-attach ke shared memory");
        exit(EXIT_FAILURE);
    }
    *value = 0;

    //Buat thread untuk menanmpilkan stok setiap 5 detik
    pthread_t tid;
    int err = pthread_create(&tid, NULL, &print_stok, value);
    if (err != 0)
    {
        puts("Gagal membuat thread");
        exit(EXIT_FAILURE);
    }

    //Buka socket
    serv_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1)
    {
        perror("Gagal membuat socket");
        exit(EXIT_FAILURE);
    }

    //Kosongkan serv_addr dan isi
    memset((char*)&serv_addr, '\0', sizeof serv_addr);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    serv_addr.sin_port = htons(PORT);

    //Server binding
    if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof serv_addr) == -1)
    {
        perror("Binding gagal");
        exit(EXIT_FAILURE);
    }

    //Buka listener
    if (listen(serv_sock, 5) == -1)
    {
        perror("Gagal membuat listener");
        exit(EXIT_FAILURE);
    }

    //Tunggu koneksi
    cli_len = sizeof cli_addr;
    cli_sock = accept(serv_sock, (struct sockaddr*)&cli_addr, &cli_len);
    if (cli_sock == -1)
    {
        perror("Koneksi gagal");
        exit(EXIT_FAILURE);
    }

    //Tunggu perintah dari klien
    memset(buffer, '\0', BUF_SIZE);
    while ((n = read(cli_sock, buffer, BUF_SIZE - 1)) != 0)
    {
        buffer[n] = '\0';
        puts(buffer);
        if (strcmp(buffer, "tambah") == 0)
            (*value)++;
        memset(buffer, '\0', BUF_SIZE);
    }

    //Selesai
    close(cli_sock);
    close(serv_sock);

    return 0;
}
