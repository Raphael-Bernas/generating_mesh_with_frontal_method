#ifndef maillage_HPP_INCLUDED
#define maillage_HPP_INCLUDED

#include <iostream>     // Entrée/sortie standard
#include <string>       // Type "string"
#include <vector>       // Conteneur "vector" (stockage dynamique)
#include <cmath>        // Fonctions cos, sin, etc
#include <cstdlib>      // Fonction rand
#include <fstream>      // Lecture/écriture de fichiers
#include <ctime>        // Fonction time (utile pour rand)
#include <algorithm>    // Algorithmes génériques de la bibliothèque standard, comme std::find

using namespace std ;   // Utiliser les classes et fonctions de la bibliothèque standard C++ sans le préfixe "std::"


//====================================================================================================
//                                      Classes élémentaires
//====================================================================================================
class Sommet {
public:
    double x, y ;
    
    Sommet(double a, double b) : x(a), y(b) {}      // Constructeur
    bool operator==(const Sommet& autre) const ;    // Surcharge de l'operateur ==
};
class Triangle {
public:
    Sommet* sommets[3] ;

    Triangle(Sommet* s1, Sommet* s2, Sommet* s3) ;  // Constructeur
    bool operator==(const Triangle& autre) const ;  // Surcharge de l'operateur ==
};
class Arete {
public:
    Triangle* triangle ;
    int num ;

    Arete(Triangle* T, int n) : triangle(T), num(n) {}  // Constructeur

    bool operator==(const Arete& autre) const ;         // Surcharge de l'operateur ==
    Sommet& determine_sommet(int num_sommet) const ;    // Renvoie le (num_sommet)-ieme sommet de l'arrete
    float determine_longueur() const ;                  // Renvoie la longueur de l'arete
    bool operator|(const Arete& autre) const ;          // Renvoie bool("les aretes se croisent")
};
class Domaine {
public:
    int dim ;
    string nom ;
    vector<Triangle*> triangles ;
    vector<Arete*> aretes ;

    Domaine(int d, const string& name) : dim(d = 2), nom(name) {}

    bool operator==(const Domaine& autre) const ;       // Surcharge de l'opérateur ==
};


//====================================================================================================
//                              Classe Triangulation et ses dérivées
//====================================================================================================

class Triangulation {
public:
    vector<Sommet*> sommets ;
    vector<Triangle*> triangles ;
    vector<Arete*> aretes ;
    vector<Domaine*> domaines ;

    Triangulation() {}      // Constructeur

    void exportMATLAB(const string& nomFichier) const ;
    // Conversion en un fichier txt lisible sur MATLAB
    void fusionnerMaillages(const Triangulation& autre) ;
    void rotation(double angle) ;
    void translation(double deltaX, double deltaY) ;
    void homothetie(double facteur) ;
    void symetriePoint(double pointX, double pointY) ;
    void symetrieAxeOblique(double x0, double y0, double angle) ;
};

class MaillageRectangle : public Triangulation {        // Classe dérivée de la classe Triangulation
public:
    MaillageRectangle(double largeur, double hauteur, int nx, int ny) ;
    // Cas régulier : pas constants nx et ny spécifiés
    MaillageRectangle(double largeur, double hauteur, const vector<double>& abscisses, const vector<double>& ordonnees) ;
    // Cas non régulier : listes d’abcisses et d’ordonnées spécifiées

private:
    double largeur, hauteur ;
    int nx, ny ;

    void genererMaillageRectangle(double largeur, double hauteur, int nx, int ny) ;
    // Cas régulier : pas constants nx et ny spécifiés
    void genererMaillageRectangle(double largeur, double hauteur, const vector<double>& abscisses, const vector<double>& ordonnees) ;
    // Cas non régulier : listes d’abcisses et d’ordonnées spécifiées
};

class MaillageSecteurAngulaire : public Triangulation { // Classe dérivée de la classe Triangulation
public:
    MaillageSecteurAngulaire(double rayon, double angle, int N) ;   // Constructeur

private:
    double rayon, angle ;
    int N ;

    void generersecteurangleaigu() ;
};

//====================================================================================================
#endif  //maillage_HPP_INCLUDED