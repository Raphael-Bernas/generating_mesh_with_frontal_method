#ifndef maillage_HPP_INCLUDED
#define maillage_HPP_INCLUDED

#include <map>
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
    
    bool operator==(const Sommet& autre) const ;    // Surcharge de l'opérateur ==

};
class Triangle {
public:
    Sommet* sommets[3] ;

    Triangle(Sommet* s1, Sommet* s2, Sommet* s3) ;  // Constructeur
    
    bool operator==(const Triangle& autre) const ;  // Surcharge de l'opérateur ==
};
class Arete {
public:
    Triangle* triangle ;
    int num ;

    Arete(Triangle* T, int n) : triangle(T), num(n) {}  // Constructeur

    bool operator==(const Arete& autre) const ;         // Surcharge de l'opérateur ==
    Sommet& determine_sommet(int num_sommet) const ;    // Renvoie le (num_sommet)-ième sommet de l'arrête
    float determine_longueur() const ;                  // Renvoie la longueur de l'arete
    bool operator|(const Arete& autre) const ;
        /* Le signe des déterminants nous indique la position relative d'un point par rapport a une arrête.
        Si le signe du produit des deux déterminants est négatif cela signifie que les points sont de parté autre du segment.
        Il y a donc croisement.*/
    bool operator<(const Arete& autre) const;           // Opérateur de comparaison pour trier les arêtes par taille
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
        // L'axe est renseigné par un de ses points (x0,y0), et son orientation angle
};

class MaillageRectangle : public Triangulation {        // Classe dérivée de la classe Triangulation
public:
    MaillageRectangle(double largeur, double hauteur, int nx, int ny) : Triangulation(), largeur(largeur), hauteur(hauteur), nx(nx), ny(ny) {
    // Cas régulier : pas constants nx et ny spécifiés
        genererMaillageRectangle(largeur, hauteur, nx, ny) ;
    }
    MaillageRectangle(double largeur, double hauteur, const vector<double>& abscisses, const vector<double>& ordonnees) : Triangulation(), largeur(largeur), hauteur(hauteur) {
    // Cas non régulier : listes d’abcisses et d’ordonnées spécifiées
        genererMaillageRectangle(largeur, hauteur, abscisses, ordonnees) ;
    }

    double largeur, hauteur ;
    int nx, ny ;

    void genererMaillageRectangle(double largeur, double hauteur, int nx, int ny) ;
    // Cas régulier : pas constants nx et ny spécifiés
    void genererMaillageRectangle(double largeur, double hauteur, const vector<double>& abscisses, const vector<double>& ordonnees) ;
    // Cas non régulier : listes d’abcisses et d’ordonnées spécifiées
};

class MaillageSecteurAngulaire : public Triangulation { // Classe dérivée de la classe Triangulation
public:
    MaillageSecteurAngulaire(double rayon, double angle, int N) : Triangulation(), rayon(rayon), angle(angle), N(N) {
    // Constructeur
        genererSecteurAngulaire() ;
    }

    double rayon, angle ;
    int N ;

    MaillageSecteurAngulaire(double rayon, double angle, int N) ;   // Constructeur

    void generersecteurangleaigu() ;
    void genererSecteurAngulaire() ;
};

//====================================================================================================
//                              Méthode Frontale et ses dérivées
//====================================================================================================
class Front {
private:
    std::map<double, Arete> aretes; // Liste d'arêtes triées par taille
    std::vector<Point> points; // Liste des points utilisables pour générer des triangles

public:
    // Ajoute une arête au front
    void ajouterArete(const Arete& arete) {
        arretes[arete.taille] = arete;
    }

    // Supprime une arête du front
    void supprimerArete(int id) {
        arretes.erase(aretes.find(id));
    }

    // Ajoute un point à la liste des points
    void ajouterPoint(const Point& point) {
        points.push_back(point);
    }

    // Méthode pour générer un nouveau triangle
    // (vous devrez implémenter cette méthode en fonction de votre logique de maillage)
    void genererTriangle() {
        // ... logique de génération de triangle ...
    }
};



#endif  //maillage_HPP_INCLUDED