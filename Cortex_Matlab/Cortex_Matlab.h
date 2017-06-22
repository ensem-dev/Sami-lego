#pragma once
struct position
{
    char* objectName;
    float X;
    float Y;
    float Z;
};
/*struct positions
{
    int nbObjects;
    int frameNumber;
    float cameraDelay;
    position listPositions[99];
};*/


//positions getPositions();

//#ifdef _WINDOWS
// #define DLL __declspec(dllexport)
//#else
// #define DLL
//#endif

#ifdef __cplusplus 
 extern "C" {
#endif 
    //initialise une connexion sur le serveur Cortex définit par son adresse IP
     __declspec(dllexport) int getCortexConnexion(char * ipCortexServerr, char * errorMessage);

    //met à jour les coordonnées X, Y, Z de l'objet [objectIndex] ainsi que les angles azimut (angle horizontal) et elevation
     __declspec(dllexport) int getObjectPositionCortex(int objectIndex, double* X, double* Y, double* Z, double* azimut, double* elevation);

    //Annule la connexion Cortex pour libérer proprement la mémoire et permettre d'autres connexions ultérieures
     __declspec(dllexport) int exitCortexConnexion();
#ifdef __cplusplus
 }
#endif
