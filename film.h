#ifndef FILM_H
#define FILM_H

/* =========================================================
 * SECTION 1 – STRUCTURE DE DONNÉES
 * ========================================================= */

/*
 * Film : représente un film du catalogue.
 * Chaque champ correspond à une colonne dans le fichier CSV.
 */
typedef struct {
    int   id;           /* Identifiant unique, auto-incrémenté              */
    char  titre[100];   /* Titre du film (sans le caractère ';')            */
    char  genre[50];    /* Genre (ex: Action, Science-Fiction, Drame...)    */
    int   annee;        /* Année de sortie                                  */
    float note;         /* Note entre 0.0 et 10.0                           */
} Film;

/* =========================================================
 * SECTION 2 – GESTION DU FICHIER CSV
 * Responsable : Ahmadoul Khadim GUEYE et Abdou Aziz SAMB
 * ========================================================= */

/*
 * chargerFilms : lit le fichier CSV et charge les films en mémoire.
 * - nomFichier : chemin vers le fichier CSV
 * - nbFilms    : pointeur mis à jour avec le nombre de films lus
 * Retourne un tableau alloué dynamiquement de Film,
 * ou NULL si le fichier est vide ou inexistant.
 */
Film* chargerFilms(const char *nomFichier, int *nbFilms);

/*
 * sauvegarderFilms : écrit l'intégralité du tableau dans le fichier CSV.
 * Utilisé après une modification ou une suppression.
 * - nomFichier : chemin vers le fichier CSV
 * - films      : tableau de films à sauvegarder
 * - nbFilms    : nombre de films dans le tableau
 */
void sauvegarderFilms(const char *nomFichier, Film *films, int nbFilms);

/* =========================================================
 * SECTION 3 – FONCTIONNALITÉS OBLIGATOIRES
*  Responsable : Ahmadoul Khadim GUEYE et Abdou Aziz SAMB
 * ========================================================= */

/*
 * ajouterFilm : saisit les infos d'un nouveau film et l'ajoute au fichier.
 * L'ID est calculé automatiquement (max des ID existants + 1).
 * Responsable : Abdou Aziz SAMB
 */
void ajouterFilm(const char *nomFichier);

/*
 * afficherTousLesFilms : affiche tous les films sous forme de tableau lisible.
 * Responsable : Ahmadoul Khadim GUEYE
 */
void afficherTousLesFilms(Film *films, int nbFilms);

/*
 * rechercherParId : affiche le film dont l'ID correspond, ou un message d'erreur.
 * Responsable : Ahmadoul Khadim GUEYE
 */
void rechercherParId(Film *films, int nbFilms, int id);

/*
 * rechercherParTitre : recherche par sous-chaîne, insensible à la casse.
 * Affiche tous les films dont le titre contient motCle.
 * Responsable : Ahmadoul Khadim GUEYE
 */
void rechercherParTitre(Film *films, int nbFilms, const char *motCle);

/*
 * afficherParGenre : affiche tous les films d'un genre donné (insensible à la casse).
 * Responsable : Ahmadoul Khadim GUEYE
 */
void afficherParGenre(Film *films, int nbFilms, const char *genre);

/*
 * modifierFilm : permet de modifier les champs d'un film sélectionné par son ID.
 * Les modifications sont sauvegardées dans le fichier CSV.
 * Responsable : Abdou Aziz SAMB
 */
void modifierFilm(const char *nomFichier, int id);

/*
 * supprimerFilm : supprime le film dont l'ID correspond et met à jour le fichier.
 * Responsable : Abdou Aziz SAMB
 */
void supprimerFilm(const char *nomFichier, int id);

/* =========================================================
 * SECTION 4 – FONCTIONNALITÉS BONUS
*  Responsable : Ahmadoul Khadim GUEYE et Abdou Aziz SAMB
 * ========================================================= */

/*
 * trierParNoteDecroissante : trie le tableau films du mieux noté au moins bien noté.
 * Ne modifie pas le fichier CSV (tri en mémoire uniquement pour l'affichage).
 * Responsable : Abdou Aziz SAMB
 */
void trierParNoteDecroissante(Film *films, int nbFilms);

/*
 * trierAlphabetique : trie le tableau films par ordre alphabétique du titre.
 * Ne modifie pas le fichier CSV (tri en mémoire uniquement pour l'affichage).
 * Responsable : Abdou Aziz SAMB
 */
void trierAlphabetique(Film *films, int nbFilms);

/*
 * afficherTop5 : affiche les 5 films les mieux notés.
 * Responsable : Abdou Aziz SAMB
 */
void afficherTop5(Film *films, int nbFilms);

/*
 * afficherStatistiques : affiche un résumé du catalogue :
 *   - nombre total de films
 *   - note moyenne
 *   - film le mieux noté / le moins bien noté
 *   - nombre de films par genre
 * Responsable : Ahmadoul Khadim GUEYE
 */
void afficherStatistiques(Film *films, int nbFilms);

#endif /* FILM_H */