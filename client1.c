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
    message_buf sbuf, rbuf;

    if ((msqid = msgget(key, IPC_CREAT | 0666)) < 0)
    {
        perror("msgget");
        exit(1);
    }

    sbuf.mtype = 1;
    sprintf(sbuf.mtext, "2 Consulta desde Cliente 1");
    if (msgsnd(msqid, &sbuf, strlen(sbuf.mtext) + 1, 0) < 0)
    {
        perror("msgsnd");
        exit(1);
    }

    if (msgrcv(msqid, &rbuf, MSGSZ, 2, 0) < 0)
    {
        perror("msgrcv");
        exit(1);
    }
    printf("Respuesta del Servidor: %s\n", rbuf.mtext);

    return 0;
}
