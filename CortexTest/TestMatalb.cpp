#include "stdafx.h"

#include "../dllCortex/Cortex.h"
#include "../Cortex_Matlab/Cortex_Matlab.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CortexTest
{
    TEST_CLASS(TestCortex)
    {
    public:

        int getPositions() {
            int result = 1;
            char *ipCortexServer = "192.168.1.109";
            char errorMessage[256] = "pas de message";
            char buff[256];
            int nbObjTracked, numBodies;
            sBodyData bodyData;
            float *pos;
            if (getCortexConnexion(ipCortexServer, errorMessage)) {
                Logger::WriteMessage(errorMessage);
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
                    pos = (float*)bodyData.Markers;
                    sprintf_s(buff, 255, "  position : X=%f, Y=%f, Z=%f.", pos[0], pos[1], pos[2]);
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
            }
            return result;
        }

        TEST_METHOD(TestGetConnexion) {
            char *ipCortexServer = "192.168.1.109";
            char errorMessage[256] = "pas de message";
            int retval;
            retval = getCortexConnexion(ipCortexServer, errorMessage);
            Assert::AreEqual(1, retval);
        }

        TEST_METHOD(TestGetObjectPositionCortex) {
            char *ipCortexServer = "192.168.1.109";
            char errorMessage[256] = "pas de message";
            int retval;
            wchar_t buff[256];

            int objectIndex = 0;
            double X = 0.0;
            double Y = 0.0;
            double Z = 0.0;
            double azimut = 0.0;
            double elevation = 0.0;

            retval = getCortexConnexion(ipCortexServer, errorMessage);
            if (retval) {
                retval = getObjectPositionCortex(objectIndex, &X, &Y, &Z, &azimut, &elevation);
                Assert::AreEqual(0, retval);
                swprintf(buff, 255, L"position X mauvaise : X=%f", X);
                Assert::IsTrue(X < -22, buff);
                swprintf(buff, 255, L"position Y mauvaise : Y=%f", Y);
                Assert::IsTrue(Y > 66, buff);
                swprintf(buff, 255, L"position Z mauvaise : Z=%f", Z);
                Assert::IsTrue(Z > 158, buff);
                Assert::IsTrue(azimut == 0.0, L"azimut mauvais");
                Assert::IsTrue(elevation == 0.0, L"élévation mauvaise");
            }
        }
    };
}