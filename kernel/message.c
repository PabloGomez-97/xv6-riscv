#include "types.h"
#include "message.h"
#include "spinlock.h"

// Definir la cola de mensajes global
message_queue_t msg_queue;

void init_message_queue() {
    initlock(&msg_queue.lock, "msg_queue");
    msg_queue.head = 0;
    msg_queue.tail = 0;
    msg_queue.size = 0;
}
