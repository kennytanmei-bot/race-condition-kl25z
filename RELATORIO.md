# Relatório: Condição de Corrida em um Monitor de Acesso Embarcado com Zephyr RTOS

**Disciplina:** Microcontroladores 2 · **Placa:** FRDM-KL25Z · **RTOS:** Zephyr 4.2.0

> Os campos entre colchetes devem ser preenchidos com os resultados reais obtidos por você ao executar o experimento. As saídas marcadas como "ilustrativas" não são medições.

## 1. Título e objetivo

**Título:** Condição de corrida e sincronização com mutex em um monitor de acesso com Zephyr RTOS.

**Objetivo:** mostrar que duas threads que modificam o mesmo contador sem proteção podem perder atualizações, e que o uso de um mutex garante o resultado correto.

## 2. Cenário utilizado

Um sistema de monitoramento de acesso possui um sensor de porta e um sensor de presença. Cada sensor é uma tarefa independente (thread) que, ao detectar um evento, o registra em um contador global de eventos do sistema. Os sensores foram simulados por intervalos de tempo (2 ms e 3 ms entre detecções). Cada um detecta 1000 eventos, logo o total esperado é 2000. Ao final, o sistema audita o contador comparando-o com a soma dos eventos que cada sensor contou individualmente.

## 3. Condição de corrida

Uma condição de corrida ocorre quando o resultado depende da ordem de execução de threads que acessam um recurso compartilhado sem sincronização. O incremento do contador equivale a ler, calcular e escrever. Se a thread do sensor de porta lê o valor 10 e é suspensa, e a thread do sensor de presença também lê 10, ambas escreverão 11. Dois eventos ocorreram, mas o contador subiu apenas uma unidade: houve uma **atualização perdida**.

## 4. Implementação sem sincronização (Versão 1)

O módulo `registro_eventos` lê o total, calcula o novo valor, gasta 1 ms no tratamento do evento (`k_msleep`) e então escreve o resultado. A pausa foi inserida para favorecer a troca de thread entre a leitura e a escrita e tornar o problema reproduzível. Não há proteção sobre essa região crítica (`USAR_MUTEX = 0`). O resultado pode variar entre execuções, pois depende do escalonamento.

## 5. Implementação com mutex (Versão 2)

Com `USAR_MUTEX = 1`, a região crítica (ler, calcular, tratar e escrever) é envolvida por `k_mutex_lock` e `k_mutex_unlock`. Enquanto uma thread está dentro da região, a outra fica bloqueada. A pausa permaneceu dentro da região protegida apenas por motivos didáticos; em aplicações reais a região crítica deve ser a menor possível.

## 6. Resultados observados

**Versão 1 (preencher com o resultado real):** esperado 2000; detectado pelos sensores [___]; registrado no contador [___]; diferença [___].

**Versão 2 (preencher com o resultado real):** esperado 2000; detectado pelos sensores [___]; registrado no contador [___]; diferença [___].

Exemplo **ilustrativo** (não medido) do formato da saída serial:

```
Versao executada: 1 - SEM sincronizacao
Valor esperado no contador: 2000
Eventos detectados pelos sensores: 2000
Valor obtido no contador:          <menor que 2000>
RESULTADO: <N> evento(s) PERDIDO(S) - race condition demonstrada.
```

```
Versao executada: 2 - COM mutex
Valor esperado no contador: 2000
Eventos detectados pelos sensores: 2000
Valor obtido no contador:          2000
Diferenca (esperado - obtido):     0
RESULTADO: sincronizacao OK. Nenhum evento foi perdido.
```

## 7. Comparação com o esperado

Na Versão 1 espera-se um valor registrado menor que 2000, que pode variar entre execuções. Na Versão 2 espera-se exatamente 2000. [Comente aqui se os valores reais confirmaram essa expectativa.]

## 8. Conclusão

O experimento mostra que o acesso concorrente a recursos compartilhados exige sincronização. Sem ela, o resultado depende do escalonador e surgem erros intermitentes e difíceis de depurar, como eventos de acesso não registrados em um sistema de segurança. O mutex garante exclusão mútua na região crítica, ao custo de bloquear a thread concorrente, por isso essa região deve ser curta. Em sistemas embarcados, o mesmo problema ocorre com buffers de comunicação, periféricos e variáveis compartilhadas entre threads e interrupções.
