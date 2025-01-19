
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <stdio.h>

#define MSG_QUEUE_LEN 5

static QueueHandle_t msg_queue;

void printMessages(void *parameters) {
    int item;

    while (1) {
        if (xQueueReceive(msg_queue, (void *)&item, 0) == pdTRUE) {
            printf("Received: %d\n", item); 
        }

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}


int main(void) {
    printf("\n---FreeRTOS Queue Demo---\n");

    // Create queue
    msg_queue = xQueueCreate(MSG_QUEUE_LEN, sizeof(int));
    if (msg_queue == NULL) {
        printf("Failed to create queue\n");
        while (1); 
    }

    xTaskCreate(printMessages, "Print Messages", 1024, NULL, 1, NULL);

    vTaskStartScheduler();

    while (1);
}



void producerTask(void *parameters) {
    static int num = 0;

    while (1) {
        if (xQueueSend(msg_queue, (void *)&num, pdMS_TO_TICKS(10)) != pdTRUE) {
            printf("Queue full\n");
        } else {
            printf("Sent: %d\n", num);
        }
        num++;

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
