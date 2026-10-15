#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MSGSZ 128

typedef struct msgbuf
{
    long mtype;
    char mtext[MSGSZ];
} message_buf;

int main(void)
{
    int msqid;
    key_t key = 5678;
    message_buf rbuf, sbuf;
    int client_id;
    char client_msg[MSGSZ];

    if ((msqid = msgget(key, IPC_CREAT | 0666)) < 0)
    {
        perror("msgget");
        exit(1);
    }

    for (int i = 0; i < 2; i++)
    {
        if (msgrcv(msqid, &rbuf, MSGSZ, 1, 0) < 0)
        {
            perror("msgrcv");
            exit(1);
        }

        sscanf(rbuf.mtext, "%d %[^\n]", &client_id, client_msg);
        printf("Mensaje de Cliente %d: %s\n", client_id, client_msg);

        sbuf.mtype = client_id;
        if (client_id == 2)
        {
            strcpy(sbuf.mtext, "Hola Cliente 1, tu mensaje fue procesado.");
        }
        else if (client_id == 3)
        {
            strcpy(sbuf.mtext, "Hola Cliente 2, tu mensaje fue procesado.");
        }

        if (msgsnd(msqid, &sbuf, strlen(sbuf.mtext) + 1, 0) < 0)
        {
            perror("msgsnd");
            exit(1);
        }
    }

    return 0;
}
