

#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <stdio.h>
#include <string.h>

#define NUM_TASKS 5  
#define MSG_SIZE 20   

typedef struct Message {
    char body[MSG_SIZE];
    uint8_t len;
} Message;

static SemaphoreHandle_t sem_params; 

void myTask(void *parameters) {
    Message msg = *(Message *)parameters;

    xSemaphoreGive(sem_params);

    printf("Task received: %s | len: %d\n", msg.body, msg.len);

    vTaskDelay(pdMS_TO_TICKS(1000));

    vTaskDelete(NULL);
}



int main(void) {
    char task_name[12];
    Message msg;
    char text[MSG_SIZE] = "All your base";

    sem_params = xSemaphoreCreateCounting(NUM_TASKS, 0);
    if (sem_params == NULL) {
        printf("Failed to create counting semaphore\n");
        while (1); 
    }

    strcpy(msg.body, text);
    msg.len = strlen(text);

    for (int i = 0; i < NUM_TASKS; i++) {
        sprintf(task_name, "Task_%d", i);

        if (xTaskCreate(myTask,          
                        task_name,       
                        1024,           
                        (void *)&msg,    
                        1,               
                        NULL) != pdPASS) { 
            printf("Failed to create %s\n", task_name);
        }
    }

    for (int i = 0; i < NUM_TASKS; i++) {
        xSemaphoreTake(sem_params, portMAX_DELAY);
    }

    printf("All tasks have read the parameter and started successfully.\n");

    vTaskStartScheduler();

    while (1);
}