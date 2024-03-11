#include "maillage.hpp"

#include <list>         // Utilisation de list
#include <map>          // Utilisation de map
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

//====================================================================================================
bool Sommet::operator==(const Sommet& autre) const {    // Surcharge de l'opérateur ==
    return (x == autre.x) && (y == autre.y) ;
};

//====================================================================================================
Triangle::Triangle(Sommet* s1, Sommet* s2, Sommet* s3){ // Constructeur du triangle
    sommets[0] = s1 ;
    sommets[1] = s2 ;
    sommets[2] = s3 ;
}
bool Triangle::operator==(const Triangle& autre) const {  // Surcharge de l'opérateur ==
    for (int i = 0; i < 3; ++i) {
        if (!(*sommets[i] == *autre.sommets[i])) {
            return false ;
        }
    }
    return true ;
}
bool Triangle::in_triangle(const Sommet& point) const { // vérifie si un point est dans un triangle
    Sommet A = *sommets[0];
    Sommet B = *sommets[1];
    Sommet C = *sommets[2];
// Vérifier si le point est du même côté de chaque côté du triangle.
    double dotAB = (B.x - A.x) * (point.y - A.y) - (B.y - A.y) * (point.x - A.x);
    double dotBC = (C.x - B.x) * (point.y - B.y) - (C.y - B.y) * (point.x - B.x);
    double dotCA = (A.x - C.x) * (point.y - C.y) - (A.y - C.y) * (point.x - C.x);

    return (dotAB > 0 && dotBC > 0 && dotCA > 0) || (dotAB < 0 && dotBC < 0 && dotCA < 0);
}
Sommet Triangle::circumcenter() const {
    double x1 = sommets[0]->x;
    double y1 = sommets[0]->y;
    double x2 = sommets[1]->x;
    double y2 = sommets[1]->y;
    double x3 = sommets[2]->x;
    double y3 = sommets[2]->y;
    double D = 2 * (x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2));
    double Ux = ((x1 * x1 + y1 * y1) * (y2 - y3) + (x2 * x2 + y2 * y2) * (y3 - y1) + (x3 * x3 + y3 * y3) * (y1 - y2)) / D;
    double Uy = ((x1 * x1 + y1 * y1) * (x3 - x2) + (x2 * x2 + y2 * y2) * (x1 - x3) + (x3 * x3 + y3 * y3) * (x2 - x1)) / D;
    return Sommet(Ux, Uy);
}
double Triangle::circumradius() const {
    double x0 = circumcenter().x;
    double y0 = circumcenter().y;
    double x1 = sommets[0]->x;
    double y1 = sommets[0]->y;
    return sqrt((x0-x1)*(x0-x1)+(y0-y1)*(y0-y1));
}
bool Triangle::in_circle_triangle(const Sommet& point) const {
    Sommet circumcenter_point = circumcenter();
    double radius = circumradius();
    // Distance entre le point et le centre du cercle circonscrit : si la distance est inférieure ou égale au rayon, le point est dans le cercle
    double distance = sqrt((point.x - circumcenter_point.x) * (point.x - circumcenter_point.x) + (point.y - circumcenter_point.y) * (point.y - circumcenter_point.y));
    return distance <= radius;
}

//====================================================================================================
bool Arete::operator==(const Arete& autre) const {      // Surcharge de l'opérateur ==
    return (triangle == autre.triangle) && (num == autre.num) ;
}
Sommet& Arete::determine_sommet(int num_sommet) const { // Renvoie le (num_sommet)-ième sommet de l'arrête
    Sommet* sommets_trier[3] ;
    int i_minx=0;
    for(int i=0;i<3;++i){
        if( ((triangle->sommets)[i_minx])->x > ((triangle->sommets)[i])->x ){
            i_minx=i;
        }
    }
    sommets_trier[0]=triangle->sommets[i_minx];
    int i_maxy=0;
    for(int i=0;i<3;++i){
        if(( ((triangle->sommets)[i_maxy])->y < ((triangle->sommets)[i])->y )&&(i!=i_minx)){
            i_maxy=i;
        }
    }
    sommets_trier[1]=triangle->sommets[i_maxy];
    return(*(sommets_trier[(num_sommet+num-1)%3])) ;
}
float Arete::determine_longueur() const {               // Renvoie la longueur de l'arete
    Sommet& sommet_1 = determine_sommet(0);
    Sommet& sommet_2 = determine_sommet(1);
    float longueur = sqrt((sommet_1.x - sommet_2.x)*(sommet_1.x - sommet_2.x) + (sommet_1.y - sommet_2.y)*(sommet_1.y - sommet_2.y)) ;
    return(longueur) ;
}
bool Arete::operator|(const Arete& autre) const {
    Sommet A = autre.determine_sommet(0) ;
    Sommet B = autre.determine_sommet(1) ;
    Sommet C = determine_sommet(0) ;
    Sommet D = determine_sommet(1) ;
    return(((B.x-A.x)*(C.y-A.y)-(B.y-A.y)*(C.x-A.x))*((B.x-A.x)*(D.y-A.y)-(B.y-A.y)*(D.x-A.x))<0); 
    /* Le signe des déterminants nous indique la position relative d'un point par rapport a une arrête.
    Si le signe du produit des deux déterminants est négatif cela signifie que les points sont de parté autre du segment.
    Il y a donc croisement.*/
}
bool Arete::operator<(const Arete& autre) const { // Opérateur de comparaison pour trier les arêtes par taille.
    double taille = this->determine_longueur();
    double autre_taille = autre.determine_longueur();
    return taille < autre_taille;
}
Segment::Segment(Sommet* s1, Sommet* s2){ // Constructeur du segment
    sommets[0] = s1 ;
    sommets[1] = s2 ;
}
float Segment::longueur() const{
    return(sqrt((sommets[1]->y - sommets[0]->y)*(sommets[1]->y - sommets[0]->y) + (sommets[1]->x - sommets[0]->x)*(sommets[1]->x - sommets[0]->x)));
}
bool Segment::operator==(const Segment& autre) const {      // Surcharge de l'opérateur ==
    return (sommets[0]->x == autre.sommets[0]->x && sommets[0]->y == autre.sommets[0]->y && sommets[1]->x == autre.sommets[1]->x && sommets[1]->y == autre.sommets[1]->y ) ;
}
bool Segment::operator|(const Segment& autre) const {
    Sommet* A = autre.sommets[0] ;
    Sommet* B = autre.sommets[1] ;
    Sommet* C = sommets[0] ;
    Sommet* D = sommets[1] ;
    return(((B->x - A->x)*(C->y - A->y)-(B->y - A->y)*(C->x - A->x))*((B->x - A->x)*(D->y - A->y)-(B->y - A->y)*(D->x - A->x))<0);
}
bool Segment::operator<(const Segment& autre) const{
    double taille = this->longueur();
    double autre_taille = autre.longueur();
    return taille < autre_taille;
}
double Segment::operator,(const Segment& autre) {        // Produit scalaire
    return((sommets[1]->x - sommets[0]->x)*(autre.sommets[1]->x - autre.sommets[0]->x) + (sommets[1]->y - sommets[0]->y)*(autre.sommets[1]->y - autre.sommets[0]->y));
}
//====================================================================================================
bool Domaine::operator==(const Domaine& autre) const {       // Surcharge de l'opérateur ==
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


//====================================================================================================
//                              Méthode Frontale et ses dérivées
//====================================================================================================
Front::Front(const Segment** Nsegments, vector<Sommet> Npoints) {
    for (int i = 0; i < int(sizeof(Nsegments)); ++i) {
        ajouterSegment(Nsegments[i]);
    }
    for (int i = 0; i < int(sizeof(Npoints)); ++i) {
        ajouterPoint(Npoints[i]);
    }
}
Front::Front(const Segment** Nsegments, int taille) {
    for  (int i = 0; i < taille; ++i) {
        // cout << "iteration " << i << endl;
        ajouterSegment(Nsegments[i]);
        for (int j = 0; j < 2; ++j) {
            ajouterPoint(*(*Nsegments)->sommets[j]);
            // cout << "Point (" << j << ") : " ;
            // cout << ((Nsegments[i]->sommets[j])->x) << " , ";
            // cout << ((Nsegments[i]->sommets[j])->y) << " ajoute" << endl;
        }
    }
}
int Front::compteSegment() {
    int n_segment = 0;
    // Parcours des segments dans l'objet Front
    for (const auto& pair : segments) {
        // Pour chaque paire clé-valeur (double, list<const Segment*>),
        // ajouter la taille de la liste à n_segment
        n_segment += pair.second.size();
    }
    return n_segment;
}
void Front::ajouterSegment(const Segment* psegment) {     // Ajouter une arête au front
    // Rechercher de la liste correspondant à la taille de l'arête
    auto it = segments.find(psegment->longueur());

    // Si une liste pour cette taille existe déjà, ajouter le pointeur vers l'arête à cette liste
    if (it != segments.end()) {
        it->second.push_back(psegment);
    } else { // Sinon, crée une nouvelle liste contenant uniquement ce pointeur vers l'arête
            segments[psegment->longueur()] = list<const Segment*>{psegment};
    }
}
void Front::supprimerSegment(const Segment* segment) {    // Supprime une arête de la liste associée à sa taille
    // Recherche de la liste correspondant à la taille de l'arête
    auto it = segments.find(segment->longueur());

    // Si une liste pour cette taille existe, recherche et supprime l'arête
    if (it != segments.end()) {
        it->second.remove(segment);
        // Vérifier si la liste est devenue vide après la suppression
        if (it->second.empty()) {
            // Si la liste est vide, supprimer cette entrée du map
            segments.erase(it);
        }
    }
}
void Front::ajouterPoint(const Sommet& point) {     // Ajoute un point à la liste des points
    points.push_back(point);
}
void Front::supprimerPoint(const Sommet& point) {
    for (auto it = points.begin(); it != points.end(); ++it) {
        if (*it == point) {
            points.erase(it);
            break; // Sortir de la boucle une fois que le point est trouvé et supprimé
        }
    }
};
bool Front::int_front(const Sommet Point){ 
    // Méthode pour vérifier si un point est dans le front
    // On parcourt tous les segments du front et pour chaque segment,
    // on vérifie si le triplet (premier point du segment, deuxième point du segment, point considéré) est orienté dans le sens direct)
    for (auto it = segments.begin(); it != segments.end(); ++it) {
        for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
            if ( ((*it2)->sommets[1]->x - (*it2)->sommets[0]->x)*(Point.y - (*it2)->sommets[0]->y) - ((*it2)->sommets[1]->y - (*it2)->sommets[0]->y)*(Point.x - (*it2)->sommets[0]->x) < 0) {
                return false;
            }
        }
    }
    return true;
}
void Front::miseajour(const Segment* seginit){
    // Parcourir les segments du front pour trouver le segment précédant seginit (dans le sens trigo)
    // C'est à dire le segment qui a pour sommets[1] le sommet[0] de smallestSegment
    const Segment* segprec ;
    for (auto it = segments.begin(); it != segments.end(); ++it) {
        for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
            if (*((*it2)->sommets[1]) == *(seginit->sommets[0])) {
                segprec = *it2 ;
                break;
            }
        }
    }
    // Parcourir les segments du front pour trouver les segments suivant segprec (dans le sens trigo)
    // C'est à dire les segments qui ont pour sommets[0] le sommet[1] de segprec
    vector<const Segment*> segments_suivants_segprec;
    for (auto it = segments.begin(); it != segments.end(); ++it) {
        for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
            if (*((*it2)->sommets[0]) == *(segprec->sommets[1])) {
                segments_suivants_segprec.push_back(*it2);
            }
        }
    }
    // Calculs des produits scalaires permettant d'établir lequel des segments de segments_suivants_segprec
    // forme le plus grand angle avec segprec dans le sens direct
    const Segment * segment_a_garder ;
    double min = 2*M_PI ;
    for (const Segment* seg : segments_suivants_segprec) {
        double xsegprec = segprec->sommets[0]->x - segprec->sommets[1]->x ;
        double ysegprec = segprec->sommets[0]->y - segprec->sommets[1]->y ;
        double xseg = seg->sommets[1]->x - seg->sommets[0]->x ;
        double yseg = seg->sommets[1]->y - seg->sommets[0]->y ;
        double prodscal = xsegprec*xseg + ysegprec*yseg ;
        double angle = acos(prodscal/(sqrt(xsegprec*xsegprec + ysegprec*ysegprec) * sqrt(xseg*xseg + yseg*yseg))) ;
        if (angle <= min) {
            min = angle ;
            segment_a_garder = seg ;
        }                
    }
    // Suppression de segments de tous les segments de segments_suivants_segprec autres que segment_a_garder
    for (const Segment* seg : segments_suivants_segprec) {
        if (!(*seg == *segment_a_garder)) {
            supprimerSegment(seg);
        }
    }
}
void Front::print() {
    for (const auto& pair : segments) {
        for (const auto& segment : pair.second) {
            cout << "(" << segment->sommets[0]->x << ", " << segment->sommets[0]->y << ") ";
            cout << "(" << segment->sommets[1]->x << ", " << segment->sommets[1]->y << ") ";
        }
    }
    cout << endl;
}
vector<Triangle> Front::genererTriangle() { 
    vector<Triangle> nouvTriangles ;    // Triangles de sortie
    if (segments.empty()) {
        cerr << "Erreur: Aucun segment disponible pour générer des triangles." << endl;
        return nouvTriangles;
    }
    const Segment* smallestSegment = segments.begin()->second.front();  // Récupérer le plus petit segment de la map
    float longueur = smallestSegment->longueur();
    // Coordonnées du troisième point formant un triangle équilatéral avec smallestSegment
    double x3 = (smallestSegment->sommets[1]->x + smallestSegment->sommets[0]->x) / 2.0 - (smallestSegment->sommets[1]->y - smallestSegment->sommets[0]->y) * (sqrt(3) / 2.0) ;
    double y3 = (smallestSegment->sommets[1]->y + smallestSegment->sommets[0]->y) / 2.0 + (smallestSegment->sommets[1]->x - smallestSegment->sommets[0]->x) * (sqrt(3) / 2.0) ;
    double precision = 1e-14 ;
    x3 = round(x3 / precision) * precision ;        // Correction des erreurs epsilon machine
    y3 = round(y3 / precision) * precision ;
    Sommet* thirdPoint = new Sommet(x3, y3);
    
    // Si un point du front est à une distance suffisamment proche, l'utiliser comme troisième point
    for (Sommet& point : points) {
        double distance = sqrt(pow(point.x - x3, 2) + pow(point.y - y3, 2));
        if (distance < longueur / 10.0) {
            thirdPoint = &point ;
            break;
        }
    }
    cout << "thirdPoint = (" << thirdPoint->x << ", " << thirdPoint->y << ")" << endl;

    Triangle superTriangle((smallestSegment->sommets)[0], (smallestSegment->sommets)[1], thirdPoint);   // Super triangle de la méthode de Delaunay

    // Algorithme de Bowyer-Watson :
    // Liste des points intérieurs au super triangle
    vector<Sommet> points_int;
    for (const Sommet& point : points) {
        if (superTriangle.in_triangle(point)) {
            points_int.push_back(point);
        }
    }
    if (points_int.empty()) {
        if (!int_front(*thirdPoint)) {  // Vérifier si le sommet est à l'intérieur du front
        // Déterminer ce que l'on fait dans ce cas
            return nouvTriangles;
        }
        else {
            nouvTriangles.push_back(superTriangle);
            cout << "Super triangle ajouté." << endl;
            // Ajouter les segments du super triangle au front
            Segment* seg1 = new Segment((smallestSegment->sommets)[0], thirdPoint);
            Segment* seg2 = new Segment(thirdPoint, (smallestSegment->sommets)[1]);
            ajouterSegment(seg1);
            ajouterSegment(seg2);
            miseajour(seg1);
            return nouvTriangles;
        }
    }
    // Créer trois triangles en reliant le premier point aux sommets du super triangle
    Triangle triangle1((superTriangle.sommets)[0], (superTriangle.sommets)[1], &(points_int.front()));
    Triangle triangle2((superTriangle.sommets)[1], (superTriangle.sommets)[2], &(points_int.front()));
    Triangle triangle3((superTriangle.sommets)[2], (superTriangle.sommets)[0], &(points_int.front()));
    // Ajouter ces triangles à la liste de triangles de sortie
    nouvTriangles.push_back(triangle1);
    nouvTriangles.push_back(triangle2);
    nouvTriangles.push_back(triangle3);
    // Supprimer le premier point de la liste intérieure
    points_int.erase(points_int.begin());

    for (Sommet& pointk : points_int) {     // Parcourt des points intérieurs au super triangle
        // Triangles dont les cercles circonscrits contiennent pointk
        vector<Triangle> Tk ;
        for (auto it = nouvTriangles.begin(); it != nouvTriangles.end(); ++it) {
            if ((*it).in_circle_triangle(pointk)) {
                Tk.push_back(*it) ;
                if (Tk.size() == 2) {
                    break;
                }
            }
        }
        vector<Sommet> Tk_sommets;              // Sommets des triangles de Tk
        for (const auto& triangle : Tk) {
            for (const auto& sommet : triangle.sommets) {
                // Vérifie si le sommet existe déjà dans Tk_sommets
                auto it = find(Tk_sommets.begin(), Tk_sommets.end(), *sommet);
                if (it == Tk_sommets.end()) {
                    // Si le sommet n'est pas déjà présent, l'ajouter à Tk_sommets_uniques
                    Tk_sommets.push_back(*sommet);
                }
            }
        }
        // Supprimer de nouvTriangles les triangles Tk :
        for (Triangle& triangle : Tk) {
            nouvTriangles.erase(remove(nouvTriangles.begin(), nouvTriangles.end(), triangle), nouvTriangles.end());
        }    
        // Créer les 4 triangles reliant pointk aux 4 sommets de Tk_sommets :
        int n = Tk_sommets.size() ;
        for (int i = 0; i < n ; ++i) {
            Triangle triangle(&Tk_sommets[i], &Tk_sommets[(i+1)%n], &pointk);
            nouvTriangles.push_back(triangle);
        }
    }
    for (auto it = nouvTriangles.begin(); it != nouvTriangles.end();) {
        Triangle& triangle = *it;
        bool removeTriangle = false;
        for (Sommet* sommet : triangle.sommets) {
            if (!int_front(*sommet)) {  // Vérifier si le sommet est à l'intérieur du front
                removeTriangle = true;
                break;
            }
        }
        if (removeTriangle) {   // Supprimer le triangle si un de ses sommets n'est pas à l'intérieur du front
            it = nouvTriangles.erase(it);
        } else {
            ++it;
        }
    }

    return nouvTriangles;
}

//====================================================================================================
void Triangulation::exportMATLAB(const string& nomFichier) const {
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
void Triangulation::fusionnerMaillages(const Triangulation& autre) {
    // Fusionner les sommets en supprimant les doublons
    for (const auto& sommet : autre.sommets) {
        bool doublon = false;
        for (const auto& existingSommet : sommets) {
            if (*sommet == *existingSommet) {
                doublon = true;
                // Mise à jour des pointeurs dans les triangles de la triangulation "autre"
                for (const auto& triangleAutre : autre.triangles) {
                    for (int i = 0; i < 3; ++i) {
                        if (*triangleAutre->sommets[i] == *sommet) {
                            triangleAutre->sommets[i] = existingSommet;
                        }
                    }
                }
                break;
            }
        }
        if (!doublon) {
            sommets.push_back(sommet);
        }
    }

    // Fusionner les triangles en supprimant les doublons
    for (const auto& triangle : autre.triangles) {
        bool doublon = false;
        for (const auto& existingTriangle : triangles) {
            if (*triangle == *existingTriangle) {
                doublon = true;
                break;
            }
        }
        if (!doublon) {
            triangles.push_back(triangle);
            for (int i = 0; i < 3; ++i) { // Ajouter les arêtes correspondantes
                Arete* arete = new Arete(triangle, i);
                for (const auto& existingArete : aretes) {
                    if (*arete == *existingArete) { // Sans doublon
                        doublon = true;
                        break;
                    }
                }
                if (!doublon) {
                    aretes.push_back(arete);
                }
            }
        }
    }

    // Fusionner les domaines en supprimant les doublons
    for (const auto& domaine : autre.domaines) {
        bool doublon = false;
        for (const auto& existingDomaine : domaines) {
            if (*existingDomaine == *domaine) {
                doublon = true;
                break;
            }
        }
        if (!doublon) {
            domaines.push_back(domaine);
        }
    }
}
void Triangulation::rotation(double angle) {
    // Rotation de chaque point sauf le point central
    for (std::vector<Sommet*>::size_type i = 1; i < sommets.size(); ++i) {
        double x = sommets[i]->x;
        double y = sommets[i]->y;

        // Rotation de π/2
        double nouveauX = x * cos(angle) - y * sin(angle);
        double nouveauY = x * sin(angle) + y * cos(angle);

        // Mise à jour du sommet
        sommets[i]->x = nouveauX;
        sommets[i]->y = nouveauY;
    }
}
void Triangulation::translation(double deltaX, double deltaY) {
    for (vector<Sommet*>::size_type i = 0; i < sommets.size(); ++i) {
        sommets[i]->x += deltaX;
        sommets[i]->y += deltaY;
    }
}
void Triangulation::homothetie(double facteur) {
    for (vector<Sommet*>::size_type i = 1; i < sommets.size(); ++i) {
        double x = sommets[i]->x;
        double y = sommets[i]->y;

        // Homothétie
        double nouveauX = x * facteur;
        double nouveauY = y * facteur;

        // Mise à jour du sommet
        sommets[i]->x = nouveauX;
        sommets[i]->y = nouveauY;
    }
}
void Triangulation::symetriePoint(double pointX, double pointY) {
    // Symétrie par rapport à un point
    for (vector<Sommet*>::size_type i = 0; i < sommets.size(); ++i) {
        // Distances au point
        double delta_x = sommets[i]->x - pointX;
        double delta_y = sommets[i]->y - pointY;

        // Symétrie par rapport au point spécifié
        sommets[i]->x = pointX - delta_x;
        sommets[i]->y = pointY - delta_y;
    }
}
void Triangulation::symetrieAxeOblique(double x0, double y0, double angle) {
    // L'axe est renseigné par un de ses points (x0,y0), et son orientation angle
    for (vector<Sommet*>::size_type i = 0; i < sommets.size(); ++i) {
        double delta_x = sommets[i]->x - x0;
        double delta_y = sommets[i]->y - y0;

        // Distance à l'axe
        double z = delta_x * sin(angle) - delta_y * cos(angle);

        // Mise à jour du sommet
        sommets[i]->x -= 2 * z * sin(angle);
        sommets[i]->y += 2 * z * cos(angle);
    }
}

//====================================================================================================
void MaillageRectangle::genererMaillageRectangle(double largeur, double hauteur, int nx, int ny) {
    // Cas régulier : pas constants nx et ny spécifiés
    double precision = 1e-14; // Correction des erreurs epsilon machine

    double px = largeur / nx;
    double py = hauteur / ny;

    // Générer les sommets
    for (int j = 0; j <= ny; ++j) {
        for (int i = 0; i <= nx; ++i) {
            double x = i * px;
            double y = j * py;
            x = round(x / precision) * precision; // Correction des erreurs epsilon machine
            y = round(y / precision) * precision;
            sommets.push_back(new Sommet(x, y));
        }
    }

    // Générer les triangles
    for (int j = 0; j < ny; ++j) {
        for (int i = 0; i < nx; ++i) {
            int index1 = i + j * (nx + 1);
            int index2 = index1 + 1;
            int index3 = index1 + nx + 1;
            int index4 = index3 + 1;
            if (rand() % 2 == 0) { // Coupage aléatoire des rectangles élémentaires
                triangles.push_back(new Triangle(sommets[index1], sommets[index3], sommets[index2]));
                aretes.push_back(new Arete(triangles.back(), 1)); // Génération des aretes
                aretes.push_back(new Arete(triangles.back(), 2));
                aretes.push_back(new Arete(triangles.back(), 3));
                triangles.push_back(new Triangle(sommets[index2], sommets[index3], sommets[index4]));
                aretes.push_back(new Arete(triangles.back(), 1));
                aretes.push_back(new Arete(triangles.back(), 2));
                aretes.push_back(new Arete(triangles.back(), 3));
            } else {
                triangles.push_back(new Triangle(sommets[index1], sommets[index2], sommets[index4]));
                aretes.push_back(new Arete(triangles.back(), 1));
                aretes.push_back(new Arete(triangles.back(), 2));
                aretes.push_back(new Arete(triangles.back(), 3));
                triangles.push_back(new Triangle(sommets[index3], sommets[index1], sommets[index4]));
                aretes.push_back(new Arete(triangles.back(), 1));
                aretes.push_back(new Arete(triangles.back(), 2));
                aretes.push_back(new Arete(triangles.back(), 3));
            }
        }
    }
}
void MaillageRectangle::genererMaillageRectangle(double largeur, double hauteur, const vector<double>& abscisses, const vector<double>& ordonnees) {
    // Cas non régulier : listes d’abcisses et d’ordonnées spécifiées
    // Message d'erreur si malformatage des listes
    if (abscisses.size() < 2 || ordonnees.size() < 2) {
        cerr <<"Erreur : Les listes d'abcisses et d'ordonnées doivent contenir au moins deux valeurs chacune." <<endl;
        return;
    }

    double precision = 1e-14; // Correction des erreurs epsilon machine

    // Générer les sommets
    for (size_t j = 0; j < ordonnees.size(); ++j) {
        for (size_t i = 0; i < abscisses.size(); ++i) {
            double x = abscisses[i];
            double y = ordonnees[j];
            x = round(x / precision) * precision; // Correction des erreurs epsilon machine
            y = round(y / precision) * precision;
            sommets.push_back(new Sommet(x, y));
        }
    }

    // Générer les triangles
    for (size_t j = 0; j < ordonnees.size() - 1; ++j) {
        for (size_t i = 0; i < abscisses.size() - 1; ++i) {
            int index1 = i + j * abscisses.size();
            int index2 = index1 + 1;
            int index3 = index1 + abscisses.size();
            int index4 = index3 + 1;
            if (rand() % 2 == 0) { // Coupage aléatoire des rectangles élémentaires
                triangles.push_back(new Triangle(sommets[index1], sommets[index3], sommets[index2]));
                aretes.push_back(new Arete(triangles.back(), 1)); // Génération des aretes
                aretes.push_back(new Arete(triangles.back(), 2));
                aretes.push_back(new Arete(triangles.back(), 3));
                triangles.push_back(new Triangle(sommets[index2], sommets[index3], sommets[index4]));
                aretes.push_back(new Arete(triangles.back(), 1));
                aretes.push_back(new Arete(triangles.back(), 2));
                aretes.push_back(new Arete(triangles.back(), 3));
            } else {
                triangles.push_back(new Triangle(sommets[index1], sommets[index2], sommets[index4]));
                aretes.push_back(new Arete(triangles.back(), 1));
                aretes.push_back(new Arete(triangles.back(), 2));
                aretes.push_back(new Arete(triangles.back(), 3));
                triangles.push_back(new Triangle(sommets[index3], sommets[index1], sommets[index4]));
                aretes.push_back(new Arete(triangles.back(), 1));
                aretes.push_back(new Arete(triangles.back(), 2));
                aretes.push_back(new Arete(triangles.back(), 3));
            }
        }
    }
}

//====================================================================================================
MaillageSecteurAngulaire::MaillageSecteurAngulaire(double rayon, double angle, int N) : Triangulation(), rayon(rayon), angle(angle), N(N) {
    genererSecteurAngulaire() ;
}// Constructeur
void MaillageSecteurAngulaire::generersecteurangleaigu() {
    double precision = 1e-14; // Correction des erreurs epsilon machine

    // Générer les sommets
    Sommet* centre = new Sommet(0.0, 0.0); // Génération du point central
    sommets.push_back(centre);
    for (int i = 1; i < N; ++i) { // Génération par arc
        // Arcs indicés par i (le point central n'est pas un arc ici)
        double rayoninterieur = i * rayon / (N - 1);
        double theta = angle / i;
        for (int j = 0; j <= i; ++j) { // Il y a i+1 points dans le i-ème arc
            double x = rayoninterieur * cos(j * theta);
            double y = rayoninterieur * sin(j * theta);
            x = round(x / precision) * precision; // Correction des erreurs epsilon machine
            y = round(y / precision) * precision;
            sommets.push_back(new Sommet(x, y));
        }
    }

    // Générer les triangles
    for (int i = 1; i < N; ++i) {
        // Nouvelle indiciation des arcs : le point central est un arc indicé 1
        for (int j = 0; j < i; ++j) { // Il y a donc i points dans le i-ème arc
            int index1 = i * (i - 1) / 2 + j;
            int index2 = (i + 1) * i / 2 + j;
            int index3 = (i + 1) * i / 2 + j + 1;
            triangles.push_back(new Triangle(sommets[index1], sommets[index2], sommets[index3]));
        }
    }
}
void MaillageSecteurAngulaire::genererSecteurAngulaire() {
    angle = (angle == 2 * M_PI) ? angle : fmod(angle + 2 * M_PI, 2 * M_PI); // Normalisation de angle

    if (angle <= M_PI / 2) {
        generersecteurangleaigu();
    } else {
        // k = nombre de sous-maillages du découpage
        int k = (angle == 2 * M_PI) ? 4 : (angle == M_PI) ? 2 : floor(angle * 2 / M_PI) + 1;
        // Découpage en k maillages identiques d'angle aigu
        double anglek = angle / k;
        MaillageSecteurAngulaire maillage(rayon, anglek, floor(N / k) + 1);
        for (int i = 1; i < k; ++i) {
            MaillageSecteurAngulaire maillagei(rayon, anglek, floor(N / k) + 1);
            maillagei.rotation(i * anglek);
            maillage.fusionnerMaillages(maillagei);
        }
        fusionnerMaillages(maillage); // Assignation au maillage courant
    }
}
MaillageFront::MaillageFront(Front* AFront) : Triangulation(), TheFront(AFront) {}
MaillageFront::MaillageFront(char modele, float Hpas): Triangulation() {
    // Remplir le Front en fonction de différent modèle
    // Polygone hasard :
}
bool MaillageFront::MethodeFrontal(){
    if (TheFront->empty()) {
        std::cerr << "Error: No Front To Proceed" << std::endl; 
        return  false;
    }
    bool State = true;
    while (State) {
        // Appliquer GenererTriangle
        vector<Triangle> FrontCalc = TheFront->genererTriangle();
        for(int i = 0; i < int(sizeof(FrontCalc)); ++i){
            Triangle* New_Triangle = &FrontCalc[i];
            Sommet* New_Sommet1 = New_Triangle->sommets[1];
            Sommet* New_Sommet2 = New_Triangle->sommets[2];
            Sommet* New_Sommet3 = New_Triangle->sommets[3];
            triangles.push_back(New_Triangle);
            if(*find(sommets.begin(), sommets.end(), New_Sommet1 ) != New_Sommet1) {
                sommets.push_back(New_Sommet1);
            }
            if(*find(sommets.begin(), sommets.end(), New_Sommet2 ) != New_Sommet2) {
                sommets.push_back(New_Sommet2);
            }
            if(*find(sommets.begin(), sommets.end(), New_Sommet3 ) != New_Sommet3) {
                sommets.push_back(New_Sommet3);
            }
        }
        if(TheFront->compteSegment() == 3){
            State = false;
        }
        if(TheFront->compteSegment() == 0){
            std::cerr << "Error: No Convergence of Frontal Method" << std::endl; 
            return  false;
        }
    }
    // Création du dernier triangle
    Triangle* Final_Triangle;
    int i_state = 0;
    for (const auto& pair : TheFront->segments) {
        list<const Segment*> listeSegments = pair.second;
        for (const Segment* segment : listeSegments) {
            if(!(Final_Triangle->in_triangle(*(segment->sommets[0])))){
                Final_Triangle->sommets[i_state] = segment->sommets[0];
                i_state = i_state + 1;
                if(*find(sommets.begin(), sommets.end(), segment->sommets[0] ) != segment->sommets[0]) {
                    sommets.push_back(segment->sommets[0]);
                }
            }
            if(!(Final_Triangle->in_triangle(*(segment->sommets[1])))){
                Final_Triangle->sommets[i_state] = segment->sommets[1];
                i_state = i_state + 1;
                if(*find(sommets.begin(), sommets.end(), segment->sommets[1] ) != segment->sommets[1]) {
                    sommets.push_back(segment->sommets[1]);
                }
            }
            if(i_state == 3){
                break;
            }
        }
        if(i_state == 3){
                break;
        }
    }
    triangles.push_back(Final_Triangle);
    return true;
}













///////////////////////////////////////////////////////////////////////////////////
//sauvegarder le maillage
//à appeler à chaque modification du maillage (ajout d'un triangle ou sommet)
///////////////////////////////////////////////////////////////////////////////////

void Triangulation::save(){
    ofstream fichier("historique_M.txt");   // Fichier mémoire
    for (const auto& sommet : sommets) {
        fichier <<"S " ;                    // Empreinte sommet
        fichier <<sommet->x <<" " <<sommet->y <<" " ;   // Écriture des sommets
        }
    for (const auto& triangle : triangles) {    //ecriture des triangles
        fichier << "T " ;   // Empreinte triangles
        for (size_t i = 0; i < sommets.size(); ++i) {
            if (triangle->sommets[0] == sommets[i] || 
                triangle->sommets[1] == sommets[i] ||
                triangle->sommets[2] == sommets[i]) {
                    fichier <<i+1 <<" " ;  
            }
        }
    }
    fichier <<"F" << "\n" ;         // Fin d'une étape
    fichier.close() ;
}

///////////////////////////////////////////////////////////////////////////////////
//sauvegarder le front
//à appeler à chaque modificatin du front (retrait ou ajout d'une arête)
///////////////////////////////////////////////////////////////////////////////////
void Front::save(){
    ofstream fichier("historique_F.txt");   // Fichier mémoire
    for (const auto& pair : segments) {
    // Parcours des segments dans l'objet Front
        for (const auto& segment : pair.second) {
            // Ecriture des sommets des segments
            fichier <<"A " ;                     // Empreinte arete (ou plutôt segment ici)
            fichier <<segment->sommets[0]->x <<" " <<segment->sommets[0]->y <<" " ;
            fichier <<segment->sommets[1]->x <<" " <<segment->sommets[1]->y <<" " ;
        }
    }
    fichier <<"F" << "\n" ;                 // Fin d'une étape
    fichier.close() ;
}