//
// Created by bctho on 12/11/2023.
//
#include "stdio.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>

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

// Fonction pour afficher la matrice
void afficherMatrice(char matrice[LIGNES][COLONNES]) {
    system("cls || clear"); // Effacer la console (compatible avec Windows et Linux)
    for (int i = 0; i < LIGNES; i++) {
        for (int j = 0; j < COLONNES; j++) {
            printf("%c ", matrice[i][j]);
        }
        printf("\n");
    }
}

void regles() {
    printf("les regles du jeux sont:\nLe but est de recuperer les 4 oiseaux du niveau sans se faire toucher par la balle et/ou les ennemis sinon vous perdez une vie pour gagner et passez automatiquement au niveau suivant en revanche si vous perdez toutes vos vies vous avez perdu et êtes redirige au menu principal. \n"
           "Vous avez 3 vies par niveau, chaque niveau doit être fini en 120 seconde maximum, si le temp est ecoule vous perdez une vie et vous recommencez le niveau.\n"
           "Vous ne pouvez-vous deplacer qu’a droite a gauche en haut et en bas et vous ne pouvez avancer que d’une case à la fois si un obstacle est present vous ne pouvez pas effectuer votre déplacement.\n"
           "A savoir que la balle ne se déplace qu’exclusivement en diagonale et rebondit uniquement sur les murs sa vitesse est fixe. La balle « traverse » tous les obstacles du terrain sans changer de direction.");
}

// Fonction pour casser un bloc dans la matrice
void casserBloc(char matrice[LIGNES][COLONNES], int ligne, int colonne) {
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int nouvelleLigne = ligne + i;
            int nouvelleColonne = colonne + j;

// Vérifier les limites de la matrice
            if (nouvelleLigne >= 0 && nouvelleLigne < LIGNES && nouvelleColonne >= 0 && nouvelleColonne < COLONNES) {
                if (matrice[nouvelleLigne][nouvelleColonne] == BLOC) {
                    printf("Bloc en position (%d, %d) cassé !\n", nouvelleLigne, nouvelleColonne);
                    matrice[nouvelleLigne][nouvelleColonne] = VIDE;
                }
            }
        }
    }
}

// Fonction pour réinitialiser la position du personnage au centre de la carte
void reinitialiserPersonnage(char matrice[LIGNES][COLONNES], int *lignePersonnage, int *colonnePersonnage) {
    matrice[*lignePersonnage][*colonnePersonnage] = VIDE;
    *lignePersonnage = LIGNES / 2;
    *colonnePersonnage = COLONNES / 2;
    matrice[*lignePersonnage][*colonnePersonnage] = PERSONNAGE;
}

int getch() {
#ifdef _WIN32
    return _getch();
#endif
}

int partie() {
    // Initialisation de la matrice
    char matrice[LIGNES][COLONNES];
    for (int i = 0; i < LIGNES; i++) {
        for (int j = 0; j < COLONNES; j++) {
            matrice[i][j] = VIDE;
        }
    }

    // Initialisation des oiseaux
    matrice[0][0] = OISEAU;
    matrice[0][COLONNES - 1] = OISEAU;
    matrice[LIGNES - 1][0] = OISEAU;
    matrice[LIGNES - 1][COLONNES - 1] = OISEAU;

    // Initialisation des blocs
    srand(time(NULL)); // Initialiser la graine du générateur aléatoire avec le temps actuel
    for (int i = 0; i < 10; i++) {
        int ligne = rand() % LIGNES;
        int colonne = rand() % COLONNES;

        // Assurer que la position est vide avant de placer le bloc
        while (matrice[ligne][colonne] != VIDE) {
            ligne = rand() % LIGNES;
            colonne = rand() % COLONNES;
        }

        matrice[ligne][colonne] = BLOC;
    }

    // Initialisation des blocs pièges
    for (int i = 0; i < 5; i++) {
        int ligne = rand() % LIGNES;
        int colonne = rand() % COLONNES;

        // Assurer que la position est vide avant de placer le bloc piège
        while (matrice[ligne][colonne] != VIDE) {
            ligne = rand() % LIGNES;
            colonne = rand() % COLONNES;
        }

        matrice[ligne][colonne] = BLOC_PIEGE;
    }

    // Initialisation du personnage
    int lignePersonnage = LIGNES / 2;
    int colonnePersonnage = COLONNES / 2;
    matrice[lignePersonnage][colonnePersonnage] = PERSONNAGE;

    // Initialisation du timer
    time_t debut = time(NULL);
    int dureeLimite = 120;
    int vies = 3;
    // Nombre d'oiseaux récupérés
    int oiseauxRecuperes = 0;

    // Affichage initial de la matrice
    afficherMatrice(matrice);

    // Déplacement du personnage et interaction avec les blocs
    char deplacement;

    printf("\nUtilisez les touches 'w', 'a', 's', 'd' pour vous déplacer, 'p' pour casser un bloc et 'q' pour quitter.\n");

    while (1) {
        // Vérifier le nombre de vies restantes
        if (vies <= 0) {
            printf("Game Over ! Vous avez épuisé toutes vos vies.\n");
            break;
        }

        // Vérifier le nombre d'oiseaux récupérés
        if (oiseauxRecuperes == 4) {
            printf("Félicitations ! Vous avez récupéré tous les oiseaux. Vous avez gagné !\n");
            break;
        }

        // Vérifier le temps écoulé
        time_t maintenant = time(NULL);
        int tempsEcoule = (int)difftime(maintenant, debut);

        if (tempsEcoule > dureeLimite) {
            // Temps écoulé, perdre une vie
            vies--;
            printf("Temps écoulé ! Vous perdez une vie. Vies restantes : %d\n", vies);

            // Réinitialiser le timer
            debut = time(NULL);

            // Réinitialiser la position du personnage
            reinitialiserPersonnage(matrice, &lignePersonnage, &colonnePersonnage);
        }

        // Afficher le temps restant
        printf("Temps restant : %d secondes\n", dureeLimite - tempsEcoule);
        // Afficher les vies restantes
        printf("Il vous reste : %d vie(s)\n", vies);
        // Afficher le nombre d'oiseaux récupérés
        printf("Oiseaux récupérés : %d\n", oiseauxRecuperes);

        // Extraire le premier caractère de la ligne
        deplacement = getch();

        // Vérifier si le personnage peut se déplacer
        int nouvelleLigne = lignePersonnage;
        int nouvelleColonne = colonnePersonnage;

        switch (deplacement) {
            case 'w':
                nouvelleLigne--;
                break;
            case 'a':
                nouvelleColonne--;
                break;
            case 's':
                nouvelleLigne++;
                break;
            case 'd':
                nouvelleColonne++;
                break;
            case 'p':
                // Casser les blocs autour du personnage
                casserBloc(matrice, lignePersonnage, colonnePersonnage);
                break;
            case 'q':
                // Quitter le jeu
                printf("Vous avez quitté le jeu.\n");
                return 0;
            default:
                printf("Touche non valide. Utilisez les touches 'w', 'a', 's', 'd' pour vous déplacer, 'p' pour casser un bloc et 'q' pour quitter.\n");
                break;
        }

        // Vérifier si la nouvelle position est valide
        if (nouvelleLigne >= 0 && nouvelleLigne < LIGNES && nouvelleColonne >= 0 && nouvelleColonne < COLONNES) {
            // Vérifier si le personnage touche un bloc piège
            if (matrice[nouvelleLigne][nouvelleColonne] == BLOC_PIEGE) {
                printf("Vous avez touché un bloc piège ! Vous perdez une vie.\n");
                vies--;

                // Réinitialiser la position du personnage
                reinitialiserPersonnage(matrice, &lignePersonnage, &colonnePersonnage);
            } else if (matrice[nouvelleLigne][nouvelleColonne] != BLOC) {
                // Déplacer le personnage seulement si la nouvelle position n'est pas un bloc
                matrice[lignePersonnage][colonnePersonnage] = VIDE;
                lignePersonnage = nouvelleLigne;
                colonnePersonnage = nouvelleColonne;
                matrice[lignePersonnage][colonnePersonnage] = PERSONNAGE;
            }
        }

        // Vérifier si l'oiseau est récupéré
        if (matrice[nouvelleLigne][nouvelleColonne] == OISEAU) {
            printf("Oiseau récupéré !\n");
            matrice[nouvelleLigne][nouvelleColonne] = VIDE; // Marquer l'oiseau comme récupéré
            oiseauxRecuperes++;
            if (oiseauxRecuperes == 4) {
                printf("Félicitations ! Vous avez récupéré tous les oiseaux. Vous avez gagné !\n");
                break;
            }
        }

        // Afficher la matrice mise à jour
        afficherMatrice(matrice);
    }

    return 0;
}

int menu(){
    int choix;
    do {
        printf("Menu :\n 1.afficher les règles 1\n 2.Lancer un nouveau Jeu à partir du niveau 1\n 3.Charger une partie 4\n 4. mot de passe\n 5. scores\n 6. quitter\n");
        scanf("%d", &choix);
        if(choix<1 || choix>6){
            printf("erreur.\n");
        }
    } while (choix<1 || choix>6);
    return  choix;
}
void mdp(){
    int choix1;
    char mdp[]="idk";
    char testmdp[100];
    printf("votre mdp ?");
    scanf("%s",testmdp);
    if(strcmp(mdp,testmdp)==0){
        do {
            printf("1. charger le niveau 1 \n 2. charger le niveau 2\n 3. charger le niveau 3\n 4. retour\n quelle est votre choix :");
            scanf("%d", &choix1);
        } while (choix1 < 1 || choix1 > 4);
        switch (choix1) {
            case 1:
                printf("1");//charger niveau 1
                break;
            case 2:
                printf("2");//charger niveau 2
                break;
            case 3:
                printf("3");//charger niveau 3
                break;
            case 4 :
                printf("r");//retour menu
                break;
        }
    }
    else{
        printf("mauvais mdp");
    }
}
