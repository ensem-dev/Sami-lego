#include "stdafx.h"

#include "../dllCortex/Cortex.h"
#include "../Cortex_Matlab/Cortex_Matlab.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CortexTest
{
    TEST_CLASS(TestCortex)
    {
    public:

        TEST_METHOD(TestFindLocalIP) {
            char localIP[20];
            bool retval = findLocalIP("192.168.1.109", localIP);
            Assert::IsTrue(retval);
            Assert::AreEqual("192.168.1.103", localIP);
        }

        TEST_METHOD(TestGetConnexion) {
            char *ipCortexServer = "192.168.1.109";
            char errorMessage[256] = "pas de message";
            int retval;
            retval = getCortexConnexion(ipCortexServer, errorMessage);
            Logger::WriteMessage(errorMessage);
            Assert::AreEqual(1, retval);
        }

        TEST_METHOD(TestGetObjectPositionCortex) {
            char *ipCortexServer = "192.168.1.109";
            char errorMessage[256] = "pas de message";
            int retval;
            wchar_t buff[256];

            int objectIndex = 0;
            float X = 0.0;
            float Y = 0.0;
            float Z = 0.0;
            double azimut = 0.0;
            double elevation = 0.0;

            retval = getCortexConnexion(ipCortexServer, errorMessage);
            if (retval) {
                retval = getObjectPositionCortex(objectIndex, &X, &Y, &Z, &azimut, &elevation);
                Assert::AreEqual(0, retval);
                swprintf(buff, 255, L"position X  : X=%f", X);
                Logger::WriteMessage(buff);
                //Assert::IsTrue(X < -22, buff);
                swprintf(buff, 255, L"position Y  : Y=%f", Y);
                Logger::WriteMessage(buff);
                //Assert::IsTrue(Y > 66, buff);
                swprintf(buff, 255, L"position Z  : Z=%f", Z);
                Logger::WriteMessage(buff);
                //Assert::IsTrue(Z > 158, buff);
                //Assert::IsTrue(azimut == 0.0, L"azimut mauvais");
                Assert::IsTrue(elevation == 0.0, L"élévation mauvaise");
            }
            else {
                //TODO : générer une execption avec le message d'erreur errorMessage
            }
        }
        TEST_METHOD(TestGetObjectPositionCortex1000) {
            char *ipCortexServer = "192.168.1.109";
            char errorMessage[256] = "pas de message";
            int retval;
            char buff[256];

            int objectIndex = 0;
            float X = 0.0;
            float Y = 0.0;
            float Z = 0.0;
            double azimut = 0.0;
            double elevation = 0.0;
            int cpt = 0;
            while (cpt < 10000) {
                retval = getCortexConnexion(ipCortexServer, errorMessage);
                if (retval) {
                    cpt++;
                    sprintf_s(buff, 255, "%i", cpt);
                    Logger::WriteMessage(buff);
                }
                else {
                    sprintf_s(buff, 255, "Erreur de retour de getCortexConnexion : %i", retval);
                    Logger::WriteMessage(buff);
                    cpt = 1001;
                }
            }
            exitCortexConnexion();
        }
    };
}