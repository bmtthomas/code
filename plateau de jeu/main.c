#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "header.h"
#ifdef _WIN32
#include <conio.h>
#elif __linux__
#include <unistd.h>
#include <termios.h>
#endif
// Définition des dimensions de la matrice
#define LIGNES 10
#define COLONNES 20

// Caractères pour représenter les éléments dans la matrice
#define PERSONNAGE 'P'
#define OISEAU 'O'
#define BLOC 'B'
#define BLOC_PIEGE 'X' // Nouveau caractère pour les blocs pièges
#define VIDE '.'

int main(){
    int choix;
    do{
        choix=menu();
        switch (choix) {
            case 1:
                regles();
                break;
            case 2:
                partie();
                break;
            case 3:

                break;
            case 4:
                mdp();
                break;
            case 5:

                break;
                //fonction pour quitter le jeu
            case 6:

                break;
        }
    } while (choix !=6);
}