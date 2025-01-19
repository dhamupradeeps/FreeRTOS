
#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>
#include <stdio.h>

static TimerHandle_t one_shot_timer = NULL;
static TimerHandle_t auto_reload_timer = NULL;

void myTimerCallback(TimerHandle_t xTimer) {
    uint32_t timer_id = (uint32_t)pvTimerGetTimerID(xTimer);

    if (timer_id == 0) {
        printf("One-shot timer expired\n");
    } else if (timer_id == 1) {
        printf("Auto-reload timer expired\n");
    }
}


int main(void) {
    printf("---FreeRTOS Timer Demo---\n");

    one_shot_timer = xTimerCreate(
        "One-shot Timer",              
        pdMS_TO_TICKS(2000),          
        pdFALSE,                       
        (void *)0,                     
        myTimerCallback               
    );

    auto_reload_timer = xTimerCreate(
        "Auto-reload Timer",         
        pdMS_TO_TICKS(1000),           
        pdTRUE,                    
        (void *)1,                    
        myTimerCallback               
    );

    if (one_shot_timer == NULL || auto_reload_timer == NULL) {
        printf("Failed to create one or both timers\n");
        while (1); 
    }

    printf("Starting timers...\n");
    if (xTimerStart(one_shot_timer, portMAX_DELAY) != pdPASS) {
        printf("Failed to start one-shot timer\n");
    }
    if (xTimerStart(auto_reload_timer, portMAX_DELAY) != pdPASS) {
        printf("Failed to start auto-reload timer\n");
    }

    vTaskStartScheduler();

    while (1);
}
