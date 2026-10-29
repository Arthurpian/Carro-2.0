# Evidências — Edge Impulse (CP2)

Prints da Edge Impulse Studio, do projeto `Arthurpian-project-1`, documentando o processo de treinamento do modelo TinyML.

| Arquivo | O que mostra |
|---|---|
| `01_data_acquisition.png` | Dataset completo: 26 amostras de treino / 7 de teste (79%/21%), divididas entre as classes `livre`, `atencao` e `perigo`. |
| `02_create_impulse.png` | Configuração do Impulse: Time series (janela de 5.000ms, 20Hz), bloco de processamento Raw Data, bloco de classificação (Neural Network), 3 classes de saída. |
| `03_feature_explorer.png` | Feature Explorer — separação das classes no espaço de features (mostra a sobreposição entre `livre`/`atencao` que explica os erros no teste). |
| `03b_raw_data_exemplo.png` | Exemplo de uma amostra individual (`livre_01`) com o sinal bruto do sensor e o resultado do processamento DSP. |
| `04_treino_matriz_confusao.png` | Resultado do treinamento: 100% de acurácia na validação, matriz de confusão e arquitetura da rede neural (camadas densas de 20 e 10 neurônios). |
| `04b_treino_metricas_performance.png` | Métricas de precisão/recall/F1 do treino + Data Explorer (amostras classificadas) + performance estimada no dispositivo (1ms de inferência, 1.5KB de RAM). |
| `05_model_testing.png` | Model Testing no conjunto de teste (nunca visto no treino): 71,43% de acurácia — os 2 erros classificaram `livre`/`atencao` como `perigo` (erro para o lado seguro). |
| `06_deployment.png` | Deployment: biblioteca Arduino exportada com EON Compiler, modelo quantizado (int8), build gerado em 14/10/2026. |

Resultados completos e discussão no [README principal do projeto](../../README.md#cp2--tinyml-edge-impulse) e no [README do dataset](../dataset/README.md).
