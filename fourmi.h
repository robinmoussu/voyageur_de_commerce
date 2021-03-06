/*
 *    Voyageur de commerce
 *    Copyright (C) 2014 Robin Moussu - Jingbo Su
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#ifndef FOURMI_H
#define FOURMI_H

/** \file fourmi.h
 * \brief Tout ce qui relatif au parcourt des fourmis
 * \note d
 */

#include "main.h"
#include "graph.h"

/** Données relative au parcourt d'une fourmi
 */
typedef struct {
    double L;                    ///< Longueur d'un chemin, somme des longueurs de chaque arc constituant le chemin
    int nb_villes_deja_visite;
    bool parcourt_valide;
    Ville *tabu[];              ///< Liste des villes déjà parcourues par la fourmi k. La plus plus récente est enregistré dans tabu[nb_villes_deja_visite - 1]; La taille de ce tableau est constante et est égale au nombre de villes dans la simulation;
} Fourmi;

/** Initialise la structure fourmi
 *  \param villes la liste des villes (la ville de départ est tirée aléatoirement dans ce tableau)
 */
void init_fourmi(Fourmi *f, Ville villes[], int nb_villes, bool deja_visite[]);

/** Indique si la ville a déjà été visité par la fourmi
 */
bool deja_visite(Ville *a_visiter, Ville *deja_visite[], int nb_villes_deja_visite);

/** Déplace la fourmi dans la nouvelle ville, en fonction de sa visibilité et des phéromones sur l'arc
 *  \param f La fourmi en train d'effectuer son voyage
 *  \param alpha Coefficient régulant l'importance des phéromones pour le choix d'une ville
 *  \param beta  Coefficient régulant l'importance de la visibilité pour le choix d'une ville
 *  \param proba_ville tableau aloué, non initialisé servant à calculer la la probabilité de chaque ville d'être tirée.
 *  \param deja_visite tableau contenant la liste des villes dejà visitées durant ce parcourt
 */
void ville_suivante(Fourmi *f, int alpha, int beta, double proba_ville[], bool deja_visite[]);

/** Détermine le parcourt de la fourmie fourmie actuelle, valide ce parcourt puis met à jour le graph
 *  \param fourmi_actuelle Fourmi va pantounir le graph
 *  \param villes tableau contenant la liste de toutes les villes du graph
 *  \param nb_villes nombre de villes contenues dans le graph
 *  \param ville_visitees tableau alloué, non initialisé servant à mémoriser la liste des villes visitées dans ce parcourt
 *  \param alpha Coefficient régulant l'importance des phéromones pour le choix d'une ville
 *  \param beta  Coefficient régulant l'importance de la visibilité pour le choix d'une ville
 *  \param proba_ville tableau aloué, non initialisé servant à calculer la la probabilité de chaque ville d'être tirée.
 *
 *  \note Avant l'appel de cette fonction, le champ L (la distance du parcourt) de la meilleur fourmi doit etre initialisé
 *  \note Après l'appel de cette fonction, les données de fourmi_actuelle peuvent avoir changée et etre incohérente
 */
void parcourt(Fourmi *fourmi_actuelle, Ville villes[], int nb_villes, bool ville_visitees[], int alpha, int beta, double proba_ville[]);

/** Valide le parcourt d'une fourmi
 *  \param f Fourmi dont le parcourt est à tester
 *  \param nb_villes nombre de villes contenues dans le graph
 *  \param ville_visitees tableau alloué, non initialisé servant à mémoriser la liste des villes visitées dans ce parcourt
 *  \return true si le parcourt est valide
 */
bool parcourt_valide(Fourmi *f, int nb_villes, bool ville_visitees[]);

/** Vérifie si le parcourt de la fourmi est valide, et met à jour le graph (évaporation + nouveau phéromones)
 *  \note avant de mettre à jour le graph, le parcourt de fourmi_actuelle est vérifié en mode débug
 *
 *  \param fourmi_actuelle Fourmi qui vient de terminer sont parcourt
 *  \param meilleure_fourmi La fourmi possédant pour l'instant le meilleur parcourt
 *  \param nb_villes nombre de villes contenues dans le graph
 *  \param deja_visite tableau alloué, non initialisé servant à mémoriser la liste des villes visitées dans ce parcourt
 *  \param evaporation coeficiant déterminant la vitesse à laquelle les phéromones s'évaporent
 *  \param depot_pheromones quantitée de phéromones déposés à chaques passage
 *
 *  \note Avant l'appel de cette fonction, le champ L (la distance du parcourt) de la meilleur fourmi doit etre initialisé
 *  \note Après l'appel de cette fonction, les données de fourmi_actuelle peuvent avoir changée et etre incohérente
 */
void parcourt_update(Fourmi **fourmi_actuelle, Fourmi **meilleure_fourmi, int nb_villes, bool ville_visitees[], double evaporation, double depot_pheromones);

/** Affiche le parcourt d'une fourmie
 *  \note Avant de l'afficher, vérifie si le parcourt est valide
 *  \param f Fourmi dont le parcourt est à tester
 *  \param villes tableau contenant la liste de toutes les villes du graph
 *  \param nb_villes nombre de villes contenues dans le graph
 *  \param deja_visite tableau alloué, non initialisé servant à mémoriser la liste des villes visitées dans ce parcourt
 */
void affiche_parcourt(Fourmi *f, int nb_villes, bool ville_visitees[]);

/* Explore le graph à l'aide d'une colonie de fourmis
 *
 *  \param villes           Tableau contenant la liste de toutes les villes du graph
 *  \param arcs             Tableau contenant la liste de toutes les arcs du graph
 *  \param fourmis          Pointeur sur un tableau pointant sur l'espace mémoire reservé aux fourmis parcourant le graph
 *  \param meilleure_fourmi La fourmi possédant pour l'instant le meilleur parcourt
 *  \param ville_visitees   Tableau alloué, non initialisé servant à mémoriser la liste des villes visitées dans ce parcourt
 *  \param proba_ville      Tableau aloué, non initialisé servant à calculer la la probabilité de chaque ville d'être tirée.
 *  \param nb_villes        Nombre de villes contenues dans le graph
 *  \param nb_fourmis       Nombre de fourmis
 *  \param nb_voisins       Nombre d’arcs dans le graph
 *  \param max_cycle        Nombre de cycles maximum à éxécuter
 *  \param alpha            Coefficient régulant l'importance des phéromones pour le choix d'une ville
 *  \param beta             Coefficient régulant l'importance de la visibilité pour le choix d'une ville
 *  \param evaporation      Coeficiant d'évaporation (rho dans le sujet)
 *  \param depot_pheromones Quantitée de phéromones ajoutés à chaque passage (Q dans le sujet)


 */
void explore_graph(Ville villes[], Arc arcs[], Fourmi *(*fourmis[])
    , Fourmi *meilleure_fourmi, bool ville_visitees[], double proba_ville[]
    , int nb_villes, int nb_fourmis, int nb_voisins
    , int max_cycle, double alpha, double beta, double evaporation, double depot_pheromones);
#endif /* FOURMI_H */
