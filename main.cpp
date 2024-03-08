#include "maillage.hpp"

#include <iostream>     // Entrée/sortie standard
#include <string>       // Type "string"
#include <vector>       // Conteneur "vector" (stockage dynamique)
#include <cmath>        // Fonctions cos, sin, etc
#include <cstdlib>      // Fonction rand
#include <fstream>      // Lecture/écriture de fichiers
#include <ctime>        // Fonction time (utile pour rand)
#include <algorithm>    // Algorithmes génériques de la bibliothèque standard, comme std::find

using namespace std ;   // Utiliser les classes et fonctions de la bibliothèque standard C++ sans le préfixe "std::"

int main() {
//*************************************************************************************************************
// Génération de maillages :
    // MaillageRectangle maillageRegulier(5.0, 3.0, 8, 12) ;   // Maillage rectangle régulier
    // vector<double> abscisses = {0.0, 1.0, 1.9, 2.5, 3.8, 4.0, 5.0} ;
    // vector<double> ordonnees = {0.0, 0.7, 1.7, 2.0, 2.3, 2.9, 3.0} ;
    // MaillageRectangle maillageNonRegulier(5.0, 3.0, abscisses, ordonnees) ; // Maillage rectangle non régulier
    // double angle = M_PI * 5 / 4 ;
    // MaillageSecteurAngulaire maillageAngle(6.0, angle, 50) ;    // Maillage secteur angulaire

    // double angle = M_PI / 4 ;
    // MaillageSecteurAngulaire maillageAngle(6.0, angle, 7) ;
    // maillageAngle.symetrieAxeOblique(0.0, 0.0, M_PI * 5 / 8) ;

//*************************************************************************************************************
// Conversions en fichiers txt lisibles par MATLAB :
    // maillageRegulier.exportMATLAB("maillage_regulier.txt") ;
    // maillageNonRegulier.exportMATLAB("maillage_nonregulier.txt") ;
    // maillageAngle.exportMATLAB("maillage_secteurangulaire.txt") ;

//*************************************************************************************************************
    // double L = 5.0;
    // list<Sommet> sommets_hexagone;
    // for (int i = 0; i < 6; ++i) {
    //     double angle = (M_PI / 3) * i; // Angle de chaque sommet en radians
    //     double x = L * cos(angle);     // Coordonnée x du sommet
    //     double y = L * sin(angle);     // Coordonnée y du sommet
    //     sommets_hexagone.push_back(Sommet(x, y)); // Ajouter le sommet à la liste
    // }

    // cout << "Sommets de l'hexagone :" << endl;
    // int index = 1;
    // for (const Sommet& sommet : sommets_hexagone) {
    //     cout << "Sommet " << index << " : (" << sommet.x << ", " << sommet.y << ")" << endl;
    //     index++;
    // }

//*************************************************************************************************************
// Création des sommets pour former un hexagone
    Sommet A(0.0, 0.0);
    Sommet B(1.0, 0.0);
    Sommet C(1.5, sqrt(3) / 2.0);
    Sommet D(1.0, sqrt(3));
    Sommet E(0.0, sqrt(3));
    Sommet F(-0.5, sqrt(3) / 2.0);

    // Création des segments pour l'hexagone
    Segment AB(&A, &B);
    Segment BC(&B, &C);
    Segment CD(&C, &D);
    Segment DE(&D, &E);
    Segment EF(&E, &F);
    Segment FA(&F, &A);

    // Création d'un front avec les segments de l'hexagone
    const Segment* segmentsHexagone[] = {&AB, &BC, &CD, &DE, &EF, &FA};
    vector<Sommet> pointsInt;
    Front front(segmentsHexagone, pointsInt);

    // Génération des triangles à partir du front hexagonal
    vector<Triangle> triangles = front.genererTriangle();

    // Affichage des triangles générés
    cout << "Triangles generes a partir de l'hexagone :" << endl;
    for (size_t i = 0; i < triangles.size(); ++i) {
        cout << "Triangle " << i+1 << ": ";
        cout << "(" << triangles[i].sommets[0]->x << "," << triangles[i].sommets[0]->y << ") ";
        cout << "(" << triangles[i].sommets[1]->x << "," << triangles[i].sommets[1]->y << ") ";
        cout << "(" << triangles[i].sommets[2]->x << "," << triangles[i].sommets[2]->y << ")" << endl;
    }

    return 0;
}