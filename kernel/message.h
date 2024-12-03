#ifndef MESSAGE_H
#define MESSAGE_H

typedef struct message {
    int sender_pid;
    char content[128];
} message_t;

#define MAX_MESSAGES 10
#define MAX_MESSAGE_SIZE 128

typedef struct message_queue {
    message_t messages[MAX_MESSAGES];
    int head;
    int tail;
    int count; // Representa el tamaño actual de la cola
    struct spinlock lock; // Sincronización para la cola
} message_queue_t;

extern message_queue_t msg_queue;

#endif // MESSAGE_H
