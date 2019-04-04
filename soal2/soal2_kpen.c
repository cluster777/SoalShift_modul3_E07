#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define BUF_SIZE 256
#define PORT 4747

int main()
{
    //Variabel
    int serv_sock, n;
    char buffer[BUF_SIZE], *newline;
    struct sockaddr_in serv_addr;

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
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    //Mulai koneksi
    if (connect(serv_sock, (struct sockaddr*)&serv_addr, sizeof serv_addr) == -1)
    {
        perror("Koneksi gagal");
        exit(EXIT_FAILURE);
    }

    //Kirim perintah ke server
    memset(buffer, '\0', BUF_SIZE);
    while (1)
    {
        fgets(buffer, BUF_SIZE, stdin);
        newline = strrchr(buffer, '\n');
        if (newline != NULL)
            *newline = '\0';
        write(serv_sock, buffer, strlen(buffer));
        memset(buffer, '\0', BUF_SIZE);
    }

    //Selesai
    close(serv_sock);
    return 0;
}
