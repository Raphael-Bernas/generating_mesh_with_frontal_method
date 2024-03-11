#include "maillage.hpp"

#include <iostream>     // Entrée/sortie standard
#include <string>       // Type "string"
#include <vector>       // Conteneur "vector" (stockage dynamique)
#include <cmath>        // Fonctions cos, sin, etc
#include <cstdlib>      // Fonction rand
#include <fstream>      // Lecture/écriture de fichiers
#include <random>       // Inclusion de l'en-tête pour std::mt19937
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
    Sommet A(0.0, 0.0);
    Sommet B(1.0, 0.0);
    Sommet C(1.5, sqrt(3) / 2.0);
    Sommet D(1.0, sqrt(3));
    Sommet E(0.0, sqrt(3));
    Sommet F(-0.5, sqrt(3) / 2.0);
    Segment AB(&A, &B);
    Segment BC(&B, &C);
    Segment CD(&C, &D);
    Segment DE(&D, &E);
    Segment EF(&E, &F);
    Segment FA(&F, &A);
    const Segment* segmentsHexagone[] = {&AB, &BC, &CD, &DE, &EF, &FA};

    Front front(segmentsHexagone, 6);
    cout << "Front hexagonal initial : " << endl;
    front.print();
    
    cout << "Premier triangle genere : " << endl;
    // Génération des triangles à partir du front hexagonal
    vector<Triangle> triangles = front.genererTriangle();
    // Affichage des triangles générés
    for (size_t i = 0; i < triangles.size(); ++i) {
        cout << "Triangle " << i+1 << ": ";
        cout << "(" << triangles[i].sommets[0]->x << "," << triangles[i].sommets[0]->y << ") ";
        cout << "(" << triangles[i].sommets[1]->x << "," << triangles[i].sommets[1]->y << ") ";
        cout << "(" << triangles[i].sommets[2]->x << "," << triangles[i].sommets[2]->y << ")" << endl;
    }
    cout << "Nouveau front apres generation du triangle : " << endl;
    front.print();
    cout << "Deuxieme triangle genere : " << endl;
    triangles = front.genererTriangle();
    for (size_t i = 0; i < triangles.size(); ++i) {
        cout << "Triangle " << i+1 << ": ";
        cout << "(" << triangles[i].sommets[0]->x << "," << triangles[i].sommets[0]->y << ") ";
        cout << "(" << triangles[i].sommets[1]->x << "," << triangles[i].sommets[1]->y << ") ";
        cout << "(" << triangles[i].sommets[2]->x << "," << triangles[i].sommets[2]->y << ")" << endl;
    }
    cout << "Nouveau front apres la deuxieme occurence : " << endl;
    front.print();
//*************************************************************************************************************
// Test de segments se croisant
    Sommet A_b(1, 1);
    Sommet B_b(4, 4);
    Sommet C_b(1, 4);
    Sommet D_b(4, 1);

    const Segment s1(&A_b, &B_b);
    const Segment s2(&C_b, &D_b);

    if (s1.segmentsSeCroisent(&s2)) {
        std::cout << "Test réussi : Les segments se croisent." << std::endl;
    } else {
        std::cout << "Test échoué : Les segments ne se croisent pas." << std::endl;
    }

    // Test de segments ne se croisant pas
    Sommet E_b(1, 1);
    Sommet F_b(2, 2);
    Sommet G_b(5, 5);
    Sommet H_b(6, 6);

    const Segment s3(&E_b, &F_b);
    const Segment s4(&G_b, &H_b);

    if (!s3.segmentsSeCroisent(&s4)) {
        std::cout << "Test réussi : Les segments ne se croisent pas." << std::endl;
    } else {
        std::cout << "Test échoué : Les segments se croisent." << std::endl;
    }
//*************************************************************************************************************
    // Génération d'un front polygonal de n côtés
    Front thefront;
    thefront.polygone_regulier(6); // Ici, un hexagone
    std::cout << "Polygone created successfully!" << std::endl;

    // Division du front en segments plus petits
    front.Divise_Front(0.5);
    std::cout << "Front divided successfully!" << std::endl;

    // Création du maillage frontal à partir du front
    MaillageFront maillageFront(&thefront);

    // Application de la méthode frontal
    if (!maillageFront.MethodeFrontal()) {
        std::cerr << "Error: Failed to apply frontal method" << std::endl;
        return 1;
    }

    // Réussite de l'application de la méthode frontal
    std::cout << "Frontal method applied successfully!" << std::endl;

    return 0;
}