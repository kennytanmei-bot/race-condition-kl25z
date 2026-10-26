/*
 * Configuracao do sistema de monitoramento de acesso.
 *
 * Este e o UNICO arquivo que voce precisa editar para alternar entre
 * as duas versoes do experimento.
 */

#ifndef CONFIGURACAO_H
#define CONFIGURACAO_H

/* ------------------------------------------------------------------ */
/* Selecao da versao do experimento                                    */
/* ------------------------------------------------------------------ */

/*
 * 0 -> Versao 1: registro de eventos SEM protecao (condicao de corrida possivel)
 * 1 -> Versao 2: registro de eventos protegido por MUTEX
 */
#define USAR_MUTEX 0

/* ------------------------------------------------------------------ */
/* Parametros do cenario simulado                                      */
/* ------------------------------------------------------------------ */

/* Quantidade de eventos que cada sensor vai detectar durante o teste */
#define EVENTOS_POR_SENSOR 1000U

/* Quantidade de sensores (tarefas) ativos no sistema */
#define QUANTIDADE_DE_SENSORES 2U

/* Intervalo medio entre deteccoes de cada sensor (simulado), em ms */
#define PERIODO_SENSOR_PORTA_MS 2
#define PERIODO_SENSOR_PRESENCA_MS 3

/*
 * Tempo gasto no tratamento do evento (classificar, gerar registro, etc.),
 * em ms. Fica ENTRE a leitura e a escrita do contador. Em um sistema real
 * seria o tempo de processamento ou de acesso a outro recurso; aqui ele
 * existe para tornar a condicao de corrida facil de observar.
 */
#define TEMPO_TRATAMENTO_EVENTO_MS 1

/* ------------------------------------------------------------------ */
/* Parametros das threads                                              */
/* ------------------------------------------------------------------ */

#define TAMANHO_PILHA_TAREFA_SENSOR 1024
#define PRIORIDADE_TAREFA_SENSOR 5

#endif /* CONFIGURACAO_H */
