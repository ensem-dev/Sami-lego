Un programme MatLab doit
  - charger la librairie
  - initialiser une connexion sur le serveur Cortex
  - éventuellement lister les objets suivis afin de connaître leur index
  - lire des positions dans une boucle 
  - libérer la connexion (CortexExit)
  - libérer la librairie

Dans Matlab onglet HOME définir le SetPath ou doit se trouver les dll et lib Cortex_Matlab ainsi que la dll Cortex_SDK.
Le post build de la solution CortexMatlb_Natif copie les lib et dll générées par la compilation dans le dossier x64/Debug.
Et copie le fichier Cortex_Matlab.h.
on met donc se dossier dans Matlab. Il y ajoute aussi les fichier de code_matlab pour les fonctions de communication de base.

