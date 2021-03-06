#ifndef COMPORTEMENT_INCLUDED
#define COMPORTEMENT_INCLUDED

/**
 * \file comportement.h
 * \brief Header de l'ensemble de fonctions pour le comportement des mobs et du personnage
 * \author Thomas DIDIER L2 Info Le Mans
 * \version 1.1
 * \date 17/05/2020
*/
#include <time.h>

int hitE(monstre_t* e1, monstre_t* e2);
int persValid(personnage_t* p, salle_t* s);

void depDroite(personnage_t* p, salle_t* s);
void depGauche(personnage_t* p, salle_t* s);
void depVert(personnage_t* p, salle_t* s, int tryJump);
void attaquer(personnage_t* p, salle_t* s, int tryAtk);
char* prendPorte(personnage_t* p, liste_t* lPortes);

void evolution(personnage_t* p, salle_t* s);
void transformation(personnage_t* p, salle_t * s);

void compRecuperable(monstre_t* entite, personnage_t* perso, salle_t* salle);
void compCoeur(monstre_t* entite, personnage_t* perso, salle_t* salle);
void compFleches(monstre_t* entite, personnage_t* perso, salle_t* salle);
void compPortes(monstre_t* entite, personnage_t* perso, salle_t* salle);
void compMurGlace(monstre_t* entite, personnage_t* perso, salle_t* salle);
void compRoiVifplume(monstre_t* entite, personnage_t* perso, salle_t* salle);
void compSerpent(monstre_t* entite, personnage_t* perso, salle_t* salle);
void compSerpentRose(monstre_t* entite, personnage_t* perso, salle_t* salle);
void compSingeGrotte(monstre_t* entite, personnage_t* perso, salle_t* salle);
void compVenin(monstre_t* entite, personnage_t* perso, salle_t* salle);
void compVersGeant(monstre_t* entite, personnage_t* perso, salle_t* salle);
void compVifplume(monstre_t* entite, personnage_t* perso, salle_t* salle);

#endif
