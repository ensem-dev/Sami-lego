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

int exitCortexConnexion() {
    int retval = Cortex_Exit();
    return retval;
}


//met � jour les coordonn�es X, Y, Z de l'objet [objectIndex] ainsi que les angles azimut (angle horizontal) et elevation
int getObjectPositionCortex(int objectIndex, double* X, double* Y, double* Z, double* azimut, double* elevation) {
    int retval = 1;
    sBodyData bodyData;
    float *pos;
    if (Cortex_IsClientCommunicationEnabled()) {
        // A chaque appel on demande la derni�re position connue
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
            //TODO g�n�rer une exception car il n'y a pas l'objet demand� n'est pas dans la liste des objets suivis
        }
    }
    else {
        //TODO g�n�rer une exception pour demander d'initialiser une connexion au pr�alable
    }
    return retval;
}
