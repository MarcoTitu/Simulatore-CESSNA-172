#ifndef INTERPOL_H
#define INTERPOL_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double interpola(double alpha[126], double der_aerodinamica[126], double alpha_des) {
    extern int indice_numero_vicino;

    int righe = 126;
    double differenza = alpha_des - alpha[0];
    int i=1;

    // Trova l'indice del vettore degli alpha, il cui valore � quel valore tabulato nel database aerodinamico che �
    // piu' vicino ad alfa desiderato e, contemporaneamente, minore di alfa desiderato
    while(differenza >= 0 && i<126){
        differenza = alpha_des - alpha[i];
        i++;
    }
    indice_numero_vicino=i-2;

    // Calcola la derivata aerodinamica interpolata linearmente
    double delta_alpha = alpha_des - alpha[indice_numero_vicino];
    double delta = (der_aerodinamica[indice_numero_vicino + 1] - der_aerodinamica[indice_numero_vicino]) / (alpha[indice_numero_vicino + 1] - alpha[indice_numero_vicino]);
    double derivata_interpolata = der_aerodinamica[indice_numero_vicino] + delta * delta_alpha;

    return derivata_interpolata;
}

#endif
