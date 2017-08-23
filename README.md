# pbAEG
Problème d'accessibilité et d'exploitabilité dans un graph

./compile.sh
Compile et créé l'executable "ter"

opt :
-it 123 ; nombre d'itération de l'algorithme fixé à 123 (10 par défaut).
-seed 456 ; seed du générateur d'aléatoire fixée à 456 (time par défaut).
-exp 1 ; facteur de la contrainte "exploitabilité" fixé à 1 (1000 par défaut) (float).
-conn 1 ; facteur de la contrainte "connexité" fixé à 1 (2000 par défaut) (float).
-cost 2 ; facteur de l'objectif "cout" fixé à 2 (1 par défaut) (float).
-acc 2 ; facteur de l'objectif "accessibilité" fixé à 2 (1 par défaut) (float).
-ls ; Algorithme de recherche locale.
-recuit 30 0.99 ; Algorithme de recherche locale par recuit simulé, température de base fixée à 30 et facteur d'évolution fixé à 0.99.
-comp ; Algorithme complet (O(2^n)).
-LV ; Algorithme de Las Vegas.


./create.sh
Créé un graph

opt :
-o ; change le chemin de création (graph.txt par défaut).
-g ; créé un graph grille.
-gr ; créé un graphe grille avec des poids aléatoires.
-n 5 ; nombre de noeud dans le graph fixé à 5, Pour -g et -gr ce sera 5x5 (100 par défaut).
-w 100 ; poid maximum fixé à 100 (1000 par défaut).
-d 70; densité du graph fixée à 70% (50% par défaut).
