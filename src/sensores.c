#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#include "configuracao.h"
#include "registro_eventos.h"
#include "sensores.h"

/* Descricao e estado de um sensor. Cada sensor possui seu PROPRIO contador
 * de eventos detectados, escrito apenas pela tarefa dele (sem compartilhar). */
struct sensor {
	const char *nome;
	int32_t periodo_ms;
	uint32_t eventos_a_detectar;
	uint32_t eventos_detectados;
};

static struct sensor sensores[QUANTIDADE_DE_SENSORES] = {
	{ .nome = "porta",
	  .periodo_ms = PERIODO_SENSOR_PORTA_MS,
	  .eventos_a_detectar = EVENTOS_POR_SENSOR },
	{ .nome = "presenca",
	  .periodo_ms = PERIODO_SENSOR_PRESENCA_MS,
	  .eventos_a_detectar = EVENTOS_POR_SENSOR },
};

K_THREAD_STACK_ARRAY_DEFINE(pilhas_tarefas_sensores, QUANTIDADE_DE_SENSORES,
			    TAMANHO_PILHA_TAREFA_SENSOR);

static struct k_thread dados_tarefas_sensores[QUANTIDADE_DE_SENSORES];
static k_tid_t identificadores_tarefas[QUANTIDADE_DE_SENSORES];

/* Corpo da tarefa de um sensor: detecta eventos e manda registra-los */
static void tarefa_sensor(void *parametro_sensor, void *nao_usado_2, void *nao_usado_3)
{
	struct sensor *sensor_atual = parametro_sensor;

	ARG_UNUSED(nao_usado_2);
	ARG_UNUSED(nao_usado_3);

	for (uint32_t i = 0U; i < sensor_atual->eventos_a_detectar; i++) {
		/* Aguarda a proxima deteccao (simulada por um intervalo) */
		k_msleep(sensor_atual->periodo_ms);

		sensor_atual->eventos_detectados++;

		/* Registra o evento no contador global do sistema */
		registro_eventos_registrar();
	}
}

void sensores_iniciar(void)
{
	for (uint32_t i = 0U; i < QUANTIDADE_DE_SENSORES; i++) {
		identificadores_tarefas[i] = k_thread_create(
			&dados_tarefas_sensores[i],
			pilhas_tarefas_sensores[i],
			K_THREAD_STACK_SIZEOF(pilhas_tarefas_sensores[i]),
			tarefa_sensor, &sensores[i], NULL, NULL,
			PRIORIDADE_TAREFA_SENSOR, 0, K_NO_WAIT);

		k_thread_name_set(identificadores_tarefas[i], sensores[i].nome);
	}
}

void sensores_aguardar_conclusao(void)
{
	for (uint32_t i = 0U; i < QUANTIDADE_DE_SENSORES; i++) {
		k_thread_join(identificadores_tarefas[i], K_FOREVER);
	}
}

uint32_t sensores_total_detectado(void)
{
	uint32_t total = 0U;

	for (uint32_t i = 0U; i < QUANTIDADE_DE_SENSORES; i++) {
		total += sensores[i].eventos_detectados;
	}
	return total;
}
