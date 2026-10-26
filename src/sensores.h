/*
 * Modulo de sensores (simulados).
 *
 * Cria uma tarefa (thread) para cada sensor. Cada tarefa "detecta" eventos
 * em intervalos de tempo e pede o registro de cada um no modulo
 * registro_eventos.
 *
 * Em um produto real, a deteccao viria de interrupcao de GPIO (reed switch
 * da porta, sensor PIR de presenca) e a tarefa esperaria por ela em vez de
 * usar um intervalo fixo. A estrutura do restante do sistema nao mudaria.
 */

#ifndef SENSORES_H
#define SENSORES_H

#include <stdint.h>

/* Cria e inicia as tarefas dos sensores */
void sensores_iniciar(void);

/* Bloqueia ate que todas as tarefas de sensores terminem */
void sensores_aguardar_conclusao(void);

/* Retorna quantos eventos os sensores realmente detectaram (todos somados).
 * Serve como referencia para auditar o contador global. */
uint32_t sensores_total_detectado(void);

#endif /* SENSORES_H */
