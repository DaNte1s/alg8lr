#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <queue>
#include <chrono>
#include <windows.h>

using namespace std;

bool show = false;

struct Node {
    int vertex;
    struct Node* next;
};

struct Queue {
    struct Node* head;
    struct Node* tail;
};

void queue_init(struct Queue* q) {
    q->head = NULL;
    q->tail = NULL;
}

bool queue_is_empty(struct Queue* q) {
    return q->head == NULL;
}

void queue_push(struct Queue* q, int v) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->vertex = v;
    newNode->next = NULL;
    if (q->tail == NULL) {
        q->head = newNode;
        q->tail = newNode;
    } else {
        q->tail->next = newNode;
        q->tail = newNode;
    }
}

int queue_pop(struct Queue* q) {
    if (q->head == NULL) return -1;
    struct Node* temp = q->head;
    int v = temp->vertex;
    q->head = q->head->next;
    if (q->head == NULL) {
        q->tail = NULL;
    }
    free(temp);
    return v;
}

void queue_free(struct Queue* q) {
    while (q->head != NULL) {
        struct Node* temp = q->head;
        q->head = q->head->next;
        free(temp);
    }
    q->tail = NULL;
}

int** generate_adjacency_matrix(int size, int edge_percent) {
    int **G = (int **)malloc(size * sizeof(int *));
    for (int i = 0; i < size; i++) {
        G[i] = (int *)malloc(size * sizeof(int));
    }

    for (int i = 0; i < size; i++) {
        G[i][i] = 0;
        for (int j = i + 1; j < size; j++) {
            int val = (rand() % 100 < edge_percent) ? 1 : 0;
            G[i][j] = val;
            G[j][i] = val;
        }
    }
    return G;
}

void free_matrix(int **G, int size) {
    if (!G) return;
    for (int i = 0; i < size; i++) {
        free(G[i]);
    }
    free(G);
}

void print_matrix(int **G, int size) {
    printf("     ");
    for (int j = 0; j < size; j++) {
        printf("v%-3d", j);
    }
    printf("\n");

    for (int i = 0; i < size; i++) {
        printf("v%-3d ", i);
        for (int j = 0; j < size; j++) {
            printf("%-4d", G[i][j]);
        }
        printf("\n");
    }
}

struct Node* createNode(int v) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->vertex = v;
    newNode->next = NULL;
    return newNode;
}

void push_node(struct Node** head, int v) {
    struct Node* newNode = createNode(v);
    if (*head == NULL) {
        *head = newNode;
    } else {
        struct Node* temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

void pop_node(struct Node** head) {
    if (*head == NULL) return;
    struct Node* temp = *head;
    *head = (*head)->next;
    free(temp);
}

void print_lists(struct Node** adjLists, int size) {
    for (int i = 0; i < size; i++) {
        printf("v%d: ", i);
        struct Node* temp = adjLists[i];
        while (temp != NULL) {
            printf("%d ", temp->vertex);
            temp = temp->next;
        }
        printf("\n");
    }
}

void bfs_matrix_queue(int v, int size, int **G, bool *NUM) {
    queue<int> Q;
    Q.push(v);
    NUM[v] = true;

    while (!Q.empty()) {
        v = Q.front();
        Q.pop();
        if (show) printf("%d ", v);

        for (int i = 0; i < size; i++) {
            if (G[v][i] == 1 && !NUM[i]) {
                Q.push(i);
                NUM[i] = true;
            }
        }
    }
}

void run_task_1(int size, int **G, int start) {
    bool *NUM = (bool *)calloc(size, sizeof(bool));

    if (show) {
        printf("\nОбход в ширину (Матрица смежности)\n");
        printf("Стартовая вершина: %d\n", start);
        printf("Порядок обхода вершин: ");
    }

    if (!NUM[start]) {
        bfs_matrix_queue(start, size, G, NUM);
    }

    for (int i = 0; i < size; i++) {
        if (!NUM[i]) {
            bfs_matrix_queue(i, size, G, NUM);
        }
    }

    if (show) printf("\n");
    free(NUM);
}

void bfs_list_queue(int v, struct Node** adjLists, bool *NUM) {
    queue<int> Q;
    Q.push(v);
    NUM[v] = true;

    while (!Q.empty()) {
        v = Q.front();
        Q.pop();
        if (show) printf("%d ", v);

        struct Node* temp = adjLists[v];
        while (temp != NULL) {
            int adjVertex = temp->vertex;
            if (!NUM[adjVertex]) {
                Q.push(adjVertex);
                NUM[adjVertex] = true;
            }
            temp = temp->next;
        }
    }
}

void run_task_1_lists(int size, int **G, int start) {
    struct Node** adjLists = (struct Node**)malloc(size * sizeof(struct Node*));
    for (int i = 0; i < size; i++) {
        adjLists[i] = NULL;
    }

    for (int i = 0; i < size; i++) {
        struct Node* tail = NULL;
        for (int j = 0; j < size; j++) {
            if (G[i][j] == 1) {
                struct Node* newNode = createNode(j);
                if (adjLists[i] == NULL) {
                    adjLists[i] = tail = newNode;
                } else {
                    tail->next = newNode;
                    tail = newNode;
                }
            }
        }
    }

    if (show) {
        printf("\nСписки смежности графа:\n");
        print_lists(adjLists, size);
        printf("\nОбход в ширину (Списки смежности)\n");
        printf("Стартовая вершина: %d\n", start);
        printf("Порядок обхода вершин: ");
    }

    bool *NUM = (bool *)calloc(size, sizeof(bool));

    if (!NUM[start]) {
        bfs_list_queue(start, adjLists, NUM);
    }

    for (int i = 0; i < size; i++) {
        if (!NUM[i]) {
            bfs_list_queue(i, adjLists, NUM);
        }
    }

    if (show) printf("\n");

    for (int i = 0; i < size; i++) {
        while (adjLists[i] != NULL) {
            pop_node(&adjLists[i]);
        }
    }

    free(adjLists);
    free(NUM);
}

void bfs_custom_queue(int v, int size, int **G, bool *NUM) {
    struct Queue Q;
    queue_init(&Q);

    queue_push(&Q, v);
    NUM[v] = true;

    while (!queue_is_empty(&Q)) {
        v = queue_pop(&Q);
        if (show) printf("%d ", v);

        for (int i = 0; i < size; i++) {
            if (G[v][i] == 1 && !NUM[i]) {
                queue_push(&Q, i);
                NUM[i] = true;
            }
        }
    }
    queue_free(&Q);
}

void run_task_2(int size, int **G, int start) {
    bool *NUM = (bool *)calloc(size, sizeof(bool));

    if (show) {
        printf("\nОбход в ширину (Матрица смежности)\n");
        printf("Стартовая вершина: %d\n", start);
        printf("Порядок обхода вершин: ");
    }

    if (!NUM[start]) {
        bfs_custom_queue(start, size, G, NUM);
    }

    for (int i = 0; i < size; i++) {
        if (!NUM[i]) {
            bfs_custom_queue(i, size, G, NUM);
        }
    }

    if (show) printf("\n");
    free(NUM);
}

void bfs_list_custom_queue(int v, struct Node** adjLists, bool *NUM) {
    struct Queue Q;
    queue_init(&Q);

    queue_push(&Q, v);
    NUM[v] = true;

    while (!queue_is_empty(&Q)) {
        v = queue_pop(&Q);
        if (show) printf("%d ", v);

        struct Node* temp = adjLists[v];
        while (temp != NULL) {
            int adjVertex = temp->vertex;
            if (!NUM[adjVertex]) {
                queue_push(&Q, adjVertex);
                NUM[adjVertex] = true;
            }
            temp = temp->next;
        }
    }
    queue_free(&Q);
}

void run_task_2_lists(int size, int **G, int start) {
    struct Node** adjLists = (struct Node**)malloc(size * sizeof(struct Node*));
    for (int i = 0; i < size; i++) {
        adjLists[i] = NULL;
    }

    for (int i = 0; i < size; i++) {
        struct Node* tail = NULL;
        for (int j = 0; j < size; j++) {
            if (G[i][j] == 1) {
                struct Node* newNode = createNode(j);
                if (adjLists[i] == NULL) {
                    adjLists[i] = tail = newNode;
                } else {
                    tail->next = newNode;
                    tail = newNode;
                }
            }
        }
    }

    bool *NUM = (bool *)calloc(size, sizeof(bool));

    if (show) {
        printf("\nОбход в ширину (Списки смежности)\n");
        printf("Стартовая вершина: %d\n", start);
        printf("Порядок обхода вершин: ");
    }

    if (!NUM[start]) {
        bfs_list_custom_queue(start, adjLists, NUM);
    }

    for (int i = 0; i < size; i++) {
        if (!NUM[i]) {
            bfs_list_custom_queue(i, adjLists, NUM);
        }
    }

    if (show) printf("\n");

    for (int i = 0; i < size; i++) {
        while (adjLists[i] != NULL) {
            pop_node(&adjLists[i]);
        }
    }

    free(adjLists);
    free(NUM);
}

double get_time() {
    return chrono::duration<double>(chrono::high_resolution_clock::now().time_since_epoch()).count();
}

int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    srand((unsigned int)time(NULL));

    while (1) {
        printf("\n--- Меню ---\n");
        printf("1. Вывод матрицы и обход графа\n");
        printf("2. Только замер времени\n");
        printf("0. Выход\n");
        printf("Выберите пункт: ");

        int choice;
        if (scanf("%d", &choice) != 1 || choice == 0) break;
        if (choice != 1 && choice != 2) continue;

        show = (choice == 1);

        int size;
        int edge_percent;
        int start;

        printf("Введите количество вершин графа: ");
        if (scanf("%d", &size) != 1 || size <= 0) break;

        printf("Введите вероятность появления ребра в процентах (от 0 до 100): ");
        if (scanf("%d", &edge_percent) != 1 || edge_percent < 0 || edge_percent > 100) break;

        int **G = generate_adjacency_matrix(size, edge_percent);

        if (show) {
            printf("\nСгенерированная матрица смежности G (%d вершин, вероятность ребра %d%%):\n",
                   size, edge_percent);
            print_matrix(G, size);
        }

        printf("Введите начальную вершину (от 0 до %d): ", size - 1);
        if (scanf("%d", &start) != 1 || start < 0 || start >= size) start = 0;

        double t1, t2;

        printf("\nЗАДАНИЕ №1: Использование очереди из стандартной библиотеки queue\n");

        t1 = get_time();
        run_task_1(size, G, start);
        t2 = get_time();
        if (show) printf("Время выполнения: %.6f сек.\n", t2 - t1);
        else printf("Матрица смежности: %.6f сек.\n", t2 - t1);

        t1 = get_time();
        run_task_1_lists(size, G, start);
        t2 = get_time();
        if (show) printf("Время выполнения: %.6f сек.\n", t2 - t1);
        else printf("Списки смежности:  %.6f сек.\n", t2 - t1);

        printf("\nЗАДАНИЕ №2: Использование пользовательской очереди\n");

        t1 = get_time();
        run_task_2(size, G, start);
        t2 = get_time();
        if (show) printf("Время выполнения: %.6f сек.\n", t2 - t1);
        else printf("Матрица смежности: %.6f сек.\n", t2 - t1);

        t1 = get_time();
        run_task_2_lists(size, G, start);
        t2 = get_time();
        if (show) printf("Время выполнения: %.6f сек.\n", t2 - t1);
        else printf("Списки смежности:  %.6f сек.\n", t2 - t1);

        free_matrix(G, size);
    }

    printf("\n");
    system("pause");
    return 0;
}
