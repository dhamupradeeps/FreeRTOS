#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <stdio.h>

#if configNUM_CORES == 1
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

static SemaphoreHandle_t mutex_1;
static SemaphoreHandle_t mutex_2;


void doTaskA(void *parameters) {
  while (1) {
    if (xSemaphoreTake(mutex_1, portMAX_DELAY) == pdTRUE) {
      printf("Task A took mutex 1\n");
      vTaskDelay(1 / portTICK_PERIOD_MS);

      if (xSemaphoreTake(mutex_2, portMAX_DELAY) == pdTRUE) {
        printf("Task A took mutex 2\n");
        printf("Task A doing some work\n");
        vTaskDelay(500 / portTICK_PERIOD_MS);

        xSemaphoreGive(mutex_2);
      }
      xSemaphoreGive(mutex_1);
    }

    printf("Task A going to sleep\n");
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void doTaskB(void *parameters) {
 
  while (1) {
    if (xSemaphoreTake(mutex_2, portMAX_DELAY) == pdTRUE) {
      printf("Task B took mutex 2\n");
      vTaskDelay(1 / portTICK_PERIOD_MS);

      if (xSemaphoreTake(mutex_1, portMAX_DELAY) == pdTRUE) {
        printf("Task B took mutex 1\n");

        printf("Task B doing some work\n");
        vTaskDelay(500 / portTICK_PERIOD_MS);

        xSemaphoreGive(mutex_1);
      }
      xSemaphoreGive(mutex_2);
    }

    // Wait to let the other task execute
    printf("Task B going to sleep\n");
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void vSetupTasks(void) {
  mutex_1 = xSemaphoreCreateMutex();
  mutex_2 = xSemaphoreCreateMutex();

  if (mutex_1 != NULL && mutex_2 != NULL) {
    xTaskCreatePinnedToCore(
      doTaskA,
      "Task A",
      1024,
      NULL,
      2,  // High priority
      NULL,
      app_cpu
    );

    xTaskCreatePinnedToCore(
      doTaskB,
      "Task B",
      1024,
      NULL,
      1,  // Low priority
      NULL,
      app_cpu
    );
  } else {
    printf("Error creating mutexes!\n");
  }
  vTaskDelete(NULL);
}

void setup() {
  printf("FreeRTOS Deadlock Demo\n");

  vTaskDelay(1000 / portTICK_PERIOD_MS);
  printf("---FreeRTOS Deadlock Demo---\n");

  vSetupTasks();
}

void loop() {
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}
