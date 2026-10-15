# Guia passo a passo — Edge Impulse (CP2)

Pré-requisito: já ter os arquivos CSV coletados (ver [`dataset/`](dataset/)), pelo
menos ~8-10 capturas por classe (`livre`, `atencao`, `perigo`).

## 1) Criar o projeto
1. Entre em [studio.edgeimpulse.com](https://studio.edgeimpulse.com) e crie uma conta (grátis).
2. **Create new project** → nomeie algo como `carrinho-tinyml-cp2`.
3. Em **Dashboard → Project info**, confirme que o tipo de dado é "Time series".

## 2) Importar os dados (Data acquisition)
Para cada arquivo CSV coletado:
1. Vá em **Data acquisition**.
2. Clique em **Upload data** (ícone de upload, canto superior).
3. Selecione o(s) arquivo(s) CSV daquela classe.
4. Em **Label**, digite o nome da classe (`livre`, `atencao` ou `perigo`) — tem que
   bater exatamente com o nome usado em todos os arquivos daquela classe.
5. Em **Signal from category**, deixe o Edge Impulse dividir automaticamente
   entre *Training* e *Testing* (ele faz ~80/20 sozinho).
6. Repita para todos os arquivos de todas as classes.

Confira em **Data acquisition** se os três labels aparecem com um número
razoável de amostras cada (evite deixar uma classe com muito menos dados que
as outras).

## 3) Criar o Impulse (Create impulse)
1. Vá em **Create impulse**.
2. **Input block**: Time series data — Window size `5000` ms, Window increase
   `1000` ms (ou `5000`, igual ao tamanho da janela, se não quiser sobreposição).
3. **Processing block**: adicione **Raw Data** (mais simples e direto — como é
   só uma variável/distância variando devagar no tempo, não precisa de FFT/
   espectro; "Raw Data" já funciona bem aqui).
4. **Learning block**: adicione **Classification (Keras)**.
5. Clique **Save Impulse**.

## 4) Gerar features (Raw Data)
1. Vá na aba do bloco **Raw Data** (menu lateral).
2. Clique **Save parameters** → depois **Generate features**.
3. Veja o gráfico de *Feature explorer* — o ideal é ver as três classes
   relativamente separadas em "nuvens" diferentes. Se estiverem muito
   misturadas, pode ser necessário coletar dados com distâncias mais
   distintas entre as classes.

## 5) Treinar o modelo
1. Vá em **Classification** (o bloco de aprendizado).
2. Configure: **Number of training cycles** ~30, **Learning rate** padrão
   (0.0005 ou similar — deixe o valor sugerido pelo Edge Impulse).
3. Arquitetura: a rede densa (dense) padrão que o Edge Impulse sugere já é
   suficiente para esse caso (não precisa de CNN pra um sinal 1D simples).
4. Clique **Start training**.
5. Anote/print: **accuracy** final e a **matriz de confusão** que aparecem ao
   final do treino — isso vai direto na documentação do CP2.

## 6) Testar o modelo (Model testing)
1. Vá em **Model testing** → **Classify all**.
2. Anote a acurácia nos dados de teste (não usados no treino).
3. Isso é a evidência de "testes de inferência" pedida no rubric.

## 7) Exportar como biblioteca Arduino
1. Vá em **Deployment**.
2. Escolha **Arduino library**.
3. Em **EON Compiler**, pode deixar ligado (reduz o tamanho do modelo).
4. Clique **Build** — baixa um `.zip` (algo como `nome-do-projeto_inferencing.zip`).
5. No Arduino IDE: **Sketch → Include Library → Add .ZIP Library...** → selecione
   esse zip. Isso instala a biblioteca de inferência gerada.
6. Esse `.zip` também deve ser salvo no repositório, em
   [`modelo_exportado/`](modelo_exportado/), como evidência.

## 8) Evidências para a documentação
Tirar prints de:
- Tela de **Data acquisition** mostrando as 3 classes com as quantidades de amostras.
- Gráfico do **Feature explorer**.
- Resultado do treino (**accuracy** + **matriz de confusão**).
- Resultado do **Model testing**.
- Tela de **Deployment** mostrando o build da biblioteca Arduino.

Salvar essas imagens em [`evidencias_edge_impulse/`](evidencias_edge_impulse/) e
referenciar cada uma na seção correspondente do README principal.
