#include <zephyr/kernel.h>

#include "configuracao.h"
#include "registro_eventos.h"

/*
 * Total de eventos registrados pelo sistema (RECURSO COMPARTILHADO).
 * 'volatile' impede o compilador de guardar o valor em registrador, mas
 * NAO torna o incremento seguro entre threads.
 */
static volatile uint32_t total_eventos_registrados;

/* Mutex que protege o contador quando USAR_MUTEX == 1 */
K_MUTEX_DEFINE(mutex_registro_eventos);

void registro_eventos_iniciar(void)
{
	total_eventos_registrados = 0U;
}

void registro_eventos_registrar(void)
{
#if USAR_MUTEX
	/* Entrada na regiao critica: apenas uma thread por vez passa daqui */
	k_mutex_lock(&mutex_registro_eventos, K_FOREVER);
#endif

	/* --- INICIO DA REGIAO CRITICA --- */

	/* 1) Le o total atual */
	uint32_t total_lido = total_eventos_registrados;

	/* 2) Calcula o novo total */
	uint32_t novo_total = total_lido + 1U;

	/* Tratamento do evento (classificacao, geracao do registro...).
	 * A thread dorme e o escalonador pode executar a outra tarefa.
	 * Sem mutex, a outra tarefa le o MESMO total antigo.
	 * Em uma aplicacao real, a regiao critica deve ser a menor possivel. */
	k_msleep(TEMPO_TRATAMENTO_EVENTO_MS);

	/* 3) Escreve o novo total (pode apagar a atualizacao da outra thread) */
	total_eventos_registrados = novo_total;

	/* --- FIM DA REGIAO CRITICA --- */

#if USAR_MUTEX
	k_mutex_unlock(&mutex_registro_eventos);
#endif
}

uint32_t registro_eventos_obter_total(void)
{
	return total_eventos_registrados;
}
