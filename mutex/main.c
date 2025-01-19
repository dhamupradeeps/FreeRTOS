
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <stdio.h>
#include <stdlib.h>

static int shared_var = 0;
static SemaphoreHandle_t mutex;


void incTask(void *parameters) {
    int local_var;

    while (1) {
        if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
            local_var = shared_var;
            local_var++;
            vTaskDelay(rand() % 400 + 100); 
            shared_var = local_var;

            printf("Shared variable: %d\n", shared_var);

            xSemaphoreGive(mutex);
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}



int main(void) {
    printf("\n---FreeRTOS Mutex Demo---\n");

    srand(12345);

    mutex = xSemaphoreCreateMutex();
    if (mutex == NULL) {
        printf("Failed to create mutex\n");
        while (1);
    }

    xTaskCreate(incTask, "Increment Task 1", 1024, NULL, 1, NULL);

    xTaskCreate(incTask, "Increment Task 2", 1024, NULL, 1, NULL);

    vTaskStartScheduler();

    while (1);
}
