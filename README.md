<h1 align="center"> Simulador de Escalonamento de Processos em Tempo Real </h1>
<h2 align="center"> Visão Geral </h1>

<p align="center">Este projeto implementa um simulador de escalonamento de processos em tempo real, utilizando as estratégias de escalonamento Rate Monotonic (RM) e Earliest Deadline First (EDF). Ele recebe como entrada um conjunto de tarefas, cada uma com seu próprio período, tempo de execução (CPU burst) e deadline, e simula a execução dessas tarefas em um ambiente de tempo real seguindo os algoritmos de escalonamento RM ou EDF.

## Arquivo de Entrada

Você pode utilizar qualquer arquivo de entrada seguindo a formatação abaixo, o `input.txt` é usado apenas para exemplificar. 

### Formato do Arquivo de Entrada

O arquivo de entrada deve ter o seguinte formato:

`<tempo total de simulação>`       &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  `120`<br>
`<Processo> <Período> <CPU Burst> `&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`T1 50 25`<br>
`<Processo> <Período> <CPU Burst> `&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`T2 80 35`<br>
`...`    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`...`<br><br>

- >`<tempo total de simulação>`: O tempo total para a simulação rodar.
- >`<Processo>`: O nome da tarefa.
- >`<Período>`: O período do processo.
- >`<CPU Burst>`: O tempo de execução do processo.
<br>

## Como Compilar

O projeto inclui um `Makefile` para facilitar a compilação e execução. Para compilar, utilize os seguintes comandos:

```
make rate
```
> Para compilar o simulador usando o algoritmo Rate Monotonic
```
make edf
```
> Para compilar o simulador usando o algoritmo Earliest Deadline First <br>

Isso criará os executáveis rate ou edf, respectivamente.

## Execução

Após a compilação, você pode executar o simulador da seguinte forma:
```
./rate <caminho_do_arquivo_de_entrada>
```
> Para executar o simulador com o algoritmo Rate Monotonic
```
./edf <caminho_do_arquivo_de_entrada>
```
> Para executar o simulador com o algoritmo Earliest Deadline First

<br>

## Requisitos

Para compilar e executar este simulador, você precisará de:

- Um compilador C, como o GCC (GNU Compiler Collection).
- Make, para automatizar a compilação usando o `Makefile` fornecido.
- Um sistema operacional capaz de executar aplicativos compilados em C, como Linux, macOS ou Windows (com o uso de Cygwin ou Subsistema Windows para Linux `WSL` ).<br><br><br>
 
> [!WARNING]
> Este projeto é apenas para fins educacionais e demonstra a implementação básica dos algoritmos de escalonamento em tempo real RM e EDF. Não é adequado para uso em sistemas de tempo real reais sem modificações e validações adicionais. 🚧
