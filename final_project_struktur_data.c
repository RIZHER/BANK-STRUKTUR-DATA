#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct user_Node
{
    char username [64];
    int PIN;
    float saldo;
    struct user_Node *next;
    struct user_Node *prev;
}user_Node;

user_Node *Newuser_Node (char *username, int PIN, float saldo)
{
    user_Node *n = malloc (sizeof(user_Node));
    strcpy(n -> username, username);
    n -> PIN = PIN;
    n -> saldo = saldo;
    n -> prev = NULL;
    n -> next = NULL;
    return n;
}

user_Node *user_head = NULL;
user_Node *user_tail = NULL;

void user_push (char *username, int PIN, float saldo)
{
    user_Node *n = Newuser_Node (username, PIN, saldo);
    if (user_head == NULL)
    {
        user_head = n;
        user_tail = n;
    }
    else
    {
        user_tail -> next = n;
        n -> prev = user_tail;
        user_tail = n;
    }
}

typedef struct transaction_Node
{
    float jumlah; //nyetor berapa atau narik berapa
    struct transaction_Node *next;
    struct transaction_Node *prev;
}transaction_Node;

transaction_Node *Newtransaction_Node (float jumlah)
{
    transaction_Node *n = malloc (sizeof(transaction_Node));
    n -> jumlah = jumlah;
    n -> prev = NULL;
    n -> next = NULL;
    return n;
}

transaction_Node *transaction_head = NULL;
transaction_Node *transaction_tail = NULL;

void transaction_push (float jumlah)
{
    transaction_Node *n = Newtransaction_Node (jumlah);
    if (transaction_head == NULL)
    {
        transaction_head = n;
        transaction_tail = n;
    }
    else
    {
        transaction_tail -> next = n;
        n -> prev = transaction_tail;
        transaction_tail = n;
    }
}

user_Node *search_user (char *username)
{
    if (user_head == NULL)
    {
        return NULL;
    }
    else
    {
        user_Node *c = user_head;
        while (c != NULL)
        {
            if (strcmp(username, c -> username)== 0)
            {
                return c;
            }
            c = c -> next;
        }
        return NULL;
    }
}

int main (void)
{
    user_Node *curent_user=NULL;
    FILE *userfile = fopen("userdb.txt", "r"); //bingung
    if (userfile)
    {
        char username [64];
        int PIN;
        float saldo;
        while (fscanf (userfile, "%[^#]#%d#%f\n", username, &PIN, &saldo) != EOF)
        {
            user_push (username, PIN, saldo);
        }
        fclose(userfile);
    }
    else
    {
        userfile = fopen ("userdb.txt", "w");
        fclose (userfile);
    }
    system("cls");
    int opt;
    int flag = 0;
    do
    {
        printf(" 1. Login\n 2. Register\n");
        do
        {
            printf ("> ");
            scanf("%d", &opt);
            getchar();
        }
        while (opt < 1 || opt > 2);
        if (opt == 1)
        {
            if(user_head == NULL)
            {
                printf ("Data Kosong!\n");
            }
            else
            {
                char username[64];
                printf("username : ");
                scanf ("%[^\n]", username);
                getchar();
                user_Node *tmp = NULL;
                tmp = search_user (username);
                if (tmp == NULL)
                {
                    printf("Username tidak ditemukan");
                }
                else
                {
                    int PIN;
                    printf("PIN : ");
                    scanf("%i", &PIN);
                    getchar();
                    if (PIN != tmp -> PIN)
                    {
                        printf("PIN salah");
                    }
                    else
                    {
                        flag = 1;
                        printf ("PIN benar! \nSELAMAT DATANG\n");
                        curent_user=tmp;
                    }
                }
            }
        }
        else
        {
            char username[64];
            int PIN;
            printf("Username : ");
            scanf("%[^\n]", username);
            getchar();
            user_Node *tmp = NULL;
            tmp = search_user (username);
            if(tmp == NULL)
            {
                printf ("PIN : ");
                scanf("%i", &PIN);
                getchar();
                user_push(username, PIN, 500000.0f);
                userfile = fopen("userdb.txt", "a");
                fprintf(userfile, "%s#%d#%f\n", username, PIN, 500000.0f);
                fclose(userfile);
                printf("Account dibuat\n");
                curent_user=user_tail;
                flag = 1;
            }
            else
            {
                printf ("Username sudah dipakai");
            }
        }
    }
    while (flag == 0);
    system ("pause");
    do{
        system ("cls");
        printf ("1. Lihat Saldo\n2. Transfer\n3. Tarik Uang\n4. Setor Uang\n5. Exit\n");
        do {
            printf("Masukkan Pilihan : ");
            scanf("%d", &opt);
            getchar();
        }while (opt < 1 || opt > 5);

        user_Node *tmp = NULL;
        char username[64];
        float nominal;

        switch (opt)
            {
                case 5 :
                    userfile = fopen("userdb.txt", "w");
                    user_Node *c = user_head;
                    while (c != NULL)
                    {
                        fprintf(userfile, "%s#%d#%f\n", c->username, c->PIN, c->saldo);
                        c = c->next;
                    }
                    return 0;
                case 1:
                    printf("Jumlah Saldo Anda: %.2f\n", curent_user->saldo);
                    system("pause");
                    break;
                case 2:
                    do
                    {
                        printf("Penerima Transfer: ");
                        scanf("%[^\n]", username);
                        getchar();
                        tmp = search_user(username);
                    }
                    while(tmp == NULL || strcmp(username, curent_user->username) == 0);
                    do
                    {
                        printf("Nominal Transfer: ");
                        scanf("%f", &nominal);
                        getchar();
                    }
                    while(nominal > curent_user->saldo);
                    tmp->saldo += nominal;
                    curent_user->saldo -= nominal;
                    break;
                case 3:
                    do
                    {
                        printf("Nominal: ");
                        scanf("%f", &nominal);
                        getchar();
                    }
                    while(nominal > curent_user->saldo);
                    curent_user->saldo -= nominal;
                    break;
                case 4:
                    printf("Nominal: ");
                    scanf("%f", &nominal);
                    getchar();
                    curent_user->saldo += nominal;
                    break;
            }
    }while (opt!=5);

}

































