# Dataset — sensor ultrassônico (HC-SR04)

## Sensor utilizado
HC-SR04 (ultrassônico), já presente no carrinho desde o CP1. No firmware
principal (CP1) ele usa os pinos GPIO1 (TRIG) / GPIO3 (ECHO), os únicos
realmente livres na ESP32-CAM. Só que, para a **coleta de dados**, esses
mesmos pinos são usados pela Serial — então religamos o sensor
temporariamente em dois pinos livres só durante a coleta:
**TRIG → GPIO2, ECHO → GPIO4** (ver nota no topo de
[`../coleta_dados/coleta_dados.ino`](../coleta_dados/coleta_dados.ino)).

## Dado coletado
Distância (cm) até o obstáculo à frente do sensor, amostrada a 20Hz (1 leitura
a cada 50ms) por 5 segundos (100 amostras por captura/arquivo).

## Classes definidas
| Classe   | Faixa de distância usada na coleta | Significado |
|---|---|---|
| `livre`    | > 60 cm        | Sem obstáculo relevante — carrinho segue normalmente |
| `atencao`  | 20 cm – 60 cm  | Obstáculo se aproximando — reduzir velocidade |
| `perigo`   | < 20 cm        | Obstáculo muito próximo — parar/recuar |

(Ajustar as faixas acima para os valores realmente usados na coleta, se forem diferentes.)

## Como foi coletado
Sketch [`../coleta_dados/coleta_dados.ino`](../coleta_dados/coleta_dados.ino) rodado na própria
ESP32-CAM do carrinho, em modo contínuo (repete a captura sozinho a cada
~13s). Para cada captura: um objeto foi posicionado manualmente na distância
da classe desejada durante a contagem regressiva, e o CSV impresso no Serial
Monitor ao final de cada ciclo foi salvo como um arquivo `classe_NN.csv`.
Capturas em que o objeto foi movido durante a leitura (distância variando
muito no meio do bloco) foram descartadas e não entraram no dataset.

## Arquivos CSV
Nomenclatura: `<classe>_<numero>.csv` (ex: `perigo_01.csv`, `atencao_01.csv`, `livre_01.csv`),
organizados em subpastas por classe: [`livre/`](livre/), [`atencao/`](atencao/), [`perigo/`](perigo/).
Cada arquivo = 1 captura de 5s = 1 amostra (sample) no Edge Impulse.

## Quantidade de amostras
- `livre`: 5 capturas
- `atencao`: 5 capturas
- `perigo`: 25 capturas
- Total: 35 capturas limpas (~3500 linhas de leitura no total)

> `perigo` ficou com bem mais amostras que as outras duas classes. Isso não
> impede o treino no Edge Impulse, mas o ideal é coletar mais algumas
> capturas de `livre` e `atencao` (pelo menos mais 3-5 de cada) antes de
> treinar, para as três classes ficarem mais balanceadas.

Formato de cada CSV (gerado pelo sketch de coleta):
```
timestamp_ms,distancia_cm
0,85.3
50,84.9
100,85.1
...
```
