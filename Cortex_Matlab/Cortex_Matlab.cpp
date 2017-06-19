#include <stdio.h>

#include "Cortex_Matlab.h"
#include "../dllCortex/Cortex.h"

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
            sprintf_s(errorMessage, 255, "Ok c'est bon on a initialisé une connexion");
                 }
        else {
            isConnected = false;
            //Logger::WriteMessage("Aie y a un problème");
            sprintf_s(errorMessage, 255, "Aie y a un problème, on aurrait dû avoir une connexion");
            //Assert::Fail(L"on ne devrait sortir par là - la connexion ne c'est pas faite");
            //TODO throw error with errorCode
        }
    }
    else {
        //Logger::WriteMessage("Déjà connexté");
        sprintf_s(errorMessage, 255, "Déjà connexté");
    }
    return isConnected;
}

int exitCortexConnexion() {
    int retval = Cortex_Exit();
    return retval;
}


//met à jour les coordonnées X, Y, Z de l'objet [objectIndex] ainsi que les angles azimut (angle horizontal) et elevation
int getObjectPositionCortex(int objectIndex, double* X, double* Y, double* Z, double* azimut, double* elevation) {
    int retval = 1;
    sBodyData bodyData;
    float *pos;
    if (Cortex_IsClientCommunicationEnabled()) {
        // A chaque appel on demande la dernière position connue
        sFrameOfData *frame;
        frame = Cortex_GetCurrentFrame();
        if (objectIndex < frame->nBodies) {
            bodyData = frame->BodyData[objectIndex];
            pos = (float*)bodyData.Markers;
            (*X) = pos[0];
            (*Y) = pos[1];
            (*Z) = pos[2];
            (*azimut) = 0.0;
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
