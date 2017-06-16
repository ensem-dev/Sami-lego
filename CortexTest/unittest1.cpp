#include "stdafx.h"
#include "CppUnitTest.h"

#include "../dllCortex/Cortex.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CortexTest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			// TODO: Ici, votre code de test
            int expected;
            int actual;
            expected = 0;
            actual = 0;
            Assert::AreEqual(expected, actual);
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
            int numBodies;
            int nbObjTracked;
            sBodyData bodyData;
            tMarkerData *pos;
            //int nbMarkerTracked;
            //int numMark;
            //tMarkerData coordS;
            //float* coords;
            char buff[255];
            int isConnected = Cortex_IsClientCommunicationEnabled();
            char *host_matlab = "192.168.1.103";  //TODO trouver l'IP de la machine courante
            int retval;
            if (getCortexConnexion(ipCortexServer)) {
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
                    sprintf_s(buff, 255, "  Nombre de segments : %d.", bodyData.nSegments);
                    Logger::WriteMessage((const char *)buff);
                    pos = bodyData.Markers;
                    sprintf_s(buff, 255, "  position : X=%f, Y=%f, Z=%f.", *pos[0], *pos[1], *pos[2]);
                    Logger::WriteMessage((const char *)buff);
               /*   coordMiddle = bodyData.Markers[7];
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
                retval = Cortex_Exit();
                Assert::AreEqual((int)RC_Okay, retval);
            }
        }
    };
}