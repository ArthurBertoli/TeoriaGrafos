# Análise de Grafo por Lista de Adjacências

> Projeto da disciplina **Teoria dos Grafos** — Ciência da Computação, UDESC.

## Sobre

Programa em C que carrega um grafo a partir de um arquivo de arestas (`teste2.csv`), o representa como **lista de adjacências** e extrai propriedades do grafo. O grafo é tratado como não dirigido: cada aresta `u v` é inserida nos dois sentidos.

## Funcionalidades

- **Carga do grafo**: lê pares de inteiros `u v` (separados por espaço, um por linha) de `teste2.csv`.
- **Grau máximo e mínimo** dos vértices, de 0 até o maior identificador lido.
- **Classificação**: grafo simples ou multigrafo, contando laços e arestas múltiplas.
- **Componentes conexos**: busca em profundidade (DFS) a partir de cada vértice não visitado, mostrando o tamanho de cada componente e o total de componentes.

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
