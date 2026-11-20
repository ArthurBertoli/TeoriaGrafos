#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_VERTICES 10000 // Ajuste conforme a necessidade da base teste2.csv

// Estrutura para os nós da lista de adjacências
typedef struct Node {
    int vertex;
    struct Node* next;
} Node;

// Estrutura do Grafo
typedef struct Graph {
    int numVertices;
    Node* adjLists[MAX_VERTICES];
    int visited[MAX_VERTICES];
    int degrees[MAX_VERTICES];
    bool present[MAX_VERTICES]; // vértice apareceu em alguma aresta do arquivo
} Graph;

// Variáveis globais para os requisitos d) e e)
int loops = 0;
int multipleEdges = 0;

// Função para criar um novo nó
Node* createNode(int v) {
    Node* newNode = malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Erro: memoria insuficiente.\n");
        exit(1);
    }
    newNode->vertex = v;
    newNode->next = NULL;
    return newNode;
}

// Função para inicializar o grafo
Graph* createGraph(void) {
    Graph* graph = malloc(sizeof(Graph));
    if (graph == NULL) {
        printf("Erro: memoria insuficiente.\n");
        exit(1);
    }
    graph->numVertices = MAX_VERTICES;
    for (int i = 0; i < MAX_VERTICES; i++) {
        graph->adjLists[i] = NULL;
        graph->visited[i] = 0;
        graph->degrees[i] = 0;
        graph->present[i] = false;
    }
    return graph;
}

// Insere dest no início da lista de adjacências de src
static void insertAdj(Graph* graph, int src, int dest) {
    Node* newNode = createNode(dest);
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;
}

// Requisito b e d: Adicionar aresta não dirigida {src, dest} e verificar multigrafo.
// Retorna false se algum vértice estiver fora do intervalo [0, MAX_VERTICES).
bool addEdge(Graph* graph, int src, int dest) {
    if (src < 0 || dest < 0 || src >= MAX_VERTICES || dest >= MAX_VERTICES) return false;

    // d) Verificar Arestas Múltiplas (a aresta já existia antes desta)
    Node* temp = graph->adjLists[src];
    while (temp) {
        if (temp->vertex == dest) {
            multipleEdges++;
            break;
        }
        temp = temp->next;
    }

    // Adiciona na lista (mesmo se for múltipla para respeitar a estrutura de multigrafo)
    if (src == dest) {
        // d) Laço: entra uma vez na lista e soma 2 ao grau do vértice
        loops++;
        insertAdj(graph, src, src);
        graph->degrees[src] += 2;
    } else {
        insertAdj(graph, src, dest);
        insertAdj(graph, dest, src);
        // Atualiza graus (requisito c)
        graph->degrees[src]++;
        graph->degrees[dest]++;
    }

    graph->present[src] = true;
    graph->present[dest] = true;
    return true;
}

// Requisito e: DFS (iterativa, com pilha explícita) para encontrar componentes conexos.
// A versão recursiva podia estourar a pilha de execução em componentes muito grandes.
void DFS(Graph* graph, int start, int* count, int* stack) {
    int top = 0;
    stack[top++] = start;
    graph->visited[start] = 1;
    while (top > 0) {
        int vertex = stack[--top];
        (*count)++;
        Node* adjList = graph->adjLists[vertex];
        while (adjList) {
            int connectedVertex = adjList->vertex;
            if (graph->visited[connectedVertex] == 0) {
                graph->visited[connectedVertex] = 1; // marcado ao empilhar: cada vértice entra uma vez
                stack[top++] = connectedVertex;
            }
            adjList = adjList->next;
        }
    }
}

void freeGraph(Graph* graph) {
    for (int i = 0; i < MAX_VERTICES; i++) {
        Node* atual = graph->adjLists[i];
        while (atual) {
            Node* prox = atual->next;
            free(atual);
            atual = prox;
        }
    }
    free(graph);
}

int main(void) {
    FILE* file = fopen("teste2.csv", "r");

    if (!file) {
        printf("Erro: Arquivo teste2.csv não encontrado.\n");
        return 1;
    }

    Graph* graph = createGraph();

    // Requisito b: Carga primária
    int u, v;
    int maxId = -1;
    int ignored = 0;
    while (fscanf(file, "%d %d", &u, &v) == 2) {
        if (!addEdge(graph, u, v)) { // grafo não-dirigido: cada aresta vale nos dois sentidos
            ignored++;
            continue;
        }
        if (u > maxId) maxId = u;
        if (v > maxId) maxId = v;
    }
    fclose(file);

    if (ignored > 0) {
        printf("Aviso: %d aresta(s) ignorada(s) por ter vertice fora de 0..%d.\n", ignored, MAX_VERTICES - 1);
    }
    if (maxId < 0) {
        printf("Nenhuma aresta valida encontrada em teste2.csv.\n");
        freeGraph(graph);
        return 1;
    }

    // Requisito c: Grau máximo e mínimo (apenas dos vértices que aparecem no arquivo)
    int minDeg = -1, maxDeg = 0;
    for (int i = 0; i <= maxId; i++) {
        if (!graph->present[i]) continue;
        if (graph->degrees[i] > maxDeg) maxDeg = graph->degrees[i];
        if (minDeg < 0 || graph->degrees[i] < minDeg) minDeg = graph->degrees[i];
    }

    // Requisito d: Simples ou Multigrafo
    printf("--- Analise do Grafo ---\n");
    printf("Grau Maximo: %d\n", maxDeg);
    printf("Grau Minimo: %d\n", minDeg);

    if (loops > 0 || multipleEdges > 0) {
        printf("Tipo: Multigrafo\n");
        printf("Lacos: %d\n", loops);
        printf("Arestas Multiplas: %d\n", multipleEdges);
    } else {
        printf("Tipo: Grafo Simples\n");
    }

    // Requisito e: Componentes Conexos
    int* stack = malloc(sizeof(int) * MAX_VERTICES);
    if (stack == NULL) {
        printf("Erro: memoria insuficiente.\n");
        freeGraph(graph);
        return 1;
    }
    int numComponents = 0;
    printf("\n--- Componentes Conexos ---\n");
    for (int i = 0; i <= maxId; i++) {
        if (graph->visited[i] == 0 && graph->present[i]) {
            int componentSize = 0;
            numComponents++;
            DFS(graph, i, &componentSize, stack);
            printf("Componente %d: Tamanho %d\n", numComponents, componentSize);
        }
    }
    printf("Total de Componentes: %d\n", numComponents);

    free(stack);
    freeGraph(graph);
    return 0;
}
