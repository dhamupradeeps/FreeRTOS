

#include <FreeRTOS.h>
#include <task.h>
#include <stdio.h>
#include <string.h>

// Some string to print
const char msg[] = "New Message.";

// Task handles
static TaskHandle_t task_1 = NULL;
static TaskHandle_t task_2 = NULL;

//*****************************************************************************
// Tasks

// Task: print a message to Serial Terminal with lower priority
void startTask1(void *parameter) {
    int msg_len = strlen(msg);

    while (1) {
        printf("\n");
        for (int i = 0; i < msg_len; i++) {
            putchar(msg[i]);
        }
        printf("\n");
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for 1000 ms
    }
}

// Task: print '*' to Serial Terminal with higher priority
void startTask2(void *parameter) {
    while (1) {
        putchar('*');
        fflush(stdout); // Ensure immediate output
        vTaskDelay(pdMS_TO_TICKS(100)); // Delay for 100 ms
    }
}

//*****************************************************************************
// Main function

int main(void) {
    // Configure Serial (if applicable)
    printf("---FreeRTOS Task Demo---\n");

    // Create tasks with different priorities
    xTaskCreate(startTask1, "Task 1", 256, NULL, 1, &task_1); // Lower priority
    xTaskCreate(startTask2, "Task 2", 256, NULL, 2, &task_2); // Higher priority

    // Start the scheduler
    vTaskStartScheduler();

    // Should never reach here as the scheduler takes control
    while (1) {
    }

    return 0;
}

// Loop-like function for demonstration of task management
void manageTasks(void) {
    // Suspend the higher-priority task for some intervals
    for (int i = 0; i < 3; i++) {
        vTaskSuspend(task_2);
        vTaskDelay(pdMS_TO_TICKS(2000)); // Delay for 2000 ms
        vTaskResume(task_2);
        vTaskDelay(pdMS_TO_TICKS(2000)); // Delay for 2000 ms
    }

    // Delete the lower-priority task
    if (task_1 != NULL) {
        vTaskDelete(task_1);
        task_1 = NULL;
    }
}
