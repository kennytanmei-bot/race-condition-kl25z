# Monitor de acesso: race condition na FRDM-KL25Z com Zephyr RTOS

Sistema embarcado simplificado de monitoramento de acesso. Um **sensor de porta** e um **sensor de presença** (simulados) rodam em threads separadas e registram cada evento detectado em um **contador global**. Sem sincronização, as duas threads provocam uma **condição de corrida** e eventos são perdidos. Com um **mutex** do Zephyr, o registro fica correto.

> **Status de verificação:** o código foi escrito conforme as APIs do Zephyr 4.2 e a sintaxe foi checada com cabeçalhos de teste, mas **não foi compilado com o Zephyr nem testado na placa pelo autor deste material**. Compile e execute no seu ambiente antes de entregar e registre os resultados reais.

## Requisitos

- Placa FRDM-KL25Z (MKL25Z128), cabo USB na porta do OpenSDA
- Visual Studio Code + extensão PlatformIO
- Zephyr RTOS 4.2.0 (framework `zephyr` do PlatformIO)

## Estrutura

```
race-condition-kl25z/
├── platformio.ini
├── zephyr/
│   ├── CMakeLists.txt
│   └── prj.conf
├── src/
│   ├── configuracao.h        <- parâmetros e seleção da versão (USAR_MUTEX)
│   ├── registro_eventos.c/.h <- contador global (recurso compartilhado) e mutex
│   ├── sensores.c/.h         <- tarefas dos sensores de porta e presença
│   └── main.c                <- inicia o sistema e audita o resultado
├── .gitignore
├── README.md
└── RELATORIO.md
```

## Como funciona

- Cada sensor detecta 1000 eventos, em intervalos de 2 ms (porta) e 3 ms (presença). Esperado: **2000** eventos registrados.
- Cada sensor mantém também seu próprio contador de eventos detectados (sem compartilhar), usado como referência na auditoria final.
- O registro de um evento lê o total, calcula o novo valor, gasta 1 ms no "tratamento do evento" e escreve o total. Essa pausa fica entre a leitura e a escrita e favorece a troca de thread no meio da operação.

| Versão | `USAR_MUTEX` (em `src/configuracao.h`) | Resultado |
|--------|-----------------------------------------|-----------|
| 1 – sem sincronização | `0` | Duas threads podem ler o mesmo total antigo; uma sobrescreve a atualização da outra e eventos são perdidos. |
| 2 – com mutex | `1` | `k_mutex_lock`/`k_mutex_unlock` protegem a região crítica; o total é 2000. |

O resultado da Versão 1 pode variar entre execuções, pois depende do escalonamento. A pausa só torna a perda mais provável.

## Compilar, gravar e observar

1. Abra a pasta do projeto no VS Code (`File > Open Folder`).
2. Em `src/configuracao.h`, defina `USAR_MUTEX` como `0` ou `1`.
3. Clique em **Build** (✓) na barra do PlatformIO.
4. Conecte a placa e clique em **Upload** (→).
5. Abra o **Serial Monitor** (115200 baud); se a saída já tiver passado, aperte o reset da placa.

Cada execução leva alguns segundos.

## Saída ilustrativa (NÃO é resultado medido)

Versão 1:

```
Versao executada: 1 - SEM sincronizacao
Valor esperado no contador: 2000
Eventos detectados pelos sensores: 2000
Valor obtido no contador:          <menor que 2000>
RESULTADO: <N> evento(s) PERDIDO(S) - race condition demonstrada.
```

Versão 2:

```
Versao executada: 2 - COM mutex
Valor esperado no contador: 2000
Eventos detectados pelos sensores: 2000
Valor obtido no contador:          2000
Diferenca (esperado - obtido):     0
RESULTADO: sincronizacao OK. Nenhum evento foi perdido.
```

## Publicar no GitHub

```bash
git init
git add .
git commit -m "Monitor de acesso: demonstracao de race condition com Zephyr na FRDM-KL25Z"
git branch -M main
git remote add origin https://github.com/<seu-usuario>/<nome-do-repositorio>.git
git push -u origin main
```

Crie antes o repositório vazio em github.com (sem README) e troque `<seu-usuario>` e `<nome-do-repositorio>`.

## Do protótipo ao produto

Os sensores são simulados por intervalos de tempo. Em um produto real, cada tarefa esperaria uma interrupção de GPIO (reed switch da porta, sensor PIR) por meio de um semáforo, e o restante do sistema (módulo de registro e mutex) permaneceria igual.
