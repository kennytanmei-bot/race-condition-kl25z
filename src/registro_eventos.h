/*
 * Modulo de registro de eventos.
 *
 * Mantem o contador GLOBAL de eventos do sistema. Esse contador e o
 * recurso compartilhado: todas as tarefas de sensores o utilizam.
 */

#ifndef REGISTRO_EVENTOS_H
#define REGISTRO_EVENTOS_H

#include <stdint.h>

/* Zera o contador. Deve ser chamada antes de iniciar as tarefas. */
void registro_eventos_iniciar(void);

/* Registra UM evento detectado (incrementa o total do sistema).
 * Pode ser chamada por varias threads. */
void registro_eventos_registrar(void);

/* Retorna o total de eventos registrados ate o momento. */
uint32_t registro_eventos_obter_total(void);

#endif /* REGISTRO_EVENTOS_H */
