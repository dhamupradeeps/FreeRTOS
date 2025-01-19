
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <stdio.h>
#include <stdlib.h>

static SemaphoreHandle_t bin_sem;

void blinkLED(void *parameters) {
    int delay_time = *(int *)parameters;

    xSemaphoreGive(bin_sem);

    printf("Received delay time: %d ms\n", delay_time);
 
    printf("Initializing LED_PIN as output\n");

    while (1) {
        // Simulate turning LED on/off
        printf("LED ON\n");
        vTaskDelay(pdMS_TO_TICKS(delay_time));

        printf("LED OFF\n");
        vTaskDelay(pdMS_TO_TICKS(delay_time));
    }
}


int main(void) {
    int delay_arg;

    srand(12345);

    printf("\n---FreeRTOS Binary Semaphore Demo---\n");
    printf("Enter a delay time in milliseconds: ");

    delay_arg = 500; 
    printf("%d\n", delay_arg);

    bin_sem = xSemaphoreCreateBinary();
    if (bin_sem == NULL) {
        printf("Failed to create binary semaphore\n");
        while (1); // Halt if semaphore creation fails
    }

    xTaskCreate(blinkLED,        // Task function
                "Blink LED",     // Task name
                1024,            // Stack size
                (void *)&delay_arg, // Task parameter
                1,               // Task priority
                NULL);           // Task handle

    xSemaphoreTake(bin_sem, portMAX_DELAY);

    printf("Semaphore received, task parameter passed successfully.\n");

    vTaskStartScheduler();

    while (1);
}
