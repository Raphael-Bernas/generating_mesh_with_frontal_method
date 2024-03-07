#include <fstream>      // Lecture/écriture de fichiers
#include <list>         // Utilisation de list
#include <iostream>     // Entrée/sortie standard
///////////////////////////////////////////////////////////////////////////////////
//sauvegarder le maillage
//à appeler à chaque modification du maillage (ajout d'un triangle ou sommet)
///////////////////////////////////////////////////////////////////////////////////

void Triangulation::save(){
    ofstream fichier(historique_M); //fichier mémoire
    for (const auto& sommet : sommets) {        // Écriture des sommets
        fichier <<"S" ;    // Empreinte somemt
        fichier <<sommet->x <<" " <<sommet->y;
        }
    for (const auto& triangle : triangles) {    //ecriture des triangles
        fichier << "T" ;   // Empreinte triangles
        for (size_t i = 0; i < sommets.size(); ++i) {
            if (triangle->sommets[0] == sommets[i] || 
                triangle->sommets[1] == sommets[i] ||
                triangle->sommets[2] == sommets[i]) {
                    fichier <<i+1 <<" " ;  
            }
        }
    }
    fichier <<"F" ///fin d'une étape
    fichier.close() ;
}

///////////////////////////////////////////////////////////////////////////////////
//sauvegarder le front
//à appeler à chaque modificatin du front (retrait ou ajout d'une arête)
///////////////////////////////////////////////////////////////////////////////////
void Front::save(){
    ofstream fichier(historique_F); //fichier mémoire
    for (const auto& pair : segments) {
        fichier<<"S" //emprunte segment
        const list<const Segment*>& listeSegments = pair.second; // Liste des segments associé à une clé
        for (const Segment* segment : listeSegments) {
            fichier << segment->x <<" " << segment->y;

            
        }
    }
    fichier << "F"; //fin d'une étape
    fichier.close();
}
