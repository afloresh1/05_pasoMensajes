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

int main()
{
    int msqid;
    int msgflg = IPC_CREAT | 0666;
    key_t key;
    message_buf sbuf;
    message_buf rbuf;
    size_t buf_length;
    key = 1234;
    if ((msqid = msgget(key, msgflg)) < 0)
    {
        perror("msgget()");
        exit(1);
    }
    // Esto me indica el tipo del mensaje
    sbuf.mtype = 1;
    (void)strcpy(sbuf.mtext, "Are you hearing me?");
    buf_length = strlen(sbuf.mtext) + 1;
    // En este caso el emisor o sender envia el mensaje pero no espera respuesta
    // para saber si el receptor recibio o no el mensaje
    if (msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT) < 0)
    {
        perror("msgsnd");
        exit(1);
    }

    if (msgrcv(msqid, &rbuf, MSGSZ, 2, 0) < 0)
    {
        perror("msgrcv");
        exit(1);
    }
    printf("message_rec: %s\n", rbuf.mtext);

    sbuf.mtype = 3;
    strcpy(sbuf.mtext, "I can hear you too");
    if (msgsnd(msqid, &sbuf, strlen(sbuf.mtext) + 1, 0) < 0)
    {
        perror("msgsnd");
        exit(1);
    }

   exit(0);
}
