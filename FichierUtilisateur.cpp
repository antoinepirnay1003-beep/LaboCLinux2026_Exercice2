#include "FichierUtilisateur.h"
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

// Calcul de la somme pondérée des codes ASCII modulo 97
int hash(const char* motDePasse) {
    int i = 0;
    int somme = 0;
    while (motDePasse[i] != '\0') {
        somme += (i + 1) * motDePasse[i];
        i++;
    }
    return somme % 97;
}

int estPresent(const char* nom) {
    int fd = open(FICHIER_UTILISATEURS, O_RDONLY);
    if (fd == -1) return -1; // Le fichier n'existe pas

    UTILISATEUR u;
    int pos = 1;
    
    // Lecture séquentielle structure par structure
    while (read(fd, &u, sizeof(UTILISATEUR)) == sizeof(UTILISATEUR)) {
        if (strcmp(u.nom, nom) == 0) {
            close(fd);
            return pos; // Utilisateur trouvé, on retourne sa position (1-based)
        }
        pos++;
    }
    close(fd);
    return 0; // Utilisateur non trouvé
}

void ajouteUtilisateur(const char* nom, const char* motDePasse) {
    // Ouverture en écriture seule, création si absent, ajout à la fin
    int fd = open(FICHIER_UTILISATEURS, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd != -1) {
        UTILISATEUR u;
        strcpy(u.nom, nom);
        u.hash = hash(motDePasse);
        
        write(fd, &u, sizeof(UTILISATEUR));
        close(fd);
    }
}

int verifieMotDePasse(int pos, const char* motDePasse) {
    int fd = open(FICHIER_UTILISATEURS, O_RDONLY);
    if (fd == -1) return -1;

    UTILISATEUR u;
    
    // Déplacement du curseur jusqu'à la position demandée (pos commence à 1)
    lseek(fd, (pos - 1) * sizeof(UTILISATEUR), SEEK_SET);
    
    if (read(fd, &u, sizeof(UTILISATEUR)) == sizeof(UTILISATEUR)) {
        close(fd);
        if (u.hash == hash(motDePasse)) {
            return 1; // Mot de passe correct
        } else {
            return 0; // Mot de passe incorrect
        }
    }
    close(fd);
    return 0;
}

int listeUtilisateurs(UTILISATEUR *vecteur) {
    int fd = open(FICHIER_UTILISATEURS, O_RDONLY);
    if (fd == -1) return 0;

    int compteur = 0;
    
    // Lecture de tous les éléments du fichier
    while (read(fd, &vecteur[compteur], sizeof(UTILISATEUR)) == sizeof(UTILISATEUR)) {
        compteur++;
    }
    
    close(fd);
    return compteur; // Retourne le nombre d'utilisateurs lus
}
