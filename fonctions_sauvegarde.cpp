#include <fstream>      // Lecture/écriture de fichiers
#include <list>         // Utilisation de list
#include <iostream>     // Entrée/sortie standard
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
    // Parcours des aretes dans l'objet Front
        for (const auto& segment : pair.second) {
            // Ecriture des sommets des segments
            fichier <<segment->sommets[0]->x <<" " <<segment->sommets[0]->y <<" " ;
            fichier <<segment->sommets[1]->x <<" " <<segment->sommets[1]->y <<" " ;
        }
    }
    fichier <<"F" << "\n" ;                 // Fin d'une étape
    fichier.close() ;
}