/**
    CS Teorija grafova
    chrom.cpp
    Svrha: Implementira trazenje kromatskog broja i realizaciju bojanja u jednostavnom grafu.

    @author Dario Pavlovic, 0036455992
    @version 1.0 10/05/2014
*/

#include <stdio.h>
#include <stdlib.h>

/**
    Glavna funkcija u cijelom rjesenju. 
    Rekurzija koja pokusava svaki cvor obojati nekom od K dostupnih boja.
    Ako to ne uspije, vrati false. U svakoj razini, ona provjerava jedan cvor,
    te ako nema boja kojom bi se mogao obojati taj cvor, vrati false. Inace vrati
    true prethodnoj razini.

    @param graph matrica susjedstva
    @param N broj cvorova u grafu
    @param K maksimalni broj boja
    @depth trenutna dubina rekurzije i oznaka cvora
    @colors polje indikatora koji je cvor obojan kojom bojom
    @return true ili false, ovisno o tome je li graf moguce obojati
*/

bool find(int *graph, int N, int K, int depth, int *colors) {
    if (depth >= N) {
        return true;
    }

    bool cont = false;

    for (int i = 1; i <= K; i++) {
        // try coloring
        colors[depth] = i;

        // check if neighbors are okay
        for (int j = 0; j < N; j++) {
            if (j == depth) continue;

            // if neighbor, then check 
            if (graph[depth * N + j] &&  colors[depth] == colors[j]) {
                cont = true;
                break;
            }
        }

        if (cont) {
            cont = false;
            continue;
        }

        if (find(graph, N, K, depth+1, colors)) {
            colors[depth] = 0;
            return true;
        }
    }

    colors[depth] = 0;
    return false;
}

/**
    Funkcija koja poziva rekurzije za svaki moguci kromatski broj pocev od 1.

    @param graph matrica susjedstva
    @param N broj cvorova u grafu
    @return kromatski broj grafa
*/
int find_chrom(int *graph, int N) {

    // int K = 1;

    int sol = N;
    int *colors = (int*) malloc(sizeof(int) * N);
    for (int i = 0; i < N; i++) {
        colors[i] = 0;
    }

    for (int K = 1; K <= N-1; K++) {
        if (find(graph, N, K, 0, colors)) {
            sol = K;
            break;
        }
    }

    free(colors);
    return sol;
}


/**
    Main radi inicijalizaciju svih potrebnih varijabli i velicina.

    @param put put do datoteke
    @return status izlazni status
*/
int main(int argc, char **argv) {
    // read the graph
    if (argc != 2) {
        printf("Path to the graph needed.\n");
        exit(1);
    }

    char *path = argv[1];
    FILE *f = fopen(path, "r");
    if (f == NULL) {
        printf("Invalid path.\n");
        exit(1);
    }

    int N;
    fscanf(f, "%d", &N);
    int *graph = (int*) malloc(sizeof(int) * N * N);

    // if graph is complete, result is known immediately
    bool is_complete = true;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            fscanf(f, "%d", &(graph[i * N + j]));
            if (i != j && graph[i * N + j] != 1) {
                is_complete = false;
            }
        }
    }

    fclose(f);
    if (is_complete) {
        printf("%d\n", N);
        free(graph);
        return 0;
    }

    printf("%d\n", find_chrom(graph, N));
    free(graph);
    return 0;
}