#include <stdio.h>

#include "Cortex_Matlab.h"
#include "../dllCortex/Cortex.h"

//Includes pour la recup�ration des IPs afin de trouver l'IP du bon r�seau
#include <winsock2.h>  //DWORD....
#include <WS2tcpip.h>    //inet_ntop qui remplace inet_ntoa
#include <IPHlpApi.h>   //MIB_IPADDRTABLE
#pragma comment(lib, "Ws2_32.lib")   // Ajout� pour inet_ntop()
#pragma comment(lib, "IPHLPAPI.lib")  // GetIpAddrTable

#define _USE_MATH_DEFINES // pour avoir acc�s � PI
#include <math.h>  

/**
* Recherche l'IP local sur le m�me r�seau que l'adresse distante
*/
bool findLocalIP(char *remoteHostIP, char LocalIPBuff[20]) {
    bool retOk = false;
    ULONG remoreIp = 0;
    MIB_IPADDRTABLE *pIPAddrTable;
    long dwRetVal = 0;
    DWORD dwSize = 0;
    int cptLocalIP = 0;
    ULONG localSubnet;
    ULONG remoteSubnet;

    //Obtient la remote ip sous forme d'un long pour pouvoir lui applique les masques de sous r�seau des IPs locales 
    inet_pton(AF_INET, remoteHostIP, &remoreIp);

    pIPAddrTable = (MIB_IPADDRTABLE*)malloc(sizeof(MIB_IPADDRTABLE));
    if (pIPAddrTable) {
        dwRetVal = GetIpAddrTable(pIPAddrTable, &dwSize, 0);
        if (ERROR_INSUFFICIENT_BUFFER == dwRetVal) {
            free(pIPAddrTable);
            pIPAddrTable = (MIB_IPADDRTABLE*)malloc(dwSize);
            if (pIPAddrTable) {
                dwRetVal = GetIpAddrTable(pIPAddrTable, &dwSize, 0);
                if (NO_ERROR == dwRetVal) {
                    for (cptLocalIP = 0; cptLocalIP < (int)pIPAddrTable->dwNumEntries; cptLocalIP++) {
                        localSubnet = pIPAddrTable->table[cptLocalIP].dwAddr & pIPAddrTable->table[cptLocalIP].dwMask;
                        remoteSubnet = remoreIp & pIPAddrTable->table[cptLocalIP].dwMask;
                        if (localSubnet == remoteSubnet) {
                            inet_ntop(AF_INET, &pIPAddrTable->table[cptLocalIP].dwAddr, LocalIPBuff, 19);
                            retOk = true;
                            break;
                        }
                    }
                }
                else {
                    //TODO il faut traiter les erreur de GetIpAddrTable
                }
                free(pIPAddrTable);
            }
            else {
                //TODO il y a eu un PB avec Malloc
            }
        }
    }
    return retOk;
}

/** 
 * Etabli une connexion avec le Host Cortex qui donne les coordonn�es des objets d�tect�s
 * Return : connexionOK=1 - si retour 0 alors la communication n'a pu �tre �tablie. Il faut lire le message d'erreur errorMessage 
 *
 * https://stackoverflow.com/questions/7445054/passing-pointer-argument-in-matlab-to-a-c-dll-function-foochar
 */
int getCortexConnexion(char * ipCortexServer, char ** errorMessage) {
    int isConnected = Cortex_IsClientCommunicationEnabled();
    char host_matlab[20] = "";
    int retval;

//    sBodyDefs* bodies;
    sFrameOfData *frame;
    sBodyData *bodyData;

    int nbBodies;
    int cptBodies;
    char vOut[255];
    char message[1025] = "Liste des objets suivit :\n";
    if (!isConnected) {
        Cortex_SetClientCommunicationEnabled(1);
        //retval = Cortex_ConfigurePortNumbers(0, -1, -1);
        findLocalIP(ipCortexServer, host_matlab);
        retval = Cortex_Initialize(host_matlab, ipCortexServer);
        if (retval == RC_Okay) {
            isConnected = Cortex_IsClientCommunicationEnabled();
            //bodies = Cortex_GetBodyDefs();  // Il semblerais que GetBodyDefs plante Matlab. En passant par Frame �a semble bien meilleur !
            //if (bodies) {
            //    nbBodies = bodies->nBodyDefs;
            //    for (int i = 0; i < nbBodies; i++) {
                //    sprintf_s(vOut, 255, "objet %d : %s.\n", i, bodies->BodyDefs[i].szName);
                //    strcat_s(message, 1024, vOut);
           //     }
                //Cortex_FreeBodyDefs(bodies);
            frame = Cortex_GetCurrentFrame();
            if (frame) {
                nbBodies = frame->nBodies;
                for (cptBodies = 0; cptBodies < nbBodies; cptBodies++) {
                    bodyData = &(frame->BodyData[cptBodies]);
                    sprintf_s(vOut, 255, "objet %d : %s.\n", cptBodies, bodyData->szName);
                    strcat_s(message, 1024, vOut);
                }
                Cortex_FreeFrame(frame);
                *errorMessage = _strdup(message);
            } else {
                *errorMessage = _strdup("Ok c'est bon on a initialise une connexion, mais l'application cortex n'est pas bien d�marr�e");
            }
        } else {
            isConnected = false;
            *errorMessage = _strdup("Aie y a un probl�me, on aurrait d� avoir une connexion");
            //TODO throw error with errorCode
        }
    } else {
        *errorMessage = _strdup("Deje connexte");
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

//Deprecated - Juste to compatible with old Matlab programs
int getPositionCortex(int objectIndex, float* X, float* Y, float* Z, double* azimut, double* elevation) {
    return getObjectPositionCortexID(objectIndex, X, Y, Z, azimut, elevation);
}

/**
 * met � jour les coordonn�es X, Y, Z de l'objet [objectIndex] ainsi que les angles azimut (angle horizontal) et elevation
 */
int getObjectPositionCortexID(int objectIndex, float* X, float* Y, float* Z, double* azimut, double* elevation) {
    int retval = 1;
    sBodyData *bodyData;
    int mark_centre_gravite = 4;
    int mark_direction = 6;
    sFrameOfData *frame;
    if (Cortex_IsClientCommunicationEnabled()) {
        // A chaque appel on demande la derni�re position connue
        frame = Cortex_GetCurrentFrame();
        if (frame) {
            if (objectIndex < frame->nBodies) {
                bodyData = &(frame->BodyData[objectIndex]);

                *X = bodyData->Markers[mark_centre_gravite][0];
                *Y = bodyData->Markers[mark_centre_gravite][1];
                *Z = bodyData->Markers[mark_centre_gravite][2];

                //calcul de l'orientation du robot dans le plan de la sc�ne (en degres)
                *azimut = atan2(bodyData->Markers[mark_direction][1] - *Y, bodyData->Markers[mark_direction][0] - *X)*180.0 / M_PI;

                (*elevation) = 0.0;
                retval = 0;

             }
            else {
                //TODO g�n�rer une exception car il n'y a pas l'objet demand� n'est pas dans la liste des objets suivis
            }
            Cortex_FreeFrame(frame);
        }
        else {
            //TODO pas normal d'avoir une r�ponse vide
//            *X = 8888888;
//            *Y = 8888888;
//            *Z = 8888888;

            //calcul de l'orientation du robot dans le plan de la sc�ne (en degres)
//            *azimut = 0.0;

//            (*elevation) = 0.0;
            retval = 1;
        }
    }
    else {
        //TODO g�n�rer une exception pour demander d'initialiser une connexion au pr�alable
    }
    return retval;
}

int getObjectPositionCortexByName(char* objectName, float* X, float* Y, float* Z, double* azimut, double* elevation) {
    int retval = 1;
    sBodyData *bodyData;
    int mark_centre_gravite = 4;
    int mark_direction = 6;


    int numBody;
 
    sFrameOfData *frame;
    if (Cortex_IsClientCommunicationEnabled()) {
        // A chaque appel on demande la derni�re position connue
        frame = Cortex_GetCurrentFrame();
        if (frame) {
            for (numBody = 0; numBody < frame->nBodies; numBody++)
            {
                bodyData = &(frame->BodyData[numBody]);
                if (strcmp(bodyData->szName, objectName) == 0)
                {
                    *X = bodyData->Markers[mark_centre_gravite][0];
                    *Y = bodyData->Markers[mark_centre_gravite][1];
                    *Z = bodyData->Markers[mark_centre_gravite][2];

                    //calcul de l'orientation du robot dans le plan de la sc�ne (en degres)
                    *azimut = atan2(bodyData->Markers[mark_direction][1] - *Y, bodyData->Markers[mark_direction][0] - *X)*180.0 / M_PI;

                    (*elevation) = 0.0;
                    retval = 0;
                }
            }
        }
        else {
            //TODO pas normal d'avoir une r�ponse vide
            //            *X = 8888888;
            //            *Y = 8888888;
            //            *Z = 8888888;

            //calcul de l'orientation du robot dans le plan de la sc�ne (en degres)
            //            *azimut = 0.0;

            //            (*elevation) = 0.0;
            retval = 1;
        }
    }
    else {
        //TODO g�n�rer une exception pour demander d'initialiser une connexion au pr�alable
    }
    
    //int objectIndex = 0;
    //retval = getObjectPositionCortexID(objectIndex, X, Y, Z, azimut, elevation);
    return retval;
}

