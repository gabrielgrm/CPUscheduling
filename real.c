#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
int proximoProcessoRM(Processos processo[], int quantidadeProcessos, int tempoAtual) {
    int indexProximo = -1;
    int menorPeriodo = 987654321;

    for (int i = 0; i < quantidadeProcessos; i++) {
        if (processo[i].tempoRestante > 0 && tempoAtual >= processo[i].deadline - processo[i].periodo) {
            if (processo[i].periodo < menorPeriodo) {
                menorPeriodo = processo[i].periodo;
                indexProximo = i;
            }
        }
    }
    return indexProximo;
}


int proximoProcessoEDF(Processos processo[], int quantidadeProcessos, int tempoAtual) {
  int indexProximo = -1;
  int menorDeadline = 987654321;
  for (int i = 0; i < quantidadeProcessos; i++) {
      if (processo[i].tempoRestante > 0 && processo[i].deadline <= menorDeadline &&
          tempoAtual >= processo[i].deadline - processo[i].periodo) {
          menorDeadline = processo[i].deadline;
          indexProximo = i; 
      }
  }
  return indexProximo;
}
void escalonamento(Processos processo[], int n, int tempoTotal, FILE *saida) {
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
  #ifdef RATE
  fprintf(saida, "EXECUTION BY RATE\n");
  #endif
  #ifdef EDF
  fprintf(saida, "EXECUTION BY EDF\n");
  #endif
  while (tempoAtual < tempoTotal) {
    #ifdef RATE
    indexProcessoAtual = proximoProcessoRM(processo, n, tempoAtual);
    #endif
    #ifdef EDF
    indexProcessoAtual = proximoProcessoEDF(processo, n, tempoAtual);
    #endif
      for(int k = 0; k < n; k++) {
        if (tempoAtual >= processo[k].deadline) {
          processo[k].tempoRestante = processo[k].cpuBurst;
          processo[k].deadline += processo[k].periodo;
          processo[k].execucaoPerdida++;
        }
      }
    if (indexUltimoProcesso != -1 && indexUltimoProcesso != indexProcessoAtual && processo[indexUltimoProcesso].tempoRestante > 0 && unidadesUltimoProcesso > 0) {
      fprintf(saida, "[%s] for %d units - H\n", processo[indexUltimoProcesso].nomeProcesso, unidadesUltimoProcesso);
      unidadesUltimoProcesso = 0;
    }
    if (indexProcessoAtual == -1) {
      idle++;
    } else {

      if (idle > 0) {
        fprintf(saida, "idle for %d units\n", idle);
        idle = 0;
      }
      Processos *processoAtual = &processo[indexProcessoAtual];
      processoAtual->tempoRestante--;
      unidadesUltimoProcesso++;
      if (processoAtual->tempoRestante == 0) {
        processoAtual->execucaoCompleta++;
        processoAtual->deadline += processoAtual->periodo;
        processoAtual->tempoRestante = processoAtual->cpuBurst;
        fprintf(saida, "[%s] for %d units - F\n", processoAtual->nomeProcesso,
                unidadesUltimoProcesso);
        unidadesUltimoProcesso = 0;
      }
      if (tempoAtual >= (processoAtual->deadline)-1 && processoAtual->tempoRestante > 0) {
        processoAtual->execucaoPerdida++;
        processoAtual->deadline += processoAtual->periodo;
        processoAtual->tempoRestante = processoAtual->cpuBurst;
        if(indexUltimoProcesso != indexProcessoAtual) {
           if (processo[indexProcessoAtual].periodo == processo[indexUltimoProcesso].periodo && indexProcessoAtual != indexUltimoProcesso ) {
            fprintf(saida, "[%s] for %d units - H\n", processoAtual->nomeProcesso, unidadesUltimoProcesso);
           }
           #ifdef EDF
            else fprintf(saida, "[%s] for %d units - L\n", processoAtual->nomeProcesso, unidadesUltimoProcesso);
           #endif
        }
        #ifdef RATE
        else if (tempoAtual == (processo[indexProcessoAtual].deadline-1)) {
          fprintf(saida, "[%s] for %d units - H\n", processoAtual->nomeProcesso, unidadesUltimoProcesso);
        } else fprintf(saida, "[%s] for %d units - L\n", processoAtual->nomeProcesso, unidadesUltimoProcesso);
        #endif
        unidadesUltimoProcesso = 0;
      }
    }
    indexUltimoProcesso = indexProcessoAtual;
    tempoAtual++;
  }
  if (idle > 0) {
    fprintf(saida, "idle for %d units\n", idle);
  }

  if (indexUltimoProcesso != -1 &&
      processo[indexUltimoProcesso].tempoRestante > 0 &&
      unidadesUltimoProcesso > 0) {
    fprintf(saida, "[%s] for %d units - K\n",
            processo[indexUltimoProcesso].nomeProcesso, unidadesUltimoProcesso);
  }
  for(int j = 0; j < n; j++) {
    if(processo[j].execucaoKill == 0 && processo[j].execucaoCompleta == 0 && processo[j].execucaoPerdida == 0) {
      processo[j].execucaoPerdida = tempoTotal/processo[j].periodo;
      if (tempoTotal >= processo[j].execucaoPerdida * processo[j].periodo) {
        processo[j].execucaoKill = 1;
      }

    }
    if(processo[j].tempoRestante < processo[j].cpuBurst && (processo[j].cpuBurst - processo[j].tempoRestante) + (processo[j].deadline - processo[j].periodo) > tempoTotal) {
      processo[j].execucaoKill++;
    }else if(processo[j].deadline - processo[j].periodo <= tempoTotal && processo[j].deadline - processo[j].periodo + processo[j].cpuBurst >= tempoTotal) {
      processo[j].execucaoKill++;
    }
  }
}

void printar(Processos processo[], int quantidadeProcessos, FILE *saida) {
    fprintf(saida, "\nLOST DEADLINES\n");
    for (int i = 0; i < quantidadeProcessos; i++) {
        fprintf(saida, "[%s] %d\n", processo[i].nomeProcesso, processo[i].execucaoPerdida);
    }
    fprintf(saida, "\nCOMPLETE EXECUTION\n");
    for (int i = 0; i < quantidadeProcessos; i++) {
        fprintf(saida, "[%s] %d\n", processo[i].nomeProcesso, processo[i].execucaoCompleta);
    }
      fprintf(saida, "\nKILLED");
    #ifdef RATE
      for (int i = 0; i < quantidadeProcessos; i++) {
          fprintf(saida, "\n[%s] %d", processo[i].nomeProcesso, processo[i].execucaoKill);
      }
    #endif
    #ifdef EDF
      for (int i = 0; i < quantidadeProcessos; i++) {
          fprintf(saida, "\n[%s] 1", processo[i].nomeProcesso);
      }
    #endif
}


int main(int argc, char *argv[]) {
    FILE *input = fopen(argv[1], "r");
    #ifdef RATE
    FILE *saida = fopen("rate.out", "w");
    #endif
    #ifdef EDF
    FILE *saida = fopen("edf.out", "w");
    #endif
    int tempoTotal, n = 0;
    char line[100];
    if (fgets(line, 100, input) != NULL) {
        line[strcspn(line, "\r\n")] = 0;
        sscanf(line, "%d", &tempoTotal);
    }
    Processos processo[1000];
    while (fgets(line, 100, input) != NULL) {
        if (sscanf(line, "%s %d %d", processo[n].nomeProcesso, &processo[n].periodo,
                &processo[n].cpuBurst) == 3) {
            n++;
        }
    }
    escalonamento(processo, n, tempoTotal, saida);
    printar(processo, n, saida);
    fclose(input);
    fclose(saida);
    return 0;
}
