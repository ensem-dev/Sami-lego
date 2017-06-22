#include <stdio.h>

#include "Cortex_Matlab.h"
#include "../dllCortex/Cortex.h"

#define _USE_MATH_DEFINES // for C  
#include <math.h>  

/** 
 * Etabli une connexion avec le Host Cortex qui donne les coordonn�es des objets d�tect�s
 * Return : connexionOK=1 - si retour 0 alors la communication n'a pu �tre �tablie. Il faut lire le message d'erreur errorMessage 
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
            //Logger::WriteMessage("Ok c'est bon on a initialis� une connexion");
            sprintf_s(errorMessage, 255, "Ok c'est bon on a initialis� une connexion");
                 }
        else {
            isConnected = false;
            //Logger::WriteMessage("Aie y a un probl�me");
            sprintf_s(errorMessage, 255, "Aie y a un probl�me, on aurrait d� avoir une connexion");
            //Assert::Fail(L"on ne devrait sortir par l� - la connexion ne c'est pas faite");
            //TODO throw error with errorCode
        }
    }
    else {
        //Logger::WriteMessage("D�j� connext�");
        sprintf_s(errorMessage, 255, "D�j� connext�");
    }
    return isConnected;
}

/**
 * D�connexion du host Cortex.
 * Pas s�r que cela fonctionne correctement. Le nettoyage de la connexion ne semble pas �tre bien fait
 */
int exitCortexConnexion() {
    int retval = Cortex_Exit();
    return retval;
}


/**
 * met � jour les coordonn�es X, Y, Z de l'objet [objectIndex] ainsi que les angles azimut (angle horizontal) et elevation
 */
int getObjectPositionCortex(int objectIndex, double* X, double* Y, double* Z, double* azimut, double* elevation) {
    int retval = 1;
    sBodyData bodyData;
    float *coordonnees_centre_essieu;

    float *coordonnees_avt;
    float *x_avt = new float;
    float *y_avt = new float;
    float *z_avt = new float;

    if (Cortex_IsClientCommunicationEnabled()) {
        // A chaque appel on demande la derni�re position connue
        sFrameOfData *frame;
        frame = Cortex_GetCurrentFrame();
        if (objectIndex < frame->nBodies) {
            bodyData = frame->BodyData[objectIndex];

            coordonnees_centre_essieu = bodyData.Markers[4];
            *X = coordonnees_centre_essieu[0];
            *Y = coordonnees_centre_essieu[1];
            *Z = coordonnees_centre_essieu[2];

            //r�cup�re les data du point virtuel � l'avant (celui qui d�finit le segment orientation)
            coordonnees_avt = bodyData.Markers[6];
            *x_avt = coordonnees_avt[0];
            *y_avt = coordonnees_avt[1];
            *z_avt = coordonnees_avt[2];
            //calcul de l'orientation du robot dans le plan de la sc�ne (en degres)
            *azimut = atan2((double)(coordonnees_avt[1] - coordonnees_centre_essieu[1]), (double)(coordonnees_avt[0] - coordonnees_centre_essieu[0]))*180.0 / M_PI;

            (*elevation) = 0.0;
            retval = 0;
        }
        else {
            //TODO g�n�rer une exception car il n'y a pas l'objet demand� n'est pas dans la liste des objets suivis
        }
    }
    else {
        //TODO g�n�rer une exception pour demander d'initialiser une connexion au pr�alable
    }
    return retval;
}
