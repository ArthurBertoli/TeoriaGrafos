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
} Graph;

// Variáveis globais para os requisitos d) e e)
int loops = 0;
int multipleEdges = 0;

// Função para criar um novo nó
Node* createNode(int v) {
    Node* newNode = malloc(sizeof(Node));
    newNode->vertex = v;
    newNode->next = NULL;
    return newNode;
}

// Função para inicializar o grafo
Graph* createGraph() {
    Graph* graph = malloc(sizeof(Graph));
    graph->numVertices = MAX_VERTICES;
    for (int i = 0; i < MAX_VERTICES; i++) {
        graph->adjLists[i] = NULL;
        graph->visited[i] = 0;
        graph->degrees[i] = 0;
    }
    return graph;
}

// Requisito b e d: Adicionar aresta e verificar multigrafo
void addEdge(Graph* graph, int src, int dest) {
    if (src >= MAX_VERTICES || dest >= MAX_VERTICES) return;

    // d) Verificar Laços
    if (src == dest) {
        loops++;
    }

    // d) Verificar Arestas Múltiplas
    Node* temp = graph->adjLists[src];
    bool exists = false;
    while (temp) {
        if (temp->vertex == dest) {
            exists = true;
            multipleEdges++;
            break;
        }
        temp = temp->next;
    }

    // Adiciona na lista (mesmo se for múltipla para respeitar a estrutura de multigrafo)
    Node* newNode = createNode(dest);
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;
    
    // Atualiza graus (requisito c)
    graph->degrees[src]++;
}

// Requisito e: DFS para encontrar componentes conexos
void DFS(Graph* graph, int vertex, int* count) {
    graph->visited[vertex] = 1;
    (*count)++;
    Node* adjList = graph->adjLists[vertex];
    while (adjList) {
        int connectedVertex = adjList->vertex;
        if (graph->visited[connectedVertex] == 0) {
            DFS(graph, connectedVertex, count);
        }
        adjList = adjList->next;
    }
}

int main() {
    Graph* graph = createGraph();
    FILE* file = fopen("teste2.csv", "r");

    if (!file) {
        printf("Erro: Arquivo teste2.csv não encontrado.\n");
        return 1;
    }

    // Requisito b: Carga primária
    int u, v;
    int maxId = 0;
    while (fscanf(file, "%d %d", &u, &v) != EOF) {
        addEdge(graph, u, v);
        addEdge(graph, v, u); // Considerando grafo não-dirigido para componentes conexos
        if (u > maxId) maxId = u;
        if (v > maxId) maxId = v;
    }
    fclose(file);

    // Requisito c: Grau máximo e mínimo
    int minDeg = graph->degrees[0], maxDeg = 0;
    for (int i = 0; i <= maxId; i++) {
        if (graph->degrees[i] > maxDeg) maxDeg = graph->degrees[i];
        if (graph->degrees[i] < minDeg) minDeg = graph->degrees[i];
    }

    // Requisito d: Simples ou Multigrafo
    printf("--- Analise do Grafo ---\n");
    printf("Grau Maximo: %d\n", maxDeg);
    printf("Grau Minimo: %d\n", minDeg);
    
    if (loops > 0 || multipleEdges > 0) {
        printf("Tipo: Multigrafo\n");
        printf("Lacos: %d\n", loops);
        printf("Arestas Multiplas: %d\n", multipleEdges / 2); // /2 pois contamos ida e volta
    } else {
        printf("Tipo: Grafo Simples\n");
    }

    // Requisito e: Componentes Conexos
    int numComponents = 0;
    printf("\n--- Componentes Conexos ---\n");
    for (int i = 0; i <= maxId; i++) {
        if (graph->visited[i] == 0 && graph->degrees[i] > 0) {
            int componentSize = 0;
            numComponents++;
            DFS(graph, i, &componentSize);
            printf("Componente %d: Tamanho %d\n", numComponents, componentSize);
        }
    }
    printf("Total de Componentes: %d\n", numComponents);

    return 0;
}