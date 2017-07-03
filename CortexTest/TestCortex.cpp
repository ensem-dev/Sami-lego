#include "stdafx.h"
 
/** pour pouvoir mettre PI dans le code **/
#define _USE_MATH_DEFINES // for C++  
#include <cmath>  

#define _USE_MATH_DEFINES // for C  
#include <math.h>  
/*****************************************/

#include "../dllCortex/Cortex.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CortexTest
{
    TEST_CLASS(TestCortex)
    {
    public:

        TEST_METHOD(TestSetLevelVerbosity) {
            int retval = 4;
            retval = Cortex_SetVerbosityLevel(VL_Debug);
            int expected = RC_Okay;
            Assert::AreEqual(expected, retval);
            int verbos = Cortex_GetVerbosityLevel();
            Assert::AreEqual((int)VL_Debug, verbos);
        }

        TEST_METHOD(TestGetSdkVersion) {
            char buff[255];
            Logger::WriteMessage("coucou");
            int retval = 4;
            unsigned char version[4];
            retval = Cortex_GetSdkVersion(version);
            int expected = RC_Okay;
            Assert::AreEqual(expected, retval);
            //Assert::AreEqual(4, version[0], "Major version");
            //Assert::AreEqual(Convert::ToByte(1), version[1], "Minor Version");
            //Assert::AreEqual(Convert::ToByte(4), Convert::ToByte(version[2]), "Revision");
            //Assert::AreEqual(Convert::ToInt32(0), Convert::ToInt32(version[3]), "sub Revision");
            char *expectVersion = "4.1.10.0";
            Logger::WriteMessage(expectVersion);
            sprintf_s(buff, 10, "%d.%d.%d.%d", version[0], version[1], version[2], version[3]);
            Logger::WriteMessage(buff);
            Assert::AreEqual(expectVersion, buff,"pas la bonne version");
            Logger::WriteMessage("fin du test");
        }

        TEST_METHOD(TestIsClientCommunicationEnabled) {
            int retval = 4;
            retval = Cortex_IsClientCommunicationEnabled();
            int expected = 0;
            Assert::AreEqual(expected, retval);
        }

        TEST_METHOD(TestConfigurePortNumbers) {
            int retval = 4;
            retval = Cortex_ConfigurePortNumbers(0, -1, -1);
            int expected = RC_Okay;
            Assert::AreEqual((int)RC_Okay, retval);
        }

        TEST_METHOD(TestGetBodyDefs) {
            int isConnexted;
            sBodyDefs* bodies;
            int nbBodies;
            char *host_matlab = "192.168.1.103";
            char *host_cortex = "192.168.1.109";
            int retval = -1;
            char vOut[255];
            Logger::WriteMessage("on va tenter une connexion");
            if (!Cortex_IsClientCommunicationEnabled()) {
                Cortex_SetClientCommunicationEnabled(1);
                retval = Cortex_ConfigurePortNumbers(0, -1, -1);
                Assert::AreEqual((int)RC_Okay, retval);
                retval = Cortex_Initialize(host_matlab, host_cortex);
                Assert::AreEqual((int)RC_Okay, retval);
            }
            Logger::WriteMessage("on va tenter une requete");
            isConnexted = Cortex_IsClientCommunicationEnabled();
            Assert::AreEqual(1, isConnexted);
            //Controle l'existance d'une connexion active avant interrogation du système
            if (isConnexted) {
                bodies = Cortex_GetBodyDefs();
                nbBodies = bodies->nBodyDefs;
                sprintf_s(vOut, 255, "Nb objets détectés : %d.", nbBodies);
                Logger::WriteMessage(vOut);
                Assert::AreEqual(3, nbBodies);
                for (int i = 0; i < nbBodies; i++) {
                    sprintf_s(vOut, 255, "objet %d : %s.", i, bodies->BodyDefs[i].szName);
                    Logger::WriteMessage(vOut);
                }
                //retval = Cortex_Exit();
                //Assert::AreEqual((int)RC_Okay, retval);
                //Cortex_SetClientCommunicationEnabled(0);
            }
        }

        int getCortexConnexion(char * ipCortexServer) {
            int isConnected = Cortex_IsClientCommunicationEnabled();
            char *host_matlab = "192.168.1.103";  //TODO trouver l'IP de la machine courante
            int retval;
            if (!isConnected) {
                Cortex_SetClientCommunicationEnabled(1);
                //retval = Cortex_ConfigurePortNumbers(0, -1, -1);
                retval = Cortex_Initialize(host_matlab, ipCortexServer);
                if (retval == RC_Okay) {
                    isConnected = Cortex_IsClientCommunicationEnabled();
                    Logger::WriteMessage("Ok c'est bon on a initialisé une connexion");
                }
                else {
                    isConnected = false;
                    Logger::WriteMessage("Aie y a un problème");
                    Assert::Fail(L"on ne devrait sortir par là - la connexion ne c'est pas faite");
                    //TODO throw error with errorCode
                }
            }
            else {
                Logger::WriteMessage("Déjà connexté");
            }
            return isConnected;
        }

        TEST_METHOD(TesGetCurrentFrame) {
            char *ipCortexServer = "192.168.1.109";
            char *errorMessage = 0;
            int numBodies;
            int nbObjTracked;
            sBodyData bodyData;
            
            float *coordonnees_centre_essieu;
            float *x = new float;
            float *y = new float;
            float *z = new float;

            float *coordonnees_avt;
            float *x_avt = new float;
            float *y_avt = new float;
            float *z_avt = new float;

            double orient;

            int cptCycle = 0;
            int maxCycle = 10;

            int mark;
            int coord;

            char buff[255];
            int isConnected = Cortex_IsClientCommunicationEnabled();
            char *host_matlab = "192.168.1.103";  //TODO trouver l'IP de la machine courante
            //int retval;
            if (getCortexConnexion(ipCortexServer)) {
                Logger::WriteMessage(errorMessage);
                while (cptCycle<maxCycle) {
                    cptCycle++;
                    sFrameOfData*  positions;
                    positions = Cortex_GetCurrentFrame();
                    sprintf_s(buff, 255, "numero du frame : %d.", positions->iFrame);
                    Logger::WriteMessage((const char *)buff);
                    sprintf_s(buff, 255, "délais depuis l'enregistrment caméra : %f.", positions->fDelay);
                    Logger::WriteMessage((const char *)buff);
                    nbObjTracked = positions->nBodies;
                    sprintf_s(buff, 255, "Nombre d'objets suivis : %d.", nbObjTracked);
                    Logger::WriteMessage((const char *)buff);
                    sprintf_s(buff, 255, "Nombre de marqueurs non reconnus : %d.", positions->nUnidentifiedMarkers);
                    Logger::WriteMessage((const char *)buff);
                    for (numBodies = 0; numBodies < nbObjTracked; numBodies++) {
                        bodyData = positions->BodyData[numBodies];
                        sprintf_s(buff, 255, " Nom de l'objet :              %s.", bodyData.szName);
                        Logger::WriteMessage((const char *)buff);
                        sprintf_s(buff, 255, "  Nombre de marqueurs : %d.", bodyData.nMarkers);
                        Logger::WriteMessage((const char *)buff);
                        for (mark = 0; mark < bodyData.nMarkers; mark++) {
                            for (coord = 0; coord < 3; coord++) {
                                sprintf_s(buff, 255, "  Mark[%i][%i] : %f.", mark, coord, bodyData.Markers[mark][coord]);
                                Logger::WriteMessage((const char *)buff);
                            }
                        }
                        sprintf_s(buff, 255, "  Nombre de segments : %d.", bodyData.nSegments);
                        Logger::WriteMessage((const char *)buff);
                        //pos = (float*)bodyData.Markers;
                        //sprintf_s(buff, 255, "  position : X=%f, Y=%f, Z=%f.", pos[0], pos[1], pos[2]);
                        //Logger::WriteMessage((const char *)buff);
                        
                        //récupère les data du point au centre de l'essieu du robot (i.e. le 1er virtual point)
                        coordonnees_centre_essieu = (&positions->BodyData[numBodies])->Markers[4]; 
                        *x = coordonnees_centre_essieu[0];
                        *y = coordonnees_centre_essieu[1];
                        *z = coordonnees_centre_essieu[2];
                        sprintf_s(buff, 255, "  centre essieu : x=%f, y=%f, z=%f.", *x, *y, *z);
                        Logger::WriteMessage((const char *)buff);
                        //récupère les data du point virtuel à l'avant (celui qui définit le segment orientation)
                        coordonnees_avt = (&positions->BodyData[numBodies])->Markers[6]; 
                        *x_avt = coordonnees_avt[0];
                        *y_avt = coordonnees_avt[1];
                        *z_avt = coordonnees_avt[2];
                        sprintf_s(buff, 255, "  avant : x=%f, y=%f, z=%f.", *x_avt, *y_avt, *z_avt);
                        Logger::WriteMessage((const char *)buff);
                        //calcul de l'orientation du robot dans le plan de la scène (en degres)
                        orient = atan2((double)(coordonnees_avt[1] - coordonnees_centre_essieu[1]), (double)(coordonnees_avt[0] - coordonnees_centre_essieu[0]))*180.0/M_PI;

                       
                        /*  coordMiddle = bodyData.Markers[7];
                        coordFront = bodyData.Markers[6];
                        if (coordMiddle[0] == 9999999 || coordFront[0] == 9999999) {
                        //    throw(ios_base::failure("Bad data from Cortex"));
                        }
                        ret[0] = 1;
                        ret[1] = (double)coordMiddle[0];//*.1;
                        ret[2] = (double)coordMiddle[1];//*.1;
                        ret[3] = (double)coordMiddle[2];//*.1;
                        aux = atan2((double)(coordFront[1] - coordMiddle[1]), (double)(coordFront[0] - coordMiddle[0]));
                        ret[4] = aux;*/
                    }
                }
                //Cortex_SetClientCommunicationEnabled(0);
                //retval = Cortex_Exit();
                //Assert::AreEqual((int)RC_Okay, retval);
            }
        }
    };
}
