#pragma once


#ifdef __cplusplus 
 extern "C" {
#endif 
     __declspec(dllexport) bool findLocalIP(char *remoteHostIP, char LocalIPBuff[20]);
     
     //initialise une connexion sur le serveur Cortex définit par son adresse IP
     __declspec(dllexport) int getCortexConnexion(char * ipCortexServerr, char ** errorMessage);

     //met à jour les coordonnées X, Y, Z de l'objet [objectIndex] ainsi que les angles azimut (angle horizontal) et elevation
     //Deprecated - use insted the getObjectPositionCortexID function.
     __declspec(dllexport) int getPositionCortex(int objectIndex, float* X, float* Y, float* Z, double* azimut, double* elevation);
     //met à jour les coordonnées X, Y, Z de l'objet [objectIndex] ainsi que les angles azimut (angle horizontal) et elevation
     __declspec(dllexport) int getObjectPositionCortexID(int objectIndex, float* X, float* Y, float* Z, double* azimut, double* elevation);
     //met à jour les coordonnées X, Y, Z de l'objet nommé ainsi que les angles azimut (angle horizontal) et elevation
     __declspec(dllexport) int getObjectPositionCortexByName(char *objectName, float* X, float* Y, float* Z, double* azimut, double* elevation);

    //Annule la connexion Cortex pour libérer proprement la mémoire et permettre d'autres connexions ultérieures
     __declspec(dllexport) int exitCortexConnexion();
#ifdef __cplusplus
 }
#endif
