# Análise de Grafo por Lista de Adjacências

> Projeto da disciplina **Teoria dos Grafos** — Ciência da Computação, UDESC.

## Sobre

Programa em C que carrega um grafo a partir de um arquivo de arestas (`teste2.csv`), o representa como **lista de adjacências** e extrai propriedades do grafo. O grafo é tratado como não dirigido: cada aresta `u v` é inserida nos dois sentidos.

## Funcionalidades

- **Carga do grafo**: lê pares de inteiros `u v` (separados por espaço, um por linha) de `teste2.csv`. A leitura para no primeiro dado que não for número, e arestas com vértice fora de 0 a 9999 são ignoradas com aviso.
- **Grau máximo e mínimo**, considerando apenas os vértices que aparecem em alguma aresta do arquivo (um laço soma 2 ao grau).
- **Classificação**: grafo simples ou multigrafo, contando laços (cada um uma vez) e arestas múltiplas (cada repetição de uma aresta já lida).
- **Componentes conexos**: busca em profundidade (DFS iterativa, com pilha explícita) a partir de cada vértice não visitado, mostrando o tamanho de cada componente e o total de componentes.

O limite de vértices é definido por `MAX_VERTICES` (10000).

## Tecnologias

- Linguagem C

## Estrutura

```
├── main.c         # código-fonte
├── teste2.csv     # base de arestas usada nos testes
└── relatorio.pdf  # relatório com explicação da solução e análise dos resultados
```

## Como compilar e executar

O arquivo `teste2.csv` precisa estar no mesmo diretório em que o programa for executado.

```bash
gcc -Wall -Wextra -O2 -std=c99 main.c -o analise_grafo
./analise_grafo
```

## Autores

- Arthur Bertoli Silva
- Leonardo Rosa da Silva
