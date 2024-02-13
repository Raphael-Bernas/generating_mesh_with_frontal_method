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

    bool operator==(const Domaine& autre) const {       // Surcharge de l'opérateur ==
        for (const auto& triangle : autre.triangles) {
            for (const auto& existingTriangle : triangles) {
                if (!(*triangle == *existingTriangle)) {
                    return false ;
                }
            }
        }
        for (const auto& arete : autre.aretes) {
            for (const auto& existingArete : aretes) {
                if (!(*arete == *existingArete)) {
                    return false ;
                }
            }
        }
        return (dim == autre.dim) ;
    }
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

    void exportMATLAB(const string& nomFichier) const {
    // Conversion en un fichier txt lisible sur MATLAB
        ofstream fichier(nomFichier) ;              // Création du fichier
        for (const auto& sommet : sommets) {        // Écriture des sommets
            fichier <<"0 " ;    // Empreinte "Sommet" pour le script MATLAB
            fichier <<sommet->x <<" " <<sommet->y <<"\n" ;
        }
        for (const auto& triangle : triangles) {    // Écriture des triangles
            fichier << "1 " ;   // Empreinte "Triangle" pour le script MATLAB
            for (size_t i = 0; i < sommets.size(); ++i) {
                // Comparer les sommets des triangles avec les sommets de la triangulation
                if (triangle->sommets[0] == sommets[i] || 
                    triangle->sommets[1] == sommets[i] ||
                    triangle->sommets[2] == sommets[i]) {
                        fichier <<i+1 <<" " ;       // Écrire l'indice du sommet dans le fichier
                }
            }
        fichier << "\n" ;
        }
        fichier.close() ;
    }
    void fusionnerMaillages(const Triangulation& autre) {
        // Fusionner les sommets en supprimant les doublons
        for (const auto& sommet : autre.sommets) {
            bool doublon = false ;
            for (const auto& existingSommet : sommets) {
                if (*sommet == *existingSommet) {
                    doublon = true ;
                    // Mise à jour des pointeurs dans les triangles de la triangulation "autre"
                    for (const auto& triangleAutre : autre.triangles) {
                        for (int i = 0; i < 3; ++i) {
                            if (*triangleAutre->sommets[i] == *sommet) {
                                triangleAutre->sommets[i] = existingSommet ;
                            }
                        }
                    }
                    break ;
                }
            }
            if (!doublon) {
                sommets.push_back(sommet) ;
            }
        }

        // Fusionner les triangles en supprimant les doublons
        for (const auto& triangle : autre.triangles) {
            bool doublon = false ;
            for (const auto& existingTriangle : triangles) {
                if (*triangle == *existingTriangle) {
                    doublon = true ;
                    break ;
                }
            }
            if (!doublon) {
                triangles.push_back(triangle) ;
                for (int i = 0; i < 3; ++i) {               // Ajouter les arêtes correspondantes
                    Arete* arete = new Arete(triangle, i) ;
                    for (const auto& existingArete : aretes) {
                        if (*arete == *existingArete) {     // Sans doublon
                            doublon = true ;
                            break ;
                        }
                    }
                    if (!doublon) {
                        aretes.push_back(arete) ;
                    }
                }
            }
        }

        // Fusionner les domaines en supprimant les doublons
        for (const auto& domaine : autre.domaines) {
            bool doublon = false ;
            for (const auto& existingDomaine : domaines) {
                if (*existingDomaine == *domaine) {
                    doublon = true ;
                    break ;
                }
            }
            if (!doublon) {
                domaines.push_back(domaine) ;
            }
        }
    }
    void rotation(double angle) {
        // Rotation de chaque point sauf le point central
        for (std::vector<Sommet*>::size_type i = 1; i < sommets.size(); ++i) {
            double x = sommets[i]->x ;
            double y = sommets[i]->y ;

            // Rotation de π/2
            double nouveauX = x * cos(angle) - y * sin(angle) ;
            double nouveauY = x * sin(angle) + y * cos(angle) ;

            // Mise à jour du sommet
            sommets[i]->x = nouveauX ;
            sommets[i]->y = nouveauY ;
        }
    }
    void translation(double deltaX, double deltaY) {
        for (vector<Sommet*>::size_type i = 0; i < sommets.size(); ++i) {
            sommets[i]->x += deltaX ;
            sommets[i]->y += deltaY ;
        }
    }
    void homothetie(double facteur) {
        for (vector<Sommet*>::size_type i = 1; i < sommets.size(); ++i) {
            double x = sommets[i]->x ;
            double y = sommets[i]->y ;

            // Homothétie
            double nouveauX = x * facteur ;
            double nouveauY = y * facteur ;

            // Mise à jour du sommet
            sommets[i]->x = nouveauX ;
            sommets[i]->y = nouveauY ;
        }
    }
    void symetriePoint(double pointX, double pointY) {
    // Symétrie par rapport à un point
        for (vector<Sommet*>::size_type i = 0; i < sommets.size(); ++i) {
            // Distances au point
            double delta_x = sommets[i]->x - pointX ;
            double delta_y = sommets[i]->y - pointY ;

            // Symétrie par rapport au point spécifié
            sommets[i]->x = pointX - delta_x ;
            sommets[i]->y = pointY - delta_y ;
        }
    }
    void symetrieAxeOblique(double x0, double y0, double angle) {
        // L'axe est renseigné par un de ses points (x0,y0), et son orientation angle
        for (vector<Sommet*>::size_type i = 0; i < sommets.size(); ++i) {
            double delta_x = sommets[i]->x - x0 ;
            double delta_y = sommets[i]->y - y0 ;

            // Distance à l'axe
            double z = delta_x * sin(angle) - delta_y * cos(angle) ;

            // Mise à jour du sommet
            sommets[i]->x -= 2 * z * sin(angle) ;
            sommets[i]->y += 2 * z * cos(angle) ;
        }
    }
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

private:
    double largeur, hauteur ;
    int nx, ny ;

    void genererMaillageRectangle(double largeur, double hauteur, int nx, int ny) {
    // Cas régulier : pas constants nx et ny spécifiés
        double precision = 1e-14;                           // Correction des erreurs epsilon machine

        double px = largeur / nx ;
        double py = hauteur / ny ;

        // Générer les sommets
        for (int j = 0; j <= ny; ++j) {
            for (int i = 0; i <= nx; ++i) {
                double x = i * px ; 
                double y = j * py ;
                x = round(x / precision) * precision ;      // Correction des erreurs epsilon machine
                y = round(y / precision) * precision ;
                sommets.push_back(new Sommet(x, y)) ;
            }
        }

        // Générer les triangles
        for (int j = 0; j < ny; ++j) {
            for (int i = 0; i < nx; ++i) {
                int index1 = i + j * (nx + 1) ;
                int index2 = index1 + 1 ;
                int index3 = index1 + nx + 1 ;
                int index4 = index3 + 1 ;
                if (rand() % 2 == 0) {      // Coupage aléatoire des rectangles élémentaires
                    triangles.push_back(new Triangle(sommets[index1], sommets[index3], sommets[index2])) ;
                    aretes.push_back(new Arete(triangles.back(), 1)) ;      // Génération des aretes
                    aretes.push_back(new Arete(triangles.back(), 2)) ;
                    aretes.push_back(new Arete(triangles.back(), 3)) ;
                    triangles.push_back(new Triangle(sommets[index2], sommets[index3], sommets[index4])) ;
                    aretes.push_back(new Arete(triangles.back(), 1)) ;
                    aretes.push_back(new Arete(triangles.back(), 2)) ;
                    aretes.push_back(new Arete(triangles.back(), 3)) ;
                }
                else {
                    triangles.push_back(new Triangle(sommets[index1], sommets[index2], sommets[index4])) ;
                    aretes.push_back(new Arete(triangles.back(), 1)) ;
                    aretes.push_back(new Arete(triangles.back(), 2)) ;
                    aretes.push_back(new Arete(triangles.back(), 3)) ;
                    triangles.push_back(new Triangle(sommets[index3], sommets[index1], sommets[index4])) ;
                    aretes.push_back(new Arete(triangles.back(), 1)) ;
                    aretes.push_back(new Arete(triangles.back(), 2)) ;
                    aretes.push_back(new Arete(triangles.back(), 3)) ;
                }
            }
        }
    }
    void genererMaillageRectangle(double largeur, double hauteur, const vector<double>& abscisses, const vector<double>& ordonnees) {
    // Cas non régulier : listes d’abcisses et d’ordonnées spécifiées
        // Message d'erreur si malformatage des listes
        if (abscisses.size() < 2 || ordonnees.size() < 2) {
            cerr <<"Erreur : Les listes d'abcisses et d'ordonnées doivent contenir au moins deux valeurs chacune." <<endl ;
            return ;
        }

        double precision = 1e-14;                           // Correction des erreurs epsilon machine

        // Générer les sommets
        for (size_t j = 0; j < ordonnees.size(); ++j) {
            for (size_t i = 0; i < abscisses.size(); ++i) {
                double x = abscisses[i] ;
                double y = ordonnees[j] ;
                x = round(x / precision) * precision ;      // Correction des erreurs epsilon machine
                y = round(y / precision) * precision ;
                sommets.push_back(new Sommet(x, y)) ;
            }
        }

        // Générer les triangles
        for (size_t j = 0; j < ordonnees.size() - 1; ++j) {
            for (size_t i = 0; i < abscisses.size() - 1; ++i) {
                int index1 = i + j * abscisses.size() ;
                int index2 = index1 + 1 ;
                int index3 = index1 + abscisses.size() ;
                int index4 = index3 + 1 ;
                if (rand() % 2 == 0) {      // Coupage aléatoire des rectangles élémentaires
                    triangles.push_back(new Triangle(sommets[index1], sommets[index3], sommets[index2])) ;
                    aretes.push_back(new Arete(triangles.back(), 1)) ;      // Génération des aretes
                    aretes.push_back(new Arete(triangles.back(), 2)) ;
                    aretes.push_back(new Arete(triangles.back(), 3)) ;
                    triangles.push_back(new Triangle(sommets[index2], sommets[index3], sommets[index4])) ;
                    aretes.push_back(new Arete(triangles.back(), 1)) ;
                    aretes.push_back(new Arete(triangles.back(), 2)) ;
                    aretes.push_back(new Arete(triangles.back(), 3)) ;
                }
                else {
                    triangles.push_back(new Triangle(sommets[index1], sommets[index2], sommets[index4])) ;
                    aretes.push_back(new Arete(triangles.back(), 1)) ;
                    aretes.push_back(new Arete(triangles.back(), 2)) ;
                    aretes.push_back(new Arete(triangles.back(), 3)) ;
                    triangles.push_back(new Triangle(sommets[index3], sommets[index1], sommets[index4])) ;
                    aretes.push_back(new Arete(triangles.back(), 1)) ;
                    aretes.push_back(new Arete(triangles.back(), 2)) ;
                    aretes.push_back(new Arete(triangles.back(), 3)) ;
                }
            }
        }
    }
};

class MaillageSecteurAngulaire : public Triangulation { // Classe dérivée de la classe Triangulation
public:
    MaillageSecteurAngulaire(double rayon, double angle, int N) : Triangulation(), rayon(rayon), angle(angle), N(N) {
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