#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#define TAM_MAX 1000



typedef int **matriz;

void inicializar_semilla();
void imprimirMatriz(int **matriz, int tam);
double microsegundos();
void dijkstra(matriz grafo, matriz distancias, int tam);
matriz crearMatriz(int n);
void iniMatriz(matriz m, int n);
void liberarMatriz(matriz m, int n);
void test();
void analizarTiempos(void (*algoritmoDijkstra)(matriz grafo, matriz distancias, int tam),void (*generador)(matriz m, int n), int n);
void calculosTiempos(int n, double time);
void ComplejiadDijkstra();




void inicializar_semilla() {
    srand(time(NULL));
}

void imprimirMatriz(int **matriz, int tam) {
    int i;
    int j;
    for (i = 0; i < tam; i++) {
        for (j = 0; j < tam; j++) {
            printf("%d\t", matriz[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

double microsegundos(){
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0 )
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

void dijkstra(matriz grafo, matriz distancias, int tam) {
    int n, i, j, min, v = 0;
    int *noVisitados = malloc(tam * sizeof(int));

    for (n = 0; n < tam; n++) {
        for (i = 0; i < tam; i++) {
            noVisitados[i] = 1;
            distancias[n][i] = grafo[n][i];
        }
        noVisitados[n] = 0;

        for (i = 0; i < tam - 2; i++) {
            /* Encontrar el nodo no visitado con la distancia mínima */
            min = TAM_MAX;
            for (j = 0; j < tam; j++) {
                if (noVisitados[j] && distancias[n][j] < min) {
                    min = distancias[n][j];
                    v = j;
                }
            }

            noVisitados[v] = 0; /* Marcar el nodo como visitado */

            /* Actualizar las distancias a los nodos adyacentes no visitados */
            for (j = 0; j < tam; j++) {
                if (noVisitados[j] && distancias[n][j] > distancias[n][v] + grafo[v][j]) {
                    distancias[n][j] = distancias[n][v] + grafo[v][j];
                }
            }
        }
    }

    free(noVisitados);
}

matriz crearMatriz(int n) {
    int i;
    matriz aux;
    if ((aux = malloc(n*sizeof(int *))) == NULL)
        return NULL;
    for (i=0; i<n; i++)
        if ((aux[i] = malloc(n*sizeof(int))) == NULL)
            return NULL;
    return aux;
}

/* Inicializacion pseudoaleatoria [1..TAM_MAX] de un grafo completo
no dirigido con n nodos, representado por su matriz de adayencia */

void iniMatriz(matriz m, int n) {
    int i, j;
    for (i=0; i<n; i++)
        for (j=i+1; j<n; j++)
            m[i][j] = rand() % TAM_MAX + 1;
    for (i=0; i<n; i++)
        for (j=0; j<=i; j++)
            if (i==j)
                m[i][j] = 0;
            else
                m[i][j] = m[j][i];
}

void liberarMatriz(matriz m, int n) {
    int i;
    for (i=0; i<n; i++)
        free(m[i]);
    free(m);
}

void test() {
    matriz matriz1;
    matriz matriz2;
    matriz matriz1Distancias;
    matriz matriz2Distancias;
    int i;
    int j;

    int matriz1Valores[5][5] = {
            {0, 1, 8, 4, 7},
            {1, 0, 2, 6, 5},
            {8, 2, 0, 9, 5},
            {4, 6, 9, 0, 3},
            {7, 5, 5, 3, 0}
    };

    int matriz2Valores[4][4] = {
            {0, 1, 4, 7},
            {1, 0, 2, 8},
            {4, 2, 0, 3},
            {7, 8, 3, 0}
    };

    // Crear matrices y asignar valores
    matriz1 = crearMatriz(5);
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            matriz1[i][j] = matriz1Valores[i][j];
        }
    }

    matriz2 = crearMatriz(4);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            matriz2[i][j] = matriz2Valores[i][j];
        }
    }

    // Matrices de distancias
    matriz1Distancias = crearMatriz(5);
    matriz2Distancias = crearMatriz(4);

    dijkstra(matriz1, matriz1Distancias, 5);
    dijkstra(matriz2, matriz2Distancias, 4);

    printf("Matriz adyacente 1:\n");
    imprimirMatriz(matriz1, 5);

    printf("Matriz adyacente 2:\n");
    imprimirMatriz(matriz2, 4);

    printf("\n\n");

    // Mostrar las matrices resultantes
    printf("Matriz de distancias minimas 1:\n");
    imprimirMatriz(matriz1Distancias, 5);

    printf("Matriz de distancias minimas 2:\n");
    imprimirMatriz(matriz2Distancias, 4);

    // Liberar matrices
    liberarMatriz(matriz1, 5);
    liberarMatriz(matriz2, 4);
    liberarMatriz(matriz1Distancias, 5);
    liberarMatriz(matriz2Distancias, 4);
}

void analizarTiempos(void (*algoritmoDijkstra)(matriz grafo, matriz distancias, int tam),void (*generador)(matriz m, int n), int n) {
    inicializar_semilla();
    int k;
    double elapsed_time1, start_time, end_time, t1, t2;
    matriz grafo;
    matriz distancias;

    grafo = crearMatriz(n);
    distancias = crearMatriz(n);

    generador(grafo, n);

    start_time = microsegundos();
    algoritmoDijkstra(grafo, distancias, n);
    end_time = microsegundos();
    elapsed_time1 = end_time - start_time;

    if (elapsed_time1 < 500) {
        printf("(*)");
        start_time = microsegundos();

        for (k = 0; k < 100; k++) {
            generador(grafo, n);
            algoritmoDijkstra(grafo, distancias, n);
        }
        end_time = microsegundos();
        t1 = end_time - start_time;
        start_time = microsegundos();

        for (k = 0; k < 100; k++) {
            generador(grafo, n);
        }
        end_time = microsegundos();
        t2 = end_time - start_time;

        elapsed_time1 = (t1 - t2) / 100;
        calculosTiempos(n, elapsed_time1);

    } else {
        calculosTiempos(n, elapsed_time1);
    }

    liberarMatriz(grafo, n);
    liberarMatriz(distancias, n);
}

void calculosTiempos(int n, double time){
    double time1, time2, time3;

    time1 = time / (pow(n, 2.6));
    time2 = time / (pow(n, 2.82));
    time3 = time / (pow(n, 3));

    printf("\t% 8d\t\t% 15.3lf\t\t% 14.8lf\t\t% 14.8lf\t\t% 14.8lf\n", n, time, time1, time2, time3);
}

void ComplejiadDijkstra() {
    int n;
    for (n = 10; n <= TAM_MAX; n *= 2) {
        analizarTiempos(dijkstra, iniMatriz, n);
    }
}




int main() {
    inicializar_semilla();

    ///Test
    printf("Test de los grafos Figura 2 y Figura 3:\n\n");
    test();

    printf("\n\n\n");

    ///Análisis de la complejidad de Dijkstra
    printf("Análisis de la complejidad de Dijkstra\n");
    printf("\t       n\t\t\t  t(n)\t\t   t(n) / n^2.6\t\t   t(n) / n^2.82\t           t(n) / n^3\n");
    ComplejiadDijkstra();




    return 0;
}
