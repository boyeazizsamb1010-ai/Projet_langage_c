#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>   

#include "film.h"

/* =========================================================
 * SECTION 1 — GESTION DU FICHIER CSV
 * ========================================================= */

/*
 * chargerFilms
 * Lit le fichier CSV et retourne un tableau de Film alloué dynamiquement.
 * *nbFilms est mis à jour avec le nombre de films lus.
 */
Film* chargerFilms(const char *nomFichier, int *nbFilms) 
{

    *nbFilms = 0; /* initialisation : 0 film par défaut */

    /* --- Étape 1 : ouvrir le fichier en lecture --- */
    FILE *f = fopen(nomFichier, "r");
    if (f == NULL) 
    {
        /* Le fichier n'existe pas encore : on le crée vide */
        f = fopen(nomFichier, "w");
        if (f != NULL) fclose(f);
        return NULL; /* catalogue vide, c'est normal au 1er lancement */
    }

    /* --- Étape 2 : compter le nombre de lignes (= nombre de films) --- */
    char ligne[300]; /* buffer temporaire pour lire une ligne */
    int compte = 0;

    while (fgets(ligne, sizeof(ligne), f) != NULL) 
    {
        /* fgets lit une ligne entière jusqu'au '\n' ou EOF */
        if (strlen(ligne) > 1) 
        { /* on ignore les lignes vides */
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
        fprintf(stderr, "Erreur : allocation mémoire impossible.\n");
        fclose(f);
        return NULL;
    }

    /* --- Étape 4 : revenir au début du fichier et lire les données --- */
    rewind(f); /* remet le curseur de lecture au début du fichier */

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
    *nbFilms = i; /* on met à jour le compteur passé en paramètre */
    return films;  /* le tableau alloué — à libérer avec free() après usage */
}

/* --------------------------------------------------------- */

/*
 * sauvegarderFilms
 * Réécrit entièrement le fichier CSV à partir du tableau en mémoire.
 * Appelée après chaque ajout, modification ou suppression.
 */
void sauvegarderFilms(const char *nomFichier, Film *films, int nbFilms) 
{

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
