#include <stdio.h>

#include "Cortex_Matlab.h"
#include "../dllCortex/Cortex.h"

#define _USE_MATH_DEFINES // for C  
#include <math.h>  

/** 
 * Etabli une connexion avec le Host Cortex qui donne les coordonnées des objets détectés
 * Return : connexionOK=1 - si retour 0 alors la communication n'a pu être établie. Il faut lire le message d'erreur errorMessage 
 */
int getCortexConnexion(char * ipCortexServer, char * errorMessage) {
    int isConnected = Cortex_IsClientCommunicationEnabled();
    char *host_matlab = "192.168.1.103";  //TODO trouver l'IP de la machine courante
    int retval;
    if (!isConnected) {
        Cortex_SetClientCommunicationEnabled(1);
        //retval = Cortex_ConfigurePortNumbers(0, -1, -1);
        retval = Cortex_Initialize(host_matlab, ipCortexServer);
        if (retval == RC_Okay) {
            isConnected = Cortex_IsClientCommunicationEnabled();
            //Logger::WriteMessage("Ok c'est bon on a initialisé une connexion");
//            sprintf_s(errorMessage, 255, "Ok c'est bon on a initialisé une connexion");
                 }
        else {
            isConnected = false;
            //Logger::WriteMessage("Aie y a un problème");
 //           sprintf_s(errorMessage, 255, "Aie y a un problème, on aurrait dû avoir une connexion");
            //Assert::Fail(L"on ne devrait sortir par là - la connexion ne c'est pas faite");
            //TODO throw error with errorCode
        }
    }
    else {
        //Logger::WriteMessage("Déjà connexté");
 //       sprintf_s(errorMessage, 255, "Déjà connexté");
    }
    return isConnected;
}

/**
 * Déconnexion du host Cortex.
 * Pas sûr que cela fonctionne correctement. Le nettoyage de la connexion ne semble pas être bien fait
 */
int exitCortexConnexion() {
    int retval = Cortex_Exit();
    return retval;
}


/**
 * met à jour les coordonnées X, Y, Z de l'objet [objectIndex] ainsi que les angles azimut (angle horizontal) et elevation
 */
int getObjectPositionCortex(int objectIndex, float* X, float* Y, float* Z, double* azimut, double* elevation) {
    int retval = 1;
    sBodyData bodyData;
    int mark_centre_gravite = 4;
    int mark_direction = 6;

    if (Cortex_IsClientCommunicationEnabled()) {
        // A chaque appel on demande la dernière position connue
        sFrameOfData *frame;
        frame = Cortex_GetCurrentFrame();
        if (objectIndex < frame->nBodies) {
            bodyData = frame->BodyData[objectIndex];

            *X = bodyData.Markers[mark_centre_gravite][0];
            *Y = bodyData.Markers[mark_centre_gravite][1];
            *Z = bodyData.Markers[mark_centre_gravite][2];

            //calcul de l'orientation du robot dans le plan de la scène (en degres)
            *azimut = atan2(bodyData.Markers[mark_direction][1] - *Y, bodyData.Markers[mark_direction][0] - *X)*180.0 / M_PI;

            (*elevation) = 0.0;
            retval = 0;
        }
        else {
            //TODO générer une exception car il n'y a pas l'objet demandé n'est pas dans la liste des objets suivis
        }
    }
    else {
        //TODO générer une exception pour demander d'initialiser une connexion au préalable
    }
    return retval;
}
