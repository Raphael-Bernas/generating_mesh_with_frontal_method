# SIM202

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

