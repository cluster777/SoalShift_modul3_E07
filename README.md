# Soal Shift Modul 3 E07
## Nomor 1
Untuk nomor 1 diminta untuk menampilkan hasil faktorial sekelompok angka dan
menampilkannya secara urut (input tidak perlu terurut)

Contoh :
```
./faktorial 5 3 4
3! = 6
4! = 24
5! = 120
```

Agar menampilkannya terurut, maka perlu disort dulu inputnya
```C
int i = 0, arr[100];
while ((scanf("%d",&arr[i])) == 1)
    i++;

//Bubble sort
for (int j = 0; j < i; j++)
{
    for(int k = j + 1; k < i; k++)
    {
        if(arr[j] > arr[k])
        {
            int temp = arr[j];
            arr[j] = arr[k];
            arr[k] = temp;
        }
    }
}
```

Kemudian dibuat thread untuk masing-masing angka agar bisa dihitung secara paralel
```C
struct isi
{
    int hitung;
    int pos;
};

void* fact(void* arg)
{
    int hasil = 1, base = ((struct isi*)arg)->hitung;

    while(base > 1)
    {
        hasil *= base;
        base--;
    }
    if (((struct isi*)arg)->pos>0)pthread_join(tid[((struct isi*)arg)->pos-1], NULL);
    printf("%d %d! = %d\n",((struct isi*)arg)->pos, ((struct isi*)arg)->hitung, hasil);
}

for(int j=0; j<i; j++)
{
    struct isi *test = (struct isi*)malloc(sizeof(struct isi));
    test->hitung = arr[j];
    test->pos = j;
    pthread_create(&(tid[j]), NULL, &fact, (void*)test));
}
```

## Nomor 2
Nomor 2 diminta untuk membuat 2 server yang masing-masing berhubungan dengan satu klien.
Server penjual dan server pembeli mempunyai stok barang yang sama.
Klien penjual bisa menambah stok, klien pembeli bila membeli (i.e. mengurangi) stok.

Catatan :
1. Antar server berhubungan dengan shared memory
2. Klien dengan server berhubungan dengan socket
3. SHMKEY, PORT, dan BUF_SIZE silahkan didefinisikan sendiri (#define)

### a) Server pembeli
Langkah-langkah :
1. Buat shared memory untuk dishare dengan server penjual
```C
key_t key = SHMKEY;
int shmid = shmget(key, sizeof key, IPC_CREAT | 0666);
int *value = shmat(shmid, NULL, 0);
*value = 0;
```
2. Buka socket dengan klien pembeli
```C
//Variabel
int serv_sock, cli_sock;
socklen_t cli_len;
struct sockaddr_in serv_addr, cli_addr;

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
```
3. Tunggu perintah dari klien
```C
//Variabel
int n;
char buffer[BUF_SIZE];
const char berhasil[] = "transaksi berhasil";
const char gagal[] = "transaksi gagal";
const char unknown[] = "tidak dikenali";

//Tunggu perintah
memset(buffer, '\0', BUF_SIZE);
while ((n = read(cli_sock, buffer, BUF_SIZE - 1)) != 0)
{
    buffer[n] = '\0';
    puts(buffer);
    if (strcmp(buffer, "beli") == 0)
    {
        if (*value > 0)
        {
            (*value)--;
            write(cli_sock, berhasil, strlen(berhasil));
        }
        else
            write(cli_sock, gagal, strlen(gagal));
    }
    else
        write(cli_sock, unknown, strlen(unknown));
    memset(buffer, '\0', BUF_SIZE);
}
```

### b) Server penjual
Langkah-langkah :
1. Buat shared memory
```C
key_t key = SHMKEY;
int shmid = shmget(key, sizeof key, IPC_CREAT | 0666);
int *value = shmat(shmid, NULL, 0);
*value = 0;
```
2. Buat thread unutk menampilkan stok setiap 5 detik
```C
//Fungsinya
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

pthread_t tid;
int err = pthread_create(&tid, NULL, &print_stok, value);
if (err != 0)
{
    puts("Gagal membuat thread");
    exit(EXIT_FAILURE);
}
```
3. Buka socket untuk klien penjual
```C
//Variabel
int serv_sock, cli_sock;
socklen_t cli_len;
struct sockaddr_in serv_addr, cli_addr;

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
```
4. Tunggu perintah dari klien
```C
//Variabel
int n;
char buffer[BUF_SIZE];

//Tunggu perintah
memset(buffer, '\0', BUF_SIZE);
while ((n = read(cli_sock, buffer, BUF_SIZE - 1)) != 0)
{
    buffer[n] = '\0';
    puts(buffer);
    if (strcmp(buffer, "tambah") == 0)
        (*value)++;
    memset(buffer, '\0', BUF_SIZE);
}
```

### c) Klien pembeli
Langkah-langkah :
1. Buka socket ke server
```C
//Variabel
int serv_sock;
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
serv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
serv_addr.sin_port = htons(PORT);

//Mulai koneksi
if (connect(serv_sock, (struct sockaddr*)&serv_addr, sizeof serv_addr) == -1)
{
    perror("Koneksi gagal");
    exit(EXIT_FAILURE);
}
```
2. Mulai kirimkan perintah
```C
//Variabel
int n;
char buffer[BUF_SIZE], *newline;

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
    n = read(serv_sock, buffer, BUF_SIZE - 1);
    if (n == 0)
        break;
    buffer[n] = '\0';
    puts(buffer);
    memset(buffer, '\0', BUF_SIZE);
}
```

### d) Klien penjual
Langkah-langkah :
1. Buka socket
```C
//Variabel
int serv_sock;
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
```
2. Mulai kirimkan perintah
```C
//Variabel
int n;
char buffer[BUF_SIZE], *newline;

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
```

## Nomor 3
Nomor 3 diminta untuk menambahkan WakeUp_Status Agmal atau Spirit_Status Iraj.

Catatan :
1. Jika input "Agmal Ayo Bangun", WakeUp_Status bertambah 15.
2. Jika input "Iraj Ayo Tidur", Spirit_Status berkurang 20.
3. Jika input "All Status", tampilkan status keduanya.
4. Jika "Agmal Ayo Bangun" dipakai 3 kali, "Iraj Ayo Tidur" nonaktif selama 10 detik.
5. Program berhenti jika WakeUp_Status >= 100 atau Spirit_Status <= 0.

thread Agmal
```C
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
```

thread Iraj
```C
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
```

Main loop
```C
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
```

## Nomor 4
Langkah-langkah :
1. Ambil 10 proses yang sedang berjalan.
```C
void* p11()
{
	system("ps -aux|head -10>>/root/Documents/salin1.txt");
}
```
2. Simpan di dua file txt.
```C
void* p21()
{
    system("cp /root/Documents/salin1.txt /root/Documents/salin2.txt");
}
```
3. Zip dua file tersebut dan hapus keduanya.
```C
void* p12()
{
    system("zip /root/Documents/zip1 /root/Documents/salin1.txt");
    system("rm /root/Documents/salin1.txt");
    sleep(15);
    pthread_create(&(tid[2]),NULL,&p13,NULL);
    pthread_join(tid[2],NULL);
}
void* p22()
{
    system("zip /root/Documents/zip2 /root/Documents/salin2.txt");
    system("rm /root/Documents/salin2.txt");
    sleep(15);
    pthread_create(&(tid[3]),NULL,&p23,NULL);
    pthread_join(tid[3],NULL);
}
```
4. Setelah 15 detik, ekstrak kembali.
```C
void* p13()
{
    system("unzip /root/Documents/zip1 -d /root/Documents");
    system("rm /root/Documents/zip1.zip");
}
void* p23()
{
    system("unzip /root/Documents/zip2 -d /root/Documents");
    system("rm /root/Documents/zip2.zip");
}
```
Di main()
```C
int main()
{
    pthread_create(&(tid[0]), NULL, &p11, NULL);
    pthread_join(tid[0], NULL);
    pthread_create(&(tid[1]), NULL, &p21, NULL);
    pthread_join(tid[1], NULL);
    pthread_create(&(tid[0]), NULL, &p12, NULL);
    pthread_create(&(tid[1]), NULL, &p22, NULL);
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
}
```

## Nomor 5
Bekerja sesuai yang diminta

Implementasinya
```C
//Variabel
int kekenyangan, kebersihan, hp, boleh = 1, punyaku = 10, cd = 0, input = 0;
pthread_mutex_t makan, mandi, batle, cool, boleh_mandi, toko;
pthread_t tid[10];

//Thread pembantu
void* hitung_lapar()
{
    while(1)
    {
        sleep(10);
        pthread_mutex_lock(&makan);
        kekenyangan -= 5;
        pthread_mutex_unlock(&makan);
    }
}
void* hitung_kotor()
{
    while(1)
    {
        sleep(30);
        pthread_mutex_lock(&mandi);
        kebersihan -= 10;
        pthread_mutex_unlock(&mandi);
    }
}
void* hitung_regen()
{
    while(1)
    {
        sleep(10);
        pthread_mutex_lock(&batle);
        hp += 5;
        if(hp > 300)
            hp = 300;
        pthread_mutex_unlock(&batle);
    }
}
void* gaboleh()
{
    while(cd > 0)
    {
        sleep(1);
        pthread_mutex_lock(&cool);
        cd--;
        pthread_mutex_unlock(&cool);
    }
    pthread_mutex_lock(&boleh_mandi);
    boleh = 1;
    pthread_mutex_unlock(&boleh_mandi);
    return NULL;
}
char getch()
{
    char buf = 0;
    struct termios old = {0};
    fflush(stdout);
    if(tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag&=~ICANON;
    old.c_lflag&=~ECHO;
    old.c_cc[VMIN]=1;
    old.c_cc[VTIME]=0;
    if(tcsetattr(0, TCSANOW, &old)<0)
        perror("tcsetattr ICANON");
    if(read(0,&buf,1)<0)
        perror("read()");
    old.c_lflag|=ICANON;
    old.c_lflag|=ECHO;
    if(tcsetattr(0, TCSADRAIN, &old)<0)
        perror ("tcsetattr ~ICANON");
    printf("%c\n",buf);
    return buf;
}

void* xinput()
{
    input = (int)getch() - '0';
    return NULL;
}

void* xwait()
{
    sleep(1);
    pthread_cancel(tid[4]);
    return NULL;
}

int main()
{
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
    pthread_mutex_init(&makan, NULL);
    pthread_mutex_init(&mandi, NULL);
    pthread_mutex_init(&batle, NULL);
    pthread_mutex_init(&boleh_mandi, NULL);
    pthread_mutex_init(&cool, NULL);
    pthread_mutex_init(&toko, NULL);
    char *nama;
    printf("inputkan nama>");
    scanf("%s", nama);

    hp = 300;
    kekenyangan = 200;
    kebersihan = 100;
    pthread_create(&(tid[0]), NULL, &hitung_lapar, NULL);
    pthread_create(&(tid[1]), NULL, &hitung_kotor, NULL);
    pthread_create(&(tid[2]), NULL, &hitung_regen, NULL);
    while((hp > 0) && (kebersihan > 0) && (kekenyangan > 0))
    {
        system("clear");
        printf("standby mode\nhealth : %d\nhunger : %d\nhygiene : %d\nfood left : %d
                \nbath will be ready in :%ds\nchoices\n1.eat\n2.bath\n3.battle\n4.shop
                \n5.exit\n", hp, kekenyangan, kebersihan, punyaku, cd);
        input = 0;
        pthread_create(&(tid[4]), NULL, &xinput, NULL);
        pthread_create(&(tid[5]), NULL, &xwait, NULL);
        pthread_join(tid[5], NULL);
        tcflush(0, TCIFLUSH);
        if(input == 1 && punyaku > 0)
        {
            pthread_mutex_lock(&makan);
            kekenyangan += 15;
            punyaku--;
            if(kekenyangan > 200)
                kekenyangan = 200;
            pthread_mutex_unlock(&makan);
        }
        else if((input == 2) && (boleh == 1))
        {
            pthread_mutex_lock(&mandi);
            pthread_mutex_lock(&boleh_mandi);
            kebersihan += 30;
            if(kebersihan > 100)
                kebersihan = 100;
            boleh = 0;
            cd = 20;
            pthread_create(&(tid[3]), NULL, &gaboleh, NULL);
            pthread_mutex_unlock(&mandi);
        }
        else if(input == 3)
        {
            pthread_mutex_lock(&batle);
            pthread_mutex_lock(&mandi);
            pthread_mutex_lock(&makan);
            int en = 100;
            while(1)
            {
                system("clear");
                printf("battle mode\nmonster's health : %d\nenemy's health : %d
                        \nchoices\n 1.attack\n 2.run\n", hp, en);
                input = 0;
                input = 0;
                pthread_create(&(tid[4]), NULL, &xinput, NULL);
                pthread_create(&(tid[5]), NULL, &xwait, NULL);
                pthread_join(tid[5], NULL);
                tcflush(0, TCIFLUSH);

                if(input == 2)
                    break;
                if(input == 1)
                {
                    en -= 20;
                    sleep(1);
                    if(en <= 0)
                        break;
                    hp -= 20;
                    if(hp <= 0)
                        break;
                }

            }
            if(hp <= 0)
                break;
            pthread_mutex_unlock(&batle);
            pthread_mutex_unlock(&mandi);
            pthread_mutex_unlock(&makan);
        }
        else if(input == 4)
        {
            pthread_mutex_lock(&batle);
            while(1)
            {
                system("clear");
                printf("shop mode\nshop food stock :%d\nyour food stock : %d
                        \nchoices\n1.buy\n2.back\n", *value, punyaku);
                input = 0;
                input = 0;
                pthread_create(&(tid[4]), NULL, &xinput, NULL);
                pthread_create(&(tid[5]), NULL, &xwait, NULL);
                pthread_join(tid[5], NULL);
                tcflush(0, TCIFLUSH);
                if(input == 2)
                    break;
                if((input == 1) && (*value > 0))
                {
                    punyaku++;
                    (*value)--;
                }

            }
            pthread_mutex_unlock(&batle);
        }
        else if(input == 5)
            break;
    }
```

Program penjual
```C
//Variabel
int input;
pthread_t tid[2];

char getch()
{
    /*#include <unistd.h>   //_getch*/
    /*#include <termios.h>  //_getch*/
    char buf=0;
    struct termios old= {0};
    fflush(stdout);
    if(tcgetattr(0, &old)<0)
        perror("tcsetattr()");
    old.c_lflag&=~ICANON;
    old.c_lflag&=~ECHO;
    old.c_cc[VMIN]=1;
    old.c_cc[VTIME]=0;
    if(tcsetattr(0, TCSANOW, &old)<0)
        perror("tcsetattr ICANON");
    if(read(0,&buf,1)<0)
        perror("read()");
    old.c_lflag|=ICANON;
    old.c_lflag|=ECHO;
    if(tcsetattr(0, TCSADRAIN, &old)<0)
        perror ("tcsetattr ~ICANON");
    printf("%c\n",buf);
    return buf;
}

//Thread pembantu
void* xinput()
{
    input = (int)getch() - '0';
    return NULL;
}
void* xwait()
{
    sleep(1);
    pthread_cancel(tid[0]);
    return NULL;
}

int main()
{
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
    *value = 10;
    while(1)
    {
        system("clear");
        printf("shop\nfood stock :%d\nchoices\n1.Restock\n2.Exit\n", *value);
        input = 0;
        pthread_create(&(tid[0]), NULL, &xinput, NULL);
        pthread_create(&(tid[1]), NULL, &xwait, NULL);
        pthread_join(tid[1], NULL);
        tcflush(0, TCIFLUSH);
        if(input == 2)
            break;
        else if(input == 1)
        {
            (*value)++;
        }
    }
}
```
