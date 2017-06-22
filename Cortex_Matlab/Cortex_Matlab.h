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
    //initialise une connexion sur le serveur Cortex d�finit par son adresse IP
     __declspec(dllexport) int getCortexConnexion(char * ipCortexServerr, char * errorMessage);

    //met � jour les coordonn�es X, Y, Z de l'objet [objectIndex] ainsi que les angles azimut (angle horizontal) et elevation
     __declspec(dllexport) int getObjectPositionCortex(int objectIndex, double* X, double* Y, double* Z, double* azimut, double* elevation);

    //Annule la connexion Cortex pour lib�rer proprement la m�moire et permettre d'autres connexions ult�rieures
     __declspec(dllexport) int exitCortexConnexion();
#ifdef __cplusplus
 }
#endif
