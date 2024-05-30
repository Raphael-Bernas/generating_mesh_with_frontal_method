# SIM202
An english version of this Read.me can be found at the end.
# Execution :
Afin d'exécuter le code veuillez utiliser la commande 'make', pour supprimer les fichiers générées 'make clean'

# Maillage - Entête :

Ce fichier contient des classes et des méthodes pour la manipulation et la génération de maillages en C++.

## Classes Élémentaires

- `Sommet`: Représente un point dans l'espace défini par ses coordonnées `x` et `y`.
- `Triangle`: Représente un triangle défini par trois sommets.
- `Arete`: Représente une arête d'un triangle.
- `Segment`: Représente un segment de droite entre deux sommets.
- `Domaine`: Représente un domaine défini par des triangles et des arêtes.

## Méthode Frontale et ses dérivées

- `Front`: Gère une structure de données frontale pour la génération de triangles.

## Classe Triangulation et ses dérivées

- `Triangulation`: Gère une collection de sommets, de triangles, d'arêtes et de domaines.
- `MaillageRectangle`: Dérivée de `Triangulation`, permet de générer un maillage rectangulaire régulier ou non régulier.
- `MaillageSecteurAngulaire`: Dérivée de `Triangulation`, permet de générer un maillage pour un secteur angulaire donné.
- `MaillageFront`: Dérivée de `Triangulation`, utilise une méthode frontale pour générer un maillage.

# Maillage - Implémentation :

Ce fichier (`Maillage.cpp`) contient l'implémentation des classes et des méthodes pour la manipulation et la génération de maillages triangulaires en C++.

## Classes Élémentaires

### `Sommet`
- Surcharge de l'opérateur `==` pour comparer deux sommets.

### `Triangle`
- Constructeur pour initialiser un triangle avec trois sommets.
- Surcharge de l'opérateur `==` pour comparer deux triangles.
- Méthodes pour vérifier si un point est dans le triangle, calculer le centre du cercle circonscrit et son rayon.

### `Arete`
- Surcharge de l'opérateur `==` pour comparer deux arêtes.
- Méthodes pour déterminer un sommet de l'arête, calculer la longueur de l'arête, vérifier si deux arêtes se croisent et pour le tri des arêtes par taille.

### `Segment`
- Constructeur pour initialiser un segment avec deux sommets.
- Méthodes pour calculer la longueur du segment, comparer deux segments, vérifier si deux segments se croisent et pour le produit scalaire.

### `Domaine`
- Surcharge de l'opérateur `==` pour comparer deux domaines.
- Cette surcharge compare les triangles et les arêtes de deux domaines ainsi que leur dimension.

## Méthode Frontale et ses dérivées

### `Front`
- Constructeur prenant un tableau de pointeurs vers des segments et un vecteur de sommets pour initialiser un front.
- Constructeur prenant un tableau de pointeurs vers des segments et une taille pour initialiser un front.
- Méthodes pour compter le nombre de segments dans le front, ajouter un segment au front, supprimer un segment du front, ajouter un point au front, supprimer un point du front, vérifier si un point est à l'intérieur du front, mettre à jour le front à partir d'un segment initial et supprimer les segments redondants et les points extérieurs au front.
- Méthode `print()`: Affiche les segments du front avec leurs coordonnées.
- Méthode `genererTriangle()`: Génère des triangles à partir des segments du front. Cette méthode suit l'algorithme de Bowyer-Watson pour construire les triangles de Delaunay.
  - Elle commence par la création d'un super triangle englobant tous les points du front.
  - Ensuite, elle détermine les points intérieurs au super triangle et génère les triangles en conséquence.
  - Elle gère également le cas où aucun point intérieur n'est trouvé en ajoutant le super triangle au front.
  - Elle met à jour le front après chaque ajout de triangle pour maintenir sa structure.
  - Elle supprime également les triangles dont les sommets ne sont pas tous à l'intérieur du front.
  - Elle renvoie les triangles générés.

### `Triangulation`
- Méthode `exportMATLAB(const string& nomFichier) const`: Exporte les sommets et les triangles de la triangulation dans un fichier texte lisible sur MATLAB. Chaque sommet est représenté par sa coordonnée (x, y), et chaque triangle est représenté par les indices de ses sommets.
- Méthode `fusionnerMaillages(const Triangulation& autre)`: Fusionne deux triangulations en fusionnant leurs sommets, triangles et domaines tout en évitant les doublons.
- Méthode `rotation(double angle)`: Effectue une rotation de tous les points de la triangulation autour d'un point central.
- Méthode `translation(double deltaX, double deltaY)`: Effectue une translation de tous les points de la triangulation selon les valeurs spécifiées en delta sur les axes x et y.
- Méthode `homothetie(double facteur)`: Effectue une homothétie de tous les points de la triangulation par rapport à un point central avec un facteur d'échelle donné.
- Méthode `symetriePoint(double pointX, double pointY)`: Effectue une symétrie de tous les points de la triangulation par rapport à un point spécifié.
- Méthode `symetrieAxeOblique(double x0, double y0, double angle)`: Effectue une symétrie de tous les points de la triangulation par rapport à un axe oblique spécifié par un point et un angle.

### `MaillageRectangle`
- Méthode `genererMaillageRectangle(double largeur, double hauteur, int nx, int ny)`: Génère un maillage rectangulaire régulier en divisant un rectangle de dimensions données en nx * ny rectangles élémentaires, avec une coupe aléatoire pour chaque rectangle.
- Méthode `genererMaillageRectangle(double largeur, double hauteur, const vector<double>& abscisses, const vector<double>& ordonnees)`: Génère un maillage rectangulaire non régulier en utilisant des listes d'abscisses et d'ordonnées spécifiées.

### `MaillageSecteurAngulaire`
- Constructeur `MaillageSecteurAngulaire(double rayon, double angle, int N)`: Initialise un maillage secteur angulaire avec un rayon, un angle et un nombre de points.
- Méthode `generersecteurangleaigu()`: Génère un maillage secteur angulaire pour un angle aigu.
- Méthode `genererSecteurAngulaire()`: Génère un maillage secteur angulaire pour un angle quelconque.

### `MaillageFront`
- Constructeur `MaillageFront(Front* AFront)`: Initialise un maillage à partir d'un front.
- Constructeur `MaillageFront(char modele, float Hpas)`: Initialise un maillage avec un modèle et un pas spécifiés.
- Méthode `MethodeFrontal()`: Effectue la méthode frontal pour générer le maillage à partir du front.

### Sauvegarde
- Méthode `save()` dans `Triangulation`: Sauvegarde l'état actuel du maillage dans un fichier texte `historique_M.txt`.
- Méthode `save()` dans `Front`: Sauvegarde l'état actuel du front dans un fichier texte `historique_F.txt`.

# Explication du script MATLAB visio_evolution.m

## Objectif
Le script `visio_evolution.m` charge les données d'évolution d'un maillage et d'un front à partir de fichiers texte, puis les visualise à chaque étape.

## Chargement des données
Le script commence par charger les données à partir des fichiers `historique_M.txt` et `historique_F.txt` qui contiennent respectivement l'évolution du maillage et du front. Veuillez modifier ces valeurs si vous souhaiter changer les fichiers textes résultats.

## Parcours des données du maillage
Le script parcourt les données du maillage en traitant chaque étape séparément. Pour chaque étape :
- Les sommets sont stockés dans la matrice `sommets`.
- Les triangles sont stockés dans la matrice `triangles`.
- À la fin de chaque étape, le maillage est visualisé en utilisant les fonctions `triplot` et `scatter`.

## Parcours des données du front
Après avoir visualisé le maillage, le script parcourt les données du front pour chaque étape. Il ajoute les sommets correspondants à la matrice `sommets` et visualise le front en utilisant la fonction `scatter`. Une pause d'une seconde est effectuée entre chaque étape.

## Conclusion
Ce script est utile pour visualiser l'évolution d'un maillage et d'un front à chaque étape, ce qui peut faciliter l'analyse et le débogage des algorithmes associés.

# Explication du script MATLAB visualisationmaillage.m

## Objectif
Le script `visualisationmaillage.m` charge les données d'un maillage à partir d'un fichier texte, que vous devrez modifier avec vos chemin, puis visualise ces données.

## Chargement des données
Le script commence par charger les données du maillage à partir du fichier texte spécifié (`maillage_regulier.txt`, `maillage_nonregulier.txt` ou `maillage_secteurangulaire.txt`). Les données sont extraites pour obtenir les sommets et les triangles.

## Visualisation du maillage
Une fois les données chargées, le script visualise le maillage en utilisant les fonctions `triplot` et `scatter`. 
- La fonction `triplot` trace les triangles du maillage en utilisant les coordonnées des sommets.
- La fonction `scatter` affiche les sommets en rouge.

## Personnalisation de la visualisation
Le script permet également de personnaliser la visualisation en ajoutant des étiquettes d'axe, en ajustant les limites des axes, en ajoutant une grille, etc. Ces options peuvent être activées ou désactivées en fonction des besoins.

## Conclusion
Ce script est utile pour charger et visualiser rapidement des données de maillage à partir d'un fichier texte. Il offre également des options de personnalisation pour ajuster la présentation de la visualisation selon les préférences de l'utilisateur.

# ENGLISH VERSION
Please do keep in mind that it was computer-translated and could contain mistakes
# Execution:
To execute the code, please use the 'make' command, and to remove the generated files, use 'make clean'.
# Mesh - Header:
This file contains classes and methods for manipulating and generating meshes in C++.
## Elementary Classes
- `Sommet` (Vertex): Represents a point in space defined by its coordinates `x` and `y`.
- `Triangle`: Represents a triangle defined by three vertices.
- `Arete` (Edge): Represents an edge of a triangle.
- `Segment`: Represents a line segment between two vertices.
- `Domaine` (Domain): Represents a domain defined by triangles and edges.
## Frontal Method and its derivatives
- `Front`: Manages a frontal data structure for triangle generation.
## Triangulation Class and its derivatives
- `Triangulation`: Manages a collection of vertices, triangles, edges, and domains.
- `MaillageRectangle` (RectangularMesh): Derived from `Triangulation`, allows generating a regular or irregular rectangular mesh.
- `MaillageSecteurAngulaire` (AngularSectorMesh): Derived from `Triangulation`, allows generating a mesh for a given angular sector.
- `MaillageFront` (FrontMesh): Derived from `Triangulation`, uses a frontal method to generate a mesh.
# Mesh - Implementation:
This file (`Maillage.cpp`) contains the implementation of classes and methods for manipulating and generating triangular meshes in C++.
## Elementary Classes
### `Sommet` (Vertex)
- Overload of the `==` operator to compare two vertices.
### `Triangle`
- Constructor to initialize a triangle with three vertices.
- Overload of the `==` operator to compare two triangles.
- Methods to check if a point is inside the triangle, calculate the center of the circumcircle and its radius.
### `Arete` (Edge)
- Overload of the `==` operator to compare two edges.
- Methods to determine a vertex of the edge, calculate the length of the edge, check if two edges intersect, and for sorting edges by size.
### `Segment`
- Constructor to initialize a segment with two vertices.
- Methods to calculate the length of the segment, compare two segments, check if two segments intersect, and for the dot product.
### `Domaine` (Domain)
- Overload of the `==` operator to compare two domains.
- This overload compares the triangles and edges of two domains as well as their dimension.
## Frontal Method and its derivatives
### `Front`
- Constructor taking an array of pointers to segments and a vector of vertices to initialize a front.
- Constructor taking an array of pointers to segments and a size to initialize a front.
- Methods to count the number of segments in the front, add a segment to the front, remove a segment from the front, add a vertex to the front, remove a vertex from the front, check if a vertex is inside the front, update the front from an initial segment, and remove redundant segments and vertices outside the front.
- Method `print()`: Displays the segments of the front with their coordinates.
- Method `genererTriangle()`: Generates triangles from the segments of the front. This method follows the Bowyer-Watson algorithm to construct Delaunay triangles.
  - It starts by creating a super triangle that encompasses all the points of the front.
  - It returns the generated triangles.
### `Triangulation`
- Method `exportMATLAB(const string& fileName) const`: Exports the vertices and triangles of the triangulation to a MATLAB-readable text file. Each vertex is represented by its coordinates (x, y), and each triangle is represented by the indices of its vertices.
- Method `fusionnerMaillages(const Triangulation& other)`: Merges two triangulations by merging their vertices, triangles, and domains while avoiding duplicates.
- Method `rotation(double angle)`: Performs a rotation of all the points in the triangulation around a central point.
- Method `translation(double deltaX, double deltaY)`: Performs a translation of all the points in the triangulation by the specified delta values on the x and y axes.
- Method `homothetie(double factor)`: Performs a scaling of all the points in the triangulation with respect to a central point using a given scaling factor.
- Method `symetriePoint(double pointX, double pointY)`: Performs a reflection of all the points in the triangulation with respect to a specified point.
- Method `symetrieAxeOblique(double x0, double y0, double angle)`: Performs a reflection of all the points in the triangulation with respect to an oblique axis specified by a point and an angle.
### `MaillageRectangle` (RectangularMesh)
- Method `genererMaillageRectangle(double width, double height, int nx, int ny)`: Generates a regular rectangular mesh by dividing a rectangle of given dimensions into nx * ny elementary rectangles, with a random cut for each rectangle.
- Method `genererMaillageRectangle(double width, double height, const vector<double>& abscissas, const vector<double>& ordinates)`: Generates a non-regular rectangular mesh using specified lists of abscissas and ordinates.
### `MaillageSecteurAngulaire` (AngularSectorMesh)
- Constructor `MaillageSecteurAngulaire(double radius, double angle, int N)`: Initializes an angular sector mesh with a radius, an angle, and a number of points.
- Method `generersecteurangleaigu()`: Generates an angular sector mesh for an acute angle.
- Method `genererSecteurAngulaire()`: Generates an angular sector mesh for an arbitrary angle.
### `MaillageFront` (FrontMesh)
- Constructor `MaillageFront(Front* aFront)`: Initializes a mesh from a front.
- Constructor `MaillageFront(char model, float step)`: Initializes a mesh with a specified model and step.
- Method `MethodeFrontal()`: Performs the frontal method to generate the mesh from the front.
### Saving
- Method `save()` in `Triangulation`: Saves the current state of the mesh to a text file `historique_M.txt`.
- Method `save()` in `Front`: Saves the current state of the front to a text file `historique_F.txt`.
# Explanation of the MATLAB script visio_evolution.m
## Objective
The script `visio_evolution.m` loads the evolution data of a mesh and a front from text files, and then visualizes them at each step.
## Loading the data
The script starts by loading the data from the files `historique_M.txt` and `historique_F.txt`, which respectively contain the evolution of the mesh and the front. Please modify these values if you want to change the result text files.
## Processing the mesh data
The script processes the mesh data by treating each step separately. For each step:
- The vertices are stored in the matrix `sommets` (vertices).
- The triangles are stored in the matrix `triangles`.
- At the end of each step, the mesh is visualized using the `triplot` and `scatter` functions.
## Processing the front data
After visualizing the mesh, the script processes the front data for each step. It adds the corresponding vertices to the `sommets` matrix and visualizes the front using the `scatter` function. A one-second pause is made between each step.
## Conclusion
This script is useful for visualizing the evolution of a mesh and a front at each step, which can facilitate the analysis and debugging of associated algorithms.
# Explanation of the MATLAB script visualisationmaillage.m
## Objective
The script `visualisationmaillage.m` loads mesh data from a text file, which you will need to modify with your path, and then visualizes this data.
## Loading the data
The script starts by loading the mesh data from the specified text file (`maillage_regulier.txt`, `maillage_nonregulier.txt`, or `maillage_secteurangulaire.txt`). The data is extracted to obtain the vertices and triangles.
## Visualizing the mesh
Once the data is loaded, the script visualizes the mesh using the `triplot` and `scatter` functions.
- The `triplot` function plots the triangles of the mesh using the coordinates of the vertices.
- The `scatter` function displays the vertices in red.
## Customizing the visualization
The script also allows customizing the visualization by adding axis labels, adjusting the axis limits, adding a grid, etc. These options can be enabled or disabled depending on the needs.
## Conclusion
This script is useful for quickly loading and visualizing mesh data from a text file. It also offers customization options to adjust the presentation of the visualization according to user preferences.

