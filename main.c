#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "film.h"

#define NOM_FICHIER "catalogue.csv"


/* afficherMenu : affiche le menu textuel */
void afficherMenu() {
    printf("========================================\n");
    printf("     CATALOGUE PERSONNEL DE FILMS       \n");
    printf("========================================\n");
    printf("  1. Ajouter un film\n");
    printf("  2. Afficher tous les films\n");
    printf("  3. Rechercher par ID\n");
    printf("  4. Rechercher par titre\n");
    printf("  5. Afficher par genre\n");
    printf("  6. Modifier un film\n");
    printf("  7. Supprimer un film\n");
    printf("----------------------------------------\n");
    printf("  8. Trier par note décroissante\n");
    printf("  9. Trier par ordre alphabétique\n");
    printf(" 10. Top 5 des films\n");
    printf(" 11. Statistiques\n");
    printf("----------------------------------------\n");
    printf("  0. Quitter\n");
    printf("========================================\n");
    printf("Votre choix : ");
}


int main() {

    int choix;

    do {
        afficherMenu();
        scanf("%d", &choix);

        switch (choix) {

            case 1:
                ajouterFilm(NOM_FICHIER);
                break;

            case 2: {
                int nb = 0;
                Film *films = chargerFilms(NOM_FICHIER, &nb);
                afficherTousLesFilms(films, nb);
                free(films);
                break;
            }

            case 3: {
                int id;
                printf("Entrez l'ID : ");
                scanf("%d", &id);
                int nb = 0;
                Film *films = chargerFilms(NOM_FICHIER, &nb);
                rechercherParId(films, nb, id);
                free(films);
                break;
            }

            case 4: {
                char motCle[100];
                int c;
                while ((c = getchar()) != '\n' && c != EOF);
                printf("Entrez le mot-clé : ");
                fgets(motCle, sizeof(motCle), stdin);
                motCle[strcspn(motCle, "\n")] = '\0';
                int nb = 0;
                Film *films = chargerFilms(NOM_FICHIER, &nb);
                rechercherParTitre(films, nb, motCle);
                free(films);
                break;
            }

            case 5: {
                char genre[50];
                int c;
                while ((c = getchar()) != '\n' && c != EOF);
                printf("Entrez le genre : ");
                fgets(genre, sizeof(genre), stdin);
                genre[strcspn(genre, "\n")] = '\0';
                int nb = 0;
                Film *films = chargerFilms(NOM_FICHIER, &nb);
                afficherParGenre(films, nb, genre);
                free(films);
                break;
            }

            case 6: {
                int id;
                printf("Entrez l'ID du film à modifier : ");
                scanf("%d", &id);
                modifierFilm(NOM_FICHIER, id);
                break;
            }

            case 7: {
                int id;
                printf("Entrez l'ID du film à supprimer : ");
                scanf("%d", &id);
                supprimerFilm(NOM_FICHIER, id);
                break;
            }

            case 8: {
                int nb = 0;
                Film *films = chargerFilms(NOM_FICHIER, &nb);
                trierParNoteDecroissante(films, nb);
                afficherTousLesFilms(films, nb);
                free(films);
                break;
            }

            case 9: {
                int nb = 0;
                Film *films = chargerFilms(NOM_FICHIER, &nb);
                trierAlphabetique(films, nb);
                afficherTousLesFilms(films, nb);
                free(films);
                break;
            }

            case 10: {
                int nb = 0;
                Film *films = chargerFilms(NOM_FICHIER, &nb);
                afficherTop5(films, nb);
                free(films);
                break;
            }

            case 11: {
                int nb = 0;
                Film *films = chargerFilms(NOM_FICHIER, &nb);
                afficherStatistiques(films, nb);
                free(films);
                break;
            }

            case 0:
                printf("Au revoir !\n");
                break;

            default:
                printf("Choix invalide. Veuillez réessayer.\n\n");
        }

    } while (choix != 0);

    return 0;
}