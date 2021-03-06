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
 */
int getCortexConnexion(char * ipCortexServer, char * errorMessage) {
    int isConnected = Cortex_IsClientCommunicationEnabled();
    //char *host_matlab = "192.168.1.103";  //TODO trouver l'IP de la machine courante
    char host_matlab[20] = "";
    int retval;
    if (!isConnected) {
        Cortex_SetClientCommunicationEnabled(1);
        //retval = Cortex_ConfigurePortNumbers(0, -1, -1);
        findLocalIP(ipCortexServer, host_matlab);
        retval = Cortex_Initialize(host_matlab, ipCortexServer);
        if (retval == RC_Okay) {
            isConnected = Cortex_IsClientCommunicationEnabled();
            //Logger::WriteMessage("Ok c'est bon on a initialis� une connexion");
//            sprintf_s(errorMessage, 255, "Ok c'est bon on a initialis� une connexion");
                 }
        else {
            isConnected = false;
            //Logger::WriteMessage("Aie y a un probl�me");
 //           sprintf_s(errorMessage, 255, "Aie y a un probl�me, on aurrait d� avoir une connexion");
            //Assert::Fail(L"on ne devrait sortir par l� - la connexion ne c'est pas faite");
            //TODO throw error with errorCode
        }
    }
    else {
        //Logger::WriteMessage("D�j� connext�");
 //       sprintf_s(errorMessage, 255, "D�j� connext�");
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
int getObjectPositionCortex(int objectIndex, float* X, float* Y, float* Z, double* azimut, double* elevation) {
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
