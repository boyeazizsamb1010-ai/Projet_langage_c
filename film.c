#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>   

#include "film.h"


Film* chargerFilms(const char *nomFichier, int *nbFilms) 
{

    *nbFilms = 0; /* initialisation : 0 film par défaut */

    FILE *f = fopen(nomFichier, "r");
    if (f == NULL) 
    {
        f = fopen(nomFichier, "w");
        if (f != NULL) fclose(f);
        return NULL; 
    }

    char ligne[300]; 
    int compte = 0;

    while (fgets(ligne, sizeof(ligne), f) != NULL) 
    {
        if (strlen(ligne) > 1) 
        { 
            compte++;
        }
    }

    if (compte == 0) 
    {
        fclose(f);
        return NULL; /* fichier vide */
    }

    /* --- Étape 3 : allouer le tableau dynamiquement --- */
    Film *films = malloc(compte * sizeof(Film));
    if (films == NULL) 
    {
        /* malloc a échoué : pas assez de mémoire */
        fprintf(stderr, "Erreur : allocation mémoire impossible.\n");
        fclose(f);
        return NULL;
    }

    rewind(f); 

    int i = 0;
    while (fgets(ligne, sizeof(ligne), f) != NULL && i < compte) {
        if (strlen(ligne) <= 1) continue; /* ignorer les lignes vides */

        
        sscanf(ligne, "%d;%99[^;];%49[^;];%d;%f",
               &films[i].id,
               films[i].titre,
               films[i].genre,
               &films[i].annee,
               &films[i].note);
        i++;
    }

    fclose(f);
    *nbFilms = i; 
    return films;  /* le tableau alloué — à libérer avec free() après usage */
}

/* --------------------------------------------------------- */

/*
 * sauvegarderFilms
 * Réécrit entièrement le fichier CSV à partir du tableau en mémoire.
 * Appelée après chaque ajout, modification ou suppression.
 */
void sauvegarderFilms(const char *nomFichier, Film *films, int nbFilms) {

    /* mode "w" : ouvre le fichier et efface son contenu existant */
    FILE *f = fopen(nomFichier, "w");
    if (f == NULL) {
        fprintf(stderr, "Erreur : impossible d'ouvrir %s en écriture.\n", nomFichier);
        return;
    }

    /* on parcourt le tableau et on écrit chaque film sur une ligne */
    for (int i = 0; i < nbFilms; i++) {
        fprintf(f, "%d;%s;%s;%d;%.1f\n",
                films[i].id,
                films[i].titre,
                films[i].genre,
                films[i].annee,
                films[i].note);
    }

    fclose(f);
}

/* =========================================================
 * SECTION 2 — FONCTIONS OBLIGATOIRES (Personne A)
 * ========================================================= */

/*
 * afficherTousLesFilms
 * Affiche le catalogue complet sous forme de tableau lisible.
 */
void afficherTousLesFilms(Film *films, int nbFilms) {

    if (films == NULL || nbFilms == 0) {
        printf("Le catalogue est vide.\n");
        return;
    }

    /* en-tête du tableau */
    printf("\n%-4s %-30s %-20s %-6s %-5s\n",
           "ID", "Titre", "Genre", "Année", "Note");
    printf("%-4s %-30s %-20s %-6s %-5s\n",
           "----", "------------------------------",
           "--------------------", "------", "-----");

    for (int i = 0; i < nbFilms; i++) {
        printf("%-4d %-30s %-20s %-6d %.1f/10\n",
               films[i].id,
               films[i].titre,
               films[i].genre,
               films[i].annee,
               films[i].note);
    }
    printf("\n");
}

/* --------------------------------------------------------- */

/*
 * rechercherParId
 * Affiche le film dont l'ID correspond, ou un message d'erreur.
 */
void rechercherParId(Film *films, int nbFilms, int id) {

    for (int i = 0; i < nbFilms; i++) {
        if (films[i].id == id) {
            printf("\n--- Film trouvé ---\n");
            printf("ID    : %d\n",   films[i].id);
            printf("Titre : %s\n",   films[i].titre);
            printf("Genre : %s\n",   films[i].genre);
            printf("Année : %d\n",   films[i].annee);
            printf("Note  : %.1f/10\n\n", films[i].note);
            return; /* on a trouvé, inutile de continuer */
        }
    }

    printf("Aucun film avec l'ID %d.\n", id);
}

/* --------------------------------------------------------- */

/*
 * strContientInsensible (fonction utilitaire interne)
 * Retourne 1 si 'haystack' contient 'needle', insensible à la casse.
 * Non déclarée dans film.h car utilisée uniquement dans film.c.
 */
static int strContientInsensible(const char *haystack, const char *needle) {
    char h[200], n[200];
    int i;

    /* convertir les deux chaînes en minuscules */
    for (i = 0; haystack[i] && i < 199; i++) h[i] = tolower(haystack[i]);
    h[i] = '\0';

    for (i = 0; needle[i] && i < 199; i++) n[i] = tolower(needle[i]);
    n[i] = '\0';

    /* strstr cherche n dans h, retourne NULL si absent */
    return strstr(h, n) != NULL;
}

/* --------------------------------------------------------- */

/*
 * rechercherParTitre
 * Affiche tous les films dont le titre contient motCle (casse ignorée).
 */
void rechercherParTitre(Film *films, int nbFilms, const char *motCle) {

    int trouve = 0;

    printf("\nRésultats pour \"%s\" :\n", motCle);

    for (int i = 0; i < nbFilms; i++) {
        if (strContientInsensible(films[i].titre, motCle)) {
            printf("  [%d] %s (%d) — %.1f/10\n",
                   films[i].id, films[i].titre,
                   films[i].annee, films[i].note);
            trouve++;
        }
    }

    if (trouve == 0) {
        printf("  Aucun film trouvé pour \"%s\".\n", motCle);
    }
    printf("\n");
}

/* --------------------------------------------------------- */

/*
 * afficherParGenre
 * Affiche tous les films d'un genre donné (casse ignorée).
 */
void afficherParGenre(Film *films, int nbFilms, const char *genre) {

    int trouve = 0;

    printf("\nFilms du genre \"%s\" :\n", genre);

    for (int i = 0; i < nbFilms; i++) {
        if (strContientInsensible(films[i].genre, genre)) {
            printf("  [%d] %s (%d) — %.1f/10\n",
                   films[i].id, films[i].titre,
                   films[i].annee, films[i].note);
            trouve++;
        }
    }

    if (trouve == 0) {
        printf("  Aucun film du genre \"%s\".\n", genre);
    }
    printf("\n");
}

/* =========================================================
 * SECTION 3 — FONCTIONS OBLIGATOIRES (Personne B)
 * ========================================================= */

/*
 * ajouterFilm
 * Saisit les informations d'un nouveau film et l'ajoute au fichier CSV.
 * L'ID est calculé automatiquement : max des ID existants + 1.
 */
void ajouterFilm(const char *nomFichier) {

    /* charger le catalogue actuel pour calculer le prochain ID */
    int nbFilms = 0;
    Film *films = chargerFilms(nomFichier, &nbFilms);

    /* calculer le prochain ID (max + 1) */
    int maxId = 0;
    for (int i = 0; i < nbFilms; i++) {
        if (films[i].id > maxId) maxId = films[i].id;
    }

    /* saisie des informations du nouveau film */
    Film nouveau;
    nouveau.id = maxId + 1;

    printf("\n--- Ajouter un film ---\n");

    /* vider le buffer clavier avant getchar/fgets */
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    printf("Titre  : ");
    fgets(nouveau.titre, sizeof(nouveau.titre), stdin);
    nouveau.titre[strcspn(nouveau.titre, "\n")] = '\0'; /* supprimer le '\n' final */

    printf("Genre  : ");
    fgets(nouveau.genre, sizeof(nouveau.genre), stdin);
    nouveau.genre[strcspn(nouveau.genre, "\n")] = '\0';

    printf("Année  : ");
    scanf("%d", &nouveau.annee);

    printf("Note (0-10) : ");
    scanf("%f", &nouveau.note);

    /* valider la note */
    if (nouveau.note < 0.0f) nouveau.note = 0.0f;
    if (nouveau.note > 10.0f) nouveau.note = 10.0f;

    /* agrandir le tableau et ajouter le nouveau film */
    films = realloc(films, (nbFilms + 1) * sizeof(Film));
    if (films == NULL) {
        fprintf(stderr, "Erreur : allocation mémoire impossible.\n");
        return;
    }
    films[nbFilms] = nouveau;
    nbFilms++;

    /* sauvegarder le tableau mis à jour */
    sauvegarderFilms(nomFichier, films, nbFilms);
    free(films);

    printf("Film \"%s\" ajouté avec l'ID %d.\n\n", nouveau.titre, nouveau.id);
}

/* --------------------------------------------------------- */

/*
 * modifierFilm
 * Modifie les champs d'un film sélectionné par son ID.
 */
void modifierFilm(const char *nomFichier, int id) {

    int nbFilms = 0;
    Film *films = chargerFilms(nomFichier, &nbFilms);

    /* chercher le film à modifier */
    int idx = -1;
    for (int i = 0; i < nbFilms; i++) {
        if (films[i].id == id) { idx = i; break; }
    }

    if (idx == -1) {
        printf("Aucun film avec l'ID %d.\n", id);
        free(films);
        return;
    }

    printf("\n--- Modifier le film [%d] %s ---\n", id, films[idx].titre);
    printf("(Appuyez sur Entrée pour garder la valeur actuelle)\n\n");

    char buffer[200];
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    /* titre */
    printf("Titre [%s] : ", films[idx].titre);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0) strcpy(films[idx].titre, buffer);

    /* genre */
    printf("Genre [%s] : ", films[idx].genre);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0) strcpy(films[idx].genre, buffer);

    /* année */
    printf("Année [%d] : ", films[idx].annee);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0) films[idx].annee = atoi(buffer);

    /* note */
    printf("Note [%.1f] : ", films[idx].note);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0) {
        films[idx].note = atof(buffer);
        if (films[idx].note < 0.0f) films[idx].note = 0.0f;
        if (films[idx].note > 10.0f) films[idx].note = 10.0f;
    }

    sauvegarderFilms(nomFichier, films, nbFilms);
    free(films);

    printf("Film modifié avec succès.\n\n");
}

/* --------------------------------------------------------- */

/*
 * supprimerFilm
 * Supprime le film dont l'ID correspond et met à jour le fichier.
 */
void supprimerFilm(const char *nomFichier, int id) {

    int nbFilms = 0;
    Film *films = chargerFilms(nomFichier, &nbFilms);

    /* chercher l'index du film à supprimer */
    int idx = -1;
    for (int i = 0; i < nbFilms; i++) {
        if (films[i].id == id) { idx = i; break; }
    }

    if (idx == -1) {
        printf("Aucun film avec l'ID %d.\n", id);
        free(films);
        return;
    }

    printf("Film \"%s\" supprimé.\n", films[idx].titre);

    /*
     * Décaler tous les films suivants d'une position vers la gauche
     * pour "écraser" le film supprimé.
     * Exemple : [A, B, C, D] → supprimer B → [A, C, D]
     */
    for (int i = idx; i < nbFilms - 1; i++) {
        films[i] = films[i + 1];
    }
    nbFilms--;

    sauvegarderFilms(nomFichier, films, nbFilms);
    free(films);
    printf("\n");
}

/* =========================================================
 * SECTION 4 — FONCTIONNALITÉS BONUS
 * ========================================================= */

/*
 * trierParNoteDecroissante
 * Tri à bulles : du film le mieux noté au moins bien noté.
 * Tri en mémoire uniquement, ne modifie pas le fichier CSV.
 */
void trierParNoteDecroissante(Film *films, int nbFilms) {
    Film temp;
    for (int i = 0; i < nbFilms - 1; i++) {
        for (int j = 0; j < nbFilms - 1 - i; j++) {
            if (films[j].note < films[j + 1].note) {
                /* échanger films[j] et films[j+1] */
                temp        = films[j];
                films[j]    = films[j + 1];
                films[j + 1] = temp;
            }
        }
    }
}

/* --------------------------------------------------------- */

/*
 * trierAlphabetique
 * Tri à bulles : ordre alphabétique du titre (insensible à la casse).
 */
void trierAlphabetique(Film *films, int nbFilms) {
    Film temp;
    for (int i = 0; i < nbFilms - 1; i++) {
        for (int j = 0; j < nbFilms - 1 - i; j++) {
            /* strcasecmp compare sans tenir compte de la casse */
            if (strcasecmp(films[j].titre, films[j + 1].titre) > 0) {
                temp        = films[j];
                films[j]    = films[j + 1];
                films[j + 1] = temp;
            }
        }
    }
}

/* --------------------------------------------------------- */

/*
 * afficherTop5
 * Affiche les 5 films les mieux notés.
 */
void afficherTop5(Film *films, int nbFilms) {

    if (films == NULL || nbFilms == 0) {
        printf("Le catalogue est vide.\n");
        return;
    }

    /*
     * On travaille sur une copie du tableau pour ne pas
     * modifier l'ordre original du catalogue.
     */
    Film *copie = malloc(nbFilms * sizeof(Film));
    if (copie == NULL) {
        fprintf(stderr, "Erreur : allocation mémoire impossible.\n");
        return;
    }
    memcpy(copie, films, nbFilms * sizeof(Film));

    trierParNoteDecroissante(copie, nbFilms);

    int limite = nbFilms < 5 ? nbFilms : 5; /* au cas où il y a moins de 5 films */

    printf("\n--- Top %d des films ---\n", limite);
    for (int i = 0; i < limite; i++) {
        printf("  %d. [%.1f/10] %s (%d)\n",
               i + 1,
               copie[i].note,
               copie[i].titre,
               copie[i].annee);
    }
    printf("\n");

    free(copie);
}

/* --------------------------------------------------------- */

/*
 * afficherStatistiques
 * Affiche un résumé complet du catalogue.
 */
void afficherStatistiques(Film *films, int nbFilms) {

    if (films == NULL || nbFilms == 0) {
        printf("Le catalogue est vide.\n");
        return;
    }

    /* calcul de la moyenne, du min et du max */
    float somme = 0.0f;
    int   idxMin = 0, idxMax = 0;

    for (int i = 0; i < nbFilms; i++) {
        somme += films[i].note;
        if (films[i].note < films[idxMin].note) idxMin = i;
        if (films[i].note > films[idxMax].note) idxMax = i;
    }

    printf("\n--- Statistiques du catalogue ---\n");
    printf("Nombre total de films : %d\n", nbFilms);
    printf("Note moyenne          : %.2f/10\n", somme / nbFilms);
    printf("Meilleur film         : %s (%.1f/10)\n",
           films[idxMax].titre, films[idxMax].note);
    printf("Film le moins bien noté : %s (%.1f/10)\n",
           films[idxMin].titre, films[idxMin].note);

    /* nombre de films par genre */
    printf("\nNombre de films par genre :\n");

    /*
     * Algorithme : pour chaque film, on vérifie si son genre
     * a déjà été compté. Si non, on compte combien de films
     * partagent ce genre et on l'affiche.
     */
    int deja_compte[200] = {0}; /* tableau de "drapeaux" pour éviter les doublons */

    for (int i = 0; i < nbFilms; i++) {
        if (deja_compte[i]) continue; /* genre déjà traité */

        int count = 1;
        for (int j = i + 1; j < nbFilms; j++) {
            if (strContientInsensible(films[j].genre, films[i].genre)
                && strContientInsensible(films[i].genre, films[j].genre)) {
                count++;
                deja_compte[j] = 1;
            }
        }
        printf("  %-20s : %d film(s)\n", films[i].genre, count);
    }
    printf("\n");
}