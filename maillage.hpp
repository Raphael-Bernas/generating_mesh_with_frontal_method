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

private:
    double rayon, angle ;
    int N ;

    MaillageSecteurAngulaire(double rayon, double angle, int N) ;   // Constructeur

    void generersecteurangleaigu() {
        double precision = 1e-14;                           // Correction des erreurs epsilon machine
        
        // Générer les sommets
        Sommet* centre = new Sommet(0.0, 0.0) ;             // Génération du point central
        sommets.push_back(centre) ;
        for (int i = 1; i < N; ++i) {                       // Génération par arc
            // Arcs indicés par i (le point central n'est pas un arc ici)
            double rayoninterieur = i * rayon / (N - 1) ;
            double theta = angle / i ;
            for (int j = 0; j <= i; ++j) {                  // Il y a i+1 points dans le i-ème arc
                double x = rayoninterieur * cos(j * theta) ;
                double y = rayoninterieur * sin(j * theta) ;
                x = round(x / precision) * precision ;      // Correction des erreurs epsilon machine
                y = round(y / precision) * precision ;
                sommets.push_back(new Sommet(x, y)) ;
            }
        }

        // Générer les triangles
        for (int i = 1; i < N; ++i) {
            // Nouvelle indiciation des arcs : le point central est un arc indicé 1
            for (int j = 0; j < i; ++j) {                   // Il y a donc i points dans le i-ème arc
            int index1 = i * (i - 1) / 2 + j ;
            int index2 = (i + 1) * i / 2 + j ;
            int index3 = (i + 1) * i / 2 + j + 1 ;
            triangles.push_back(new Triangle(sommets[index1], sommets[index2], sommets[index3])) ;
            }
        }
    }
    void genererSecteurAngulaire() {
        angle = (angle == 2 * M_PI) ? angle : fmod(angle + 2 * M_PI, 2 * M_PI) ; // Normalisation de angle

        if (angle <= M_PI / 2){
            generersecteurangleaigu() ;
        }
        else {
            // k = nombre de sous-maillages du découpage
            int k = (angle == 2 * M_PI) ? 4  :(angle == M_PI) ? 2  : floor(angle * 2 / M_PI) + 1 ;
            // Découpage en k maillages identiques d'angle aigu
            double anglek = angle / k ;
            MaillageSecteurAngulaire maillage(rayon, anglek, floor(N/k)+1) ;
            for (int i = 1; i<k; ++i) {
                MaillageSecteurAngulaire maillagei(rayon, anglek, floor(N/k)+1) ;
                maillagei.rotation(i * anglek) ;
                maillage.fusionnerMaillages(maillagei) ;
            }
            fusionnerMaillages(maillage) ;         // Assignation au maillage courant
        }
    }
};

//====================================================================================================
//                              Méthode Frontale et ses dérivées
//====================================================================================================




#endif  //maillage_HPP_INCLUDED