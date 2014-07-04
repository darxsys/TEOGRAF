/**
    CS Teorija grafova
    hamilton.cpp
    Svrha: Implementira otkrivanje je li graf skoro hamiltonovski.

    @author Dario Pavlovic, 0036455992
    @version 1.0 28/03/2014
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;

// pomocno polje za algoritam dinamickog programiranja, objasnjeno kasnije
int *dp;
typedef long long int lld;

/**
    Funkcija provjerava je li broj potencija broja 2,
    tj. ima li jedinicu na samo jednom mjestu.

    @param Broj
    @return Izlaz - logicka vrijednost
*/

bool is_power_two(lld x) {
   return x && !(x & (x - 1));
 }

/**
    Funkcija ispisuje jednu mogucu skoro hamiltonovsku stazu.

    @param 
    mask - najveca moguca maska za trenutni graf. Uloga
    maske objasnjenja u sljedecoj funkciji.
    N - broj vrhova trenutnog grafa.

    @return Ne vraca izlaz vec ispisuje putanju na stdout.
    Ako path ne postoji, ne ispisuje nista.
*/
void print_path(int *graph, lld mask, int N) {
    int current = -1;
    ostringstream ss;
    mask--;
    for (int i = 0; i < N; i++) {
        if (dp[mask * N + i] == 1) {
            current = i;
            break;
        }
    }

    if (current == -1) {
        return;
    }

    ss << (current + 1);
    // lld new_mask;
    mask = mask ^ (1<<current);
    while (mask > 0) {
        for (int i = 0; i < N; i++) {
            if (i == current) {
                continue;
            }

            if (dp[mask * N + i] == 1 && graph[current * N + i] == 1) {
                current = i;
                mask = mask ^ (1<<current);
                ss << " --> " << (current + 1);
            }
        }
    }

    cout << ss.str() << endl;
}

/**
    Funkcija provjerava je li ovaj graf skoro hamiltonovski.
    Funkcija koristi dinamicko programiranje u slozenosti O(2^N * N^2).

    Ideja je koristiti bit maske i pomocno polje dp. Bit maskama se generiraju
    svi moguci podskupovi skupa vrhova N, a u polje dp sprema se sljedece:
    dp[mask][i] sadrzi informaciju postoji li u trenutnom podskupu cvorova
    kojeg generira maska hamiltonovski put koji zavrsava u cvoru i.

    @param graph -> matrica susjedstva, N - broj vrhova
    @return izlazni status - 0 nije, 1 jest skoro hamiltonovski
*/
int check_hamiltonian(int *graph, int N) {
    lld mask = 1 << N;
    dp = (int*) malloc(sizeof(int) * mask * N);

    // inicijaliziraj prvi red dp-a na sve nule.
    for (int i = 0; i < N; i++) {
        dp[i] = 0;
    }

    // glavni dio dinamike
    for (lld i = 1; i < mask; i++) {
        for (int j = 0; j < N; j++) {
            dp[i * N + j] = 0;
            // ako vrh ne pripada trenutnom podskupu, preskoci ga
            if (((1<<j) & i) == 0) {
                continue;
            }

            // dinamika se sastoji u tome da staza kroz trenutnih vrh postoji
            // ako postoji kroz susjede koji pripadaju trenutnom podskupu
            for (int k = 0; k < N; k++) {
                if (graph[j * N + k] == 0 || ((1<<k) & i) == 0) {
                    continue;
                }
                dp[i * N + j] |= (dp[(i ^ (1<<j)) * N + k]);
            }

            // posljednji slucaj, kad je samo ovaj vrh element
            // podskupa vrhova
            if (dp[i * N + j] == 0 && is_power_two(i)) {
                dp[i * N + j] = 1;
            }
        }
    }

    // provjeri postoji li staza kroz sve vrhove (zavrsava u bilo kojem)
    int result = 0;

    for (int i = 0; i < N; i++) {
        result |= dp[(mask-1) * N + i];
    }
    return result;
}

/**
    Main radi inicijalizaciju svih potrebnih varijabli i velicina.

    @param parametar komandne linije - put do ulazne datoteke
    @return izlazni status
*/
int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "ERROR: Putanja do datoteke potrebna.\n");
        exit(1);
    }

    FILE *input = fopen(argv[1], "r");
    if (input == NULL) {
        fprintf(stderr, "ERROR: Neuspjesno otvaranje ulazne datoteke.\n");
        exit(1);    
    }

    int N;
    fscanf(input, "%d", &N);
    int graph[N][N];

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            fscanf(input, "%d", &graph[i][j]);
        }
    }

    printf("%d\n", check_hamiltonian(&graph[0][0], N));
    lld mask = (1<<N);
    print_path(&graph[0][0], mask, N);
    if (dp != NULL) {
        free(dp);
    }
    
    fclose(input);
    return 0;
}
