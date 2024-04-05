#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TASKS 10

typedef struct {
  char nomeProcesso[20];
  int periodo;
  int cpuBurst;
  int deadline;
  int tempoRestante;
  int execucaoCompleta;
  int execucaoPerdida;
  int execucaoKill;
} Processos;

int proximoProcesso(Processos processo[], int quantidadeProcessos, int tempoAtual) {
  for (int indexProximo = 0; indexProximo < quantidadeProcessos; indexProximo++) {
    if (processo[indexProximo].tempoRestante > 0 &&
        tempoAtual >= processo[indexProximo].deadline - processo[indexProximo].periodo) {
      return indexProximo;
    }
  }
  return -1;
}

void escalonamentoRateMonotonic(Processos processo[], int n, int tempoTotal) {
  int tempoAtual = 0;
  int indexProcessoAtual = -1;
  int indexUltimoProcesso = -1;
  int unidadesUltimoProcesso = 0;
  int idle = 0;

  for (int i = 0; i < n; i++) {
    processo[i].tempoRestante = processo[i].cpuBurst;
    processo[i].deadline = processo[i].periodo;
    processo[i].execucaoCompleta = 0;
    processo[i].execucaoPerdida = 0;
    processo[i].execucaoKill = 0;
  }

  while (tempoAtual < tempoTotal) {
    indexProcessoAtual = proximoProcesso(processo, n, tempoAtual);
    if (indexUltimoProcesso != -1 && indexUltimoProcesso != indexProcessoAtual && processo[indexUltimoProcesso].tempoRestante > 0 && unidadesUltimoProcesso > 0) {
      printf("[%s] for %d units - H\n", processo[indexUltimoProcesso].nomeProcesso, unidadesUltimoProcesso);
      unidadesUltimoProcesso = 0;
    }

    if (indexProcessoAtual == -1) {
      idle++;
      printf("idle - %d no tempo %d\n", idle, tempoAtual);
    } else {
      if (idle > 0) {
        printf("idle for %d units\n", idle);
        idle = 0;
      }

      Processos *processoAtual = &processo[indexProcessoAtual];
      processoAtual->tempoRestante--;
      unidadesUltimoProcesso++;

      printf("Executando %s - %d no tempo %d, CPU Remaning: %d, DL %d\n", processoAtual->nomeProcesso, unidadesUltimoProcesso, tempoAtual, processoAtual->tempoRestante, processoAtual->deadline);

      if (processoAtual->tempoRestante == 0) {
        processoAtual->execucaoCompleta++;
        processoAtual->deadline += processoAtual->periodo;
        processoAtual->tempoRestante = processoAtual->cpuBurst;
        printf("[%s] for %d units - F\n", processoAtual->nomeProcesso, unidadesUltimoProcesso);
        unidadesUltimoProcesso = 0;
      }

      if (tempoAtual >= (processoAtual->deadline) - 1 && processoAtual->tempoRestante > 0) {
        processoAtual->execucaoPerdida++;
        processoAtual->deadline += processoAtual->periodo;
        processoAtual->tempoRestante = processoAtual->cpuBurst;
        printf("[%s] for %d units - L\n", processoAtual->nomeProcesso, unidadesUltimoProcesso);
        unidadesUltimoProcesso = 0;
      }
    }

    indexUltimoProcesso = indexProcessoAtual;
    tempoAtual++;
  }

  for (int i = 0; i < n; i++) {
    if (processo[i].tempoRestante > 0) {
      processo[i].execucaoKill++;
    }
  }
  if (idle > 0) {
    printf("Idle for %d units\n", idle);
  }
  if (indexUltimoProcesso != -1 && processo[indexUltimoProcesso].tempoRestante > 0 && unidadesUltimoProcesso > 0) {
    printf("[%s] for %d units - K\n", processo[indexUltimoProcesso].nomeProcesso, unidadesUltimoProcesso);
  }
}



void printar(Processos processo[], int n) {
  printf("\nRelatório Final:\n");
  for (int i = 0; i < n; i++) {
    printf("Tarefa %s: Deadlines perdidos: %d,Execução completa: %d, Processo Killed: %d\n",
           processo[i].nomeProcesso, processo[i].execucaoPerdida, processo[i].execucaoCompleta, processo[i].execucaoKill);
  }
}

int main() {
  Processos processo[MAX_TASKS] = {
      {"T1", 50, 25}, {"T2", 80, 35},
  };
  int tempoTotal = 165;

  escalonamentoRateMonotonic(processo, 2, tempoTotal);
  printar(processo, 2);

  return 0;
}