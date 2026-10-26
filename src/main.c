/*
 * Sistema de monitoramento de acesso - FRDM-KL25Z / Zephyr RTOS 4.2
 *
 * Cenario: um sensor de porta e um sensor de presenca detectam eventos e
 * cada um deles registra o evento em um contador global do sistema.
 * Ao final, o programa audita o contador: o total registrado deve ser
 * igual ao total de eventos realmente detectados pelos sensores.
 *
 * Objetivo didatico: mostrar a condicao de corrida (USAR_MUTEX = 0) e a
 * sua correcao com mutex (USAR_MUTEX = 1). Edite src/configuracao.h.
 */

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#include "configuracao.h"
#include "registro_eventos.h"
#include "sensores.h"

int main(void)
{
	const uint32_t total_esperado = QUANTIDADE_DE_SENSORES * EVENTOS_POR_SENSOR;

	printk("\n=====================================================\n");
	printk("Monitor de acesso - demonstracao de race condition\n");
#if USAR_MUTEX
	printk("Versao executada: 2 - COM mutex\n");
#else
	printk("Versao executada: 1 - SEM sincronizacao\n");
#endif
	printk("Sensores ativos: %u (porta e presenca)\n", QUANTIDADE_DE_SENSORES);
	printk("Eventos por sensor: %u\n", EVENTOS_POR_SENSOR);
	printk("Valor esperado no contador: %u\n", total_esperado);
	printk("Executando... (aguarde alguns segundos)\n");

	registro_eventos_iniciar();
	sensores_iniciar();
	sensores_aguardar_conclusao();

	/* Auditoria: compara o que os sensores detectaram com o que foi registrado */
	const uint32_t total_detectado = sensores_total_detectado();
	const uint32_t total_registrado = registro_eventos_obter_total();
	const uint32_t eventos_perdidos = total_detectado - total_registrado;

	printk("Eventos detectados pelos sensores: %u\n", total_detectado);
	printk("Valor obtido no contador:          %u\n", total_registrado);
	printk("Diferenca (esperado - obtido):     %u\n", total_esperado - total_registrado);

	if (total_registrado == total_esperado) {
#if USAR_MUTEX
		printk("RESULTADO: sincronizacao OK. Nenhum evento foi perdido.\n");
#else
		printk("RESULTADO: nenhuma perda nesta execucao. Isso NAO prova que o codigo\n"
		       "e seguro: a race condition depende do escalonamento.\n");
#endif
	} else {
		printk("RESULTADO: %u evento(s) PERDIDO(S) - race condition demonstrada.\n",
		       eventos_perdidos);
	}
	printk("=====================================================\n");

	return 0;
}
