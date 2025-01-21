#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <stdio.h>

static SemaphoreHandle_t mutex_1;
static SemaphoreHandle_t mutex_2;

void doTaskA(void *parameters) {
    while (1) {
        // Try to take mutex_1
        if (xSemaphoreTake(mutex_1, portMAX_DELAY) == pdTRUE) {
            printf("Task A took mutex 1\n");
            vTaskDelay(1 / portTICK_PERIOD_MS);

            // Try to take mutex_2
            if (xSemaphoreTake(mutex_2, portMAX_DELAY) == pdTRUE) {
                printf("Task A took mutex 2\n");
                printf("Task A doing some work\n");
                vTaskDelay(500 / portTICK_PERIOD_MS);

                // Release mutex_2
                xSemaphoreGive(mutex_2);
            }
            // Release mutex_1
            xSemaphoreGive(mutex_1);
        }

        printf("Task A going to sleep\n");
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

void doTaskB(void *parameters) {
    while (1) {
        // Try to take mutex_2
        if (xSemaphoreTake(mutex_2, portMAX_DELAY) == pdTRUE) {
            printf("Task B took mutex 2\n");
            vTaskDelay(1 / portTICK_PERIOD_MS);

            // Try to take mutex_1
            if (xSemaphoreTake(mutex_1, portMAX_DELAY) == pdTRUE) {
                printf("Task B took mutex 1\n");
                printf("Task B doing some work\n");
                vTaskDelay(500 / portTICK_PERIOD_MS);

                // Release mutex_1
                xSemaphoreGive(mutex_1);
            }
            // Release mutex_2
            xSemaphoreGive(mutex_2);
        }

        printf("Task B going to sleep\n");
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

void vSetupTasks(void) {
    // Create mutexes
    mutex_1 = xSemaphoreCreateMutex();
    mutex_2 = xSemaphoreCreateMutex();

    if (mutex_1 != NULL && mutex_2 != NULL) {
        // Create Task A
        xTaskCreate(
            doTaskA,
            "Task A",
            1024,
            NULL,
            2, // High priority
            NULL
        );

        // Create Task B
        xTaskCreate(
            doTaskB,
            "Task B",
            1024,
            NULL,
            1, // Low priority
            NULL
        );
    } else {
        printf("Error creating mutexes!\n");
    }
}

int main(void) {
    printf("FreeRTOS Deadlock Demo\n");

    // Setup tasks and mutexes
    vSetupTasks();

    // Start the FreeRTOS scheduler
    vTaskStartScheduler();

    // Infinite loop (should never reach here)
    for (;;);
}
