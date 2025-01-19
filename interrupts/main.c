
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <stdint.h>
#include <stdio.h>

#define TIMER_DIVIDER     80          
#define TIMER_MAX_COUNT   1000000     

static volatile uint16_t adc_value = 0;
static SemaphoreHandle_t binary_semaphore = NULL;

uint16_t mock_adc_read() {
    return rand() % 1024;
}

void onTimerCallback(void *parameters) {
    BaseType_t task_woken = pdFALSE;

    adc_value = mock_adc_read();

    xSemaphoreGiveFromISR(binary_semaphore, &task_woken);

    portYIELD_FROM_ISR();
}

void printValuesTask(void *parameters) {
    while (1) {
        if (xSemaphoreTake(binary_semaphore, portMAX_DELAY)) {
            printf("ADC Value: %u\n", adc_value);
        }
    }
}



void setupTimer(TimerHandle_t *timer_handle) {
    *timer_handle = xTimerCreate(
        "OneSecondTimer",                      // Timer name
        pdMS_TO_TICKS(1000),                   // Timer period (in ticks)
        pdTRUE,                                // Auto-reload
        NULL,                                  // Timer ID
        onTimerCallback                        // Callback function
    );

    if (*timer_handle == NULL) {
        printf("Failed to create timer. Halting...\n");
        while (1); // Halt
    }

    if (xTimerStart(*timer_handle, 0) != pdPASS) {
        printf("Failed to start timer. Halting...\n");
        while (1); // Halt
    }
}


int main(void) {
    printf("--- FreeRTOS ISR Semaphore Demo ---\n");

    binary_semaphore = xSemaphoreCreateBinary();
    if (binary_semaphore == NULL) {
        printf("Failed to create semaphore. Halting...\n");
        while (1); // Halt
    }

    if (xTaskCreate(
            printValuesTask,             
            "PrintValues",               
            configMINIMAL_STACK_SIZE,    
            NULL,                        
            tskIDLE_PRIORITY + 1,        
            NULL                        
        ) != pdPASS) {
        printf("Failed to create task. Halting...\n");
        while (1); 
    }

    TimerHandle_t timer_handle = NULL;
    setupTimer(&timer_handle);

    vTaskStartScheduler();

    printf("Scheduler failed to start. Halting...\n");
    while (1); // Halt
}
