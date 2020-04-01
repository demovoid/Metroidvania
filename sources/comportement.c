#include <stdlib.h>
#include <string.h>
//#include <SDL2/SDL.h>
#include "../headers/structs.h"
#include "../headers/liste.h"
#include "../headers/comportement.h"

#define TAILLE_BLOCK 8

/*static int toucher(monstre_t* e1, monstre_t* e2){
    SDL_rect rect1, rect2;

    rect1.x = TAILLE_BLOCK*e1->pos.x + e1->delta.x;
    rect1.y = TAILLE_BLOCK*e1->pos.y + e1->delta.y;
    rect1.w = e1->type->hitbox.largeur;
    rect1.h = e1->type->hitbox.hauteur;

    rect2.x = TAILLE_BLOCK*e2->pos.x + e2->delta.x;
    rect2.y = TAILLE_BLOCK*e2->pos.y + e2->delta.y;
    rect2.w = e2->type->hitbox.largeur;
    rect2.h = e2->type->hitbox.hauteur;

    return SDL_IntersectRect(&rect1,&rect2);
}*/

static void recupElem(monstre_t* entite, personnage_t* perso){
    for(int i = 0; i < TAILLE_INVENTAIRE; i++){
        if(!strcmp(entite->type->nom,perso->nomObj[i])){
            perso->inventaire[i] = 1;
            entite->pv = 0;
        }
    }
}

int hitE(monstre_t* e1, monstre_t* e2){
    int leftE1, leftE2;
    int rightE1, rightE2;
    int topE1, topE2;
    int bottomE1, bottomE2;

    leftE1 = e1->pos.x*TAILLE_BLOCK + e1->delta.x;
    topE1 = e1->pos.y*TAILLE_BLOCK + e1->delta.y;
    rightE1 = leftE1 + e1->type->hitbox.largeur;
    bottomE1 = topE1 + e1->type->hitbox.hauteur;

    leftE2 = e2->pos.x*TAILLE_BLOCK + e2->delta.x;
    topE2 = e2->pos.y*TAILLE_BLOCK + e2->delta.y;
    rightE2 = leftE2 + e2->type->hitbox.largeur;
    bottomE2 = topE2 + e2->type->hitbox.hauteur;

    if(bottomE1 <= topE2)
        return FALSE;
    
    if(topE1 >= bottomE2)
        return FALSE;
    
    if(rightE1 <= leftE2)
        return FALSE;
    
    if(leftE1 >= rightE2)
        return FALSE;

    return TRUE;
}

int hitP(monstre_t* e, personnage_t* p){
    int leftE, leftP;
    int rightE, rightP;
    int topE, topP;
    int bottomE, bottomP;

    leftE = e->pos.x*TAILLE_BLOCK + e->delta.x;
    topE = e->pos.y*TAILLE_BLOCK + e->delta.y;
    rightE = leftE + e->type->hitbox.largeur;
    bottomE = topE + e->type->hitbox.hauteur;

    leftP = p->pos.x*TAILLE_BLOCK + p->delta.x;
    topP = p->pos.y*TAILLE_BLOCK + p->delta.y;
    rightP = leftP + p->hitbox.largeur;
    bottomP = topP + p->hitbox.hauteur;

    if(bottomE <= topP)
        return FALSE;
    
    if(topE >= bottomP)
        return FALSE;
    
    if(rightE <= leftP)
        return FALSE;
    
    if(leftE >= rightP)
        return FALSE;

    return TRUE;
}

static int hitB(monstre_t* e, salle_t* s){
    int leftE;
    int rightE;
    int topE;
    int bottomE;

    leftE = e->pos.x;
    rightE = e->pos.x + e->delta.x ? 1 : 0;
    topE = e->pos.y;
    bottomE = e->pos.y + e->delta.y ? 1 : 0;

    for(int i = leftE; i <= rightE; i++)
        for(int j = topE; j <= bottomE; j++)
            if(s->mat[i][j])
                return TRUE;

    return FALSE;
}

static int persValidDep(personnage_t* p, salle_t* s){
    int leftP;
    int rightP;
    int topP;
    int bottomP;

    leftP = p->pos.x;
    rightP = rightP + p->hitbox.largeur + p->delta.x ? 1 : 0;
    topP = p->pos.y;
    bottomP = topP + p->hitbox.hauteur + p->delta.y ? 1 : 0;

    for(int i = leftP; i <= rightP; i++)
        for(int j = topP; j <= bottomP; j++)
            if(s->mat[i][j])
                return FALSE;

    return TRUE;
}

void depDroite(personnage_t* p, salle_t* s){
    switch(p->etat){
        case IDLE:
        case RUNNING:
        case JUMPING:
        case FALLING:
            p->delta.x += p->vit_dep;
            if(p->delta.x >= TAILLE_BLOCK){
                (p->pos.x)++;
                p->delta.x = 0;
            }
            if(persValidDep(p,s))
                p->etat = RUNNING;
            else{
                p->delta.x = TAILLE_BLOCK - 1;
                (p->pos.x)--;
            }
            break;
        default:
            break;
    }
}

void depGauche(personnage_t* p, salle_t* s){
    switch(p->etat){
        case IDLE:
        case RUNNING:
        case JUMPING:
        case FALLING:
            p->delta.x -= p->vit_dep;
            if(p->delta.x < 0){
                (p->pos.x)--;
                p->delta.x = 0;
            }
            if(persValidDep(p,s))
                p->etat = RUNNING;
            else{
                p->delta.x = 0;
                (p->pos.x)++;
            }
            break;
        default:
            break;
    }
}

static void dep(monstre_t* entite, salle_t* salle){
    if(entite->pv)
        if(entite->direction){
            entite->delta.x += entite->type->vit_dep;
            if(entite->delta.x >= TAILLE_BLOCK){
                (entite->pos.x)++;
                entite->delta.x = 0;
            }
            if(hitB(entite,salle)){
                entite->direction = LEFT;
                (entite->pos.x)--;
                entite->delta.x = TAILLE_BLOCK - 1;
            }
        }else{
            entite->delta.x -= entite->type->vit_dep;
            if(entite->delta.x < 0){
                (entite->pos.x)--;
                entite->delta.x = 0;
            }
            if(hitB(entite,salle)){
                entite->direction = RIGHT;
                (entite->pos.x)++;
                entite->delta.x = 0;
            }
        }
}

static int inRange(monstre_t* entite, personnage_t* perso, int radius){
    //verif si il n'y a pas de bloc bloquant entre
    int delta = perso->pos.x - entite->pos.x;

    if(delta < radius && delta > -radius)
        return 1;
    if(delta > radius || delta < -radius)
        return 0;

    int deltaF = perso->delta.delta_x.numerateur - entite->delta.delta_x.numerateur;

    if((delta > 0 && deltaF < 0) || (delta < 0 && deltaF > 0))
        return 1;
    return 0;
}

void compRecuperable(monstre_t* entite, personnage_t* perso, salle_t* salle, liste_t* lEntites){
    if(hitP(entite,perso))
        recupElem(entite,perso);

    //gestion des sprites
    entite->spritesActuel = (entite->spritesActuel + 1)%(entite->type->nb_sprites);
}

void compFleches(monstre_t* entite, personnage_t* perso, salle_t* salle, liste_t* lEntites){
    if(strcmp(lEntites->type, "monstre"))
        return;
    monstre_t* tmp = malloc(sizeof(monstre_t));
    enTete(lEntites);
    while(!horsListe(lEntites)){
        valeurElm(lEntites,tmp);
        if(hitE(entite,tmp) && entite->pv){
            tmp->pv -= entite->type->degat;
            modifElm(lEntites,tmp);
            entite->pv = 0;
        }
        suivant(lEntites);
    }
    free(tmp);

    if(hitB(entite,salle))
        entite->pv = 0;
    
    //gestion deb
    dep(entite);

    //gestion des sprites
}

void compMurGlace(monstre_t* entite, personnage_t* perso, salle_t* salle, liste_t* lEntites){
    if(entite->pv)
        entite->pv = 2;
    /*ou
    if(entite->pv >= 10)
        entite->pv = 11;
    else
        (entite->pv)--;
    */
}

void compRoiVifplume(monstre_t* entite, personnage_t* perso, salle_t* salle, liste_t* lEntites){
    //fonction en beta
    if(hitP(entite,perso)){
        perso->pv -= entite->type->degat;
    }
    if(!hitB(entite,salle)){
        dep(entite);
    }
    //gestion sprite
}

void compSerpent(monstre_t* entite, personnage_t* perso, salle_t* salle, liste_t* lEntites){
    if(hitP(entite,perso)){
        perso->pv -= entite->type->degat;
        perso->inv = 30;
        entite->direction = 1 - entite->direction;
        //sprite
    }else{
        dep(entite,salle);
    }
    //gestion sprite
}

void compSerpentRose(monstre_t* entite, personnage_t* perso, salle_t* salle, liste_t* lEntites){
    if(hitP(entite,perso)){
        perso->pv -= entite->type->degat;
        entite->direction = 1 - entite->direction;
        //sprite
    }else{
        if(hitB(entite,salle)){
            entite->direction = 1 - entite->direction;
            //sprite
        }else{
            if(inRange(entite,perso,6)){
                //attaquer
            }
        }
    }
    //gestion sprite
}

void compSingeGrotte(monstre_t* entite, personnage_t* perso, salle_t* salle, liste_t* lEntites){
    if(inRange(entite,perso,4)){
        //attaquer
    }
}

void compVersGeant(monstre_t* entite, personnage_t* perso, salle_t* salle, liste_t* lEntites){

}

void compVifplume(monstre_t* entite, personnage_t* perso, salle_t* salle, liste_t* lEntites){

}