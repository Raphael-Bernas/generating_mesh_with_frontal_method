#ifndef maillage_HPP_INCLUDED
#define maillage_HPP_INCLUDED

#include <list>         // Utilisation de list
#include <map>          // Utilisation de map
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

    bool operator==(const Triangle& autre) const;        // Surcharge de l'opérateur ==
    bool in_triangle(const Sommet& point) const;         // Vérifie si un point est dans un triangle
    bool in_circle_triangle(const Sommet& point) const;  // Vérifie si un point est dans un cercle circonscrit
    Sommet circumcenter() const;                          // Renvoie le centre du cercle circonscrit
    double circumradius() const;                          // Renvoie la longueur du rayon du cercle circonscrit
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
class Segment {
    public:
        Sommet* sommets[2] ;
    
    Segment(Sommet* s1, Sommet* s2);
    float longueur() const;
    bool operator==(const Segment& autre) const ;
    double operator,(const Segment& autre) ;        // Produit scalaire
    bool operator|(const Segment& autre) const ;
    bool operator<(const Segment& autre) const;
    // Fonction pour interpoler un point le long du segment
    Sommet pointInterpolation(double t) const;
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
//                              Méthode Frontale et ses dérivées
//====================================================================================================
class Front {
public:
    map<double, list<const Segment*>> segments;     // Liste de liste d'arêtes triées par taille
    vector<Sommet> points;                          // Liste des points utilisables pour générer des triangles

    Front() {}; // Constructeur vide
    Front(const Segment** Nsegments, vector<Sommet> Npoints) ;      // Constructeur Front
    Front(const Segment** Nsegments, int taille) ;                  // Constructeur Front
    void ajouterSegment(const Segment* psegment) ;      // Ajoute une arête au front
    void supprimerSegment(const Segment* segment) ;     // Supprime une arête du front
    void ajouterPoint(const Sommet& point) ;            // Ajoute un point à la liste des points
    void supprimerPoint(const Sommet& point) ;          // Supprime un point de notre liste
    vector<Triangle> genererTriangle();                 // Générer un nouveau triangle
    bool int_front(const Sommet Point);                 // Vérifier si un point est dans le front
    void Divise_Front(double h);                        // Divise le front selon un pas h donnée
    void polygone_random(int n);                        // Génère un front random polygonal de n côté
    int compteSegment();                                // Compter le nombre de segments dans le front
    bool empty() const {return segments.empty();}       // Vérifier si le front est vide
    void save();                                        // Sauvegarder le front
    const Segment* miseajour(const Segment* seginit);           // Mettre à jour le front à partir d'un segment initial à la méthode de Delaunay
    void print();                                 // Afficher le front
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
    void save() ;   // Sauvegarde de la triangulation
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
    double rayon, angle ;
    int N ;

    MaillageSecteurAngulaire(double rayon, double angle, int N) ;   // Constructeur

    void generersecteurangleaigu() ;
    void genererSecteurAngulaire() ;
};
class MaillageFront : public Triangulation {
public:
    Front* TheFront ;
    MaillageFront(Front* AFront) ;
    MaillageFront(char modele, float Hpas) ;
    bool MethodeFrontal() ;
};

#endif  //maillage_HPP_INCLUDED