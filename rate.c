#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TASKS 10

typedef struct {
  char name[20];
  int period;
  int cpuBurst;
  int deadline;
  int remainingTime;
  int executions;
  int lostDeadlines;
  int killed;
} Task;

int findNextTask(Task tasks[], int n, int currentTime) {
  for (int i = 0; i < n; i++) {
    if (tasks[i].remainingTime > 0 &&
        currentTime >= tasks[i].deadline - tasks[i].period) {
      return i;
    }
  }
  return -1;
}

void simulateTasks(Task tasks[], int n, int totalTime) {
  int currentTime = 0;
  int currentTaskIndex = -1;
  int lastTaskIndex = -1;
  int lastTaskUnits = 0;
  int idleTime = 0;

  for (int i = 0; i < n; i++) {
    tasks[i].remainingTime = tasks[i].cpuBurst;
    tasks[i].deadline = tasks[i].period;
    tasks[i].executions = 0;
    tasks[i].lostDeadlines = 0;
    tasks[i].killed = 0;
  }

  while (currentTime < totalTime) {
    currentTaskIndex = findNextTask(tasks, n, currentTime);

    if (currentTaskIndex == -1) {
      idleTime++;
      printf("idle - %d no tempo %d\n", idleTime, currentTime);
    } else {
      if (idleTime > 0) {
        printf("idle for %d units\n", idleTime);
        idleTime = 0;
      }

      Task *currentTask = &tasks[currentTaskIndex];
      currentTask->remainingTime--;
      lastTaskUnits++;

      printf("Executando %s - %d no tempo %d, CPU Remaning: %d, DL %d\n", currentTask->name, lastTaskUnits, currentTime, currentTask->remainingTime, currentTask->deadline);

      if (currentTask->remainingTime == 0) {
        currentTask->executions++;
        currentTask->deadline += currentTask->period;
        currentTask->remainingTime = currentTask->cpuBurst;
        printf("[%s] for %d units - F\n", currentTask->name, lastTaskUnits);
        lastTaskUnits = 0;
      }

      if (currentTime >= (currentTask->deadline) - 1 && currentTask->remainingTime > 0) {
        currentTask->lostDeadlines++;
        currentTask->deadline += currentTask->period;
        currentTask->remainingTime = currentTask->cpuBurst;
        printf("[%s] for %d units - L\n", currentTask->name, lastTaskUnits);
        lastTaskUnits = 0;
      }

      lastTaskIndex = currentTaskIndex;
    }

    currentTime++;
  }

  if (idleTime > 0) {
    printf("Idle for %d units\n", idleTime);
  }
  if (lastTaskIndex != -1 && tasks[lastTaskIndex].remainingTime > 0 && lastTaskUnits > 0) {
    tasks[lastTaskIndex].killed++;
    printf("[%s] for %d units - K\n", tasks[lastTaskIndex].name, lastTaskUnits);
  }
}


void printReport(Task tasks[], int n) {
  printf("\nRelatório Final:\n");
  for (int i = 0; i < n; i++) {
    printf("Tarefa %s: Deadlines perdidos: %d,Execução completa: %d, Processo Finalizado: %d\n",
           tasks[i].name, tasks[i].lostDeadlines, tasks[i].executions, tasks[i].killed);
  }
}

int main() {
  Task tasks[MAX_TASKS] = {
      {"T1", 50, 25}, {"T2", 80, 35},
  };
  int totalTime = 165;

  simulateTasks(tasks, 2, totalTime);
  printReport(tasks, 2);

  return 0;
}