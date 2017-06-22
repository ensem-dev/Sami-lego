#include "stdafx.h"

#include "../dllCortex/Cortex.h"
#include "../Cortex_Matlab/Cortex_Matlab.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CortexTest
{
    TEST_CLASS(TestCortex)
    {
    public:


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
                //Assert::IsTrue(X < -22, buff);
                swprintf(buff, 255, L"position Y mauvaise : Y=%f", Y);
                //Assert::IsTrue(Y > 66, buff);
                swprintf(buff, 255, L"position Z mauvaise : Z=%f", Z);
                //Assert::IsTrue(Z > 158, buff);
                //Assert::IsTrue(azimut == 0.0, L"azimut mauvais");
                Assert::IsTrue(elevation == 1.0, L"élévation mauvaise");
            }
            else {
                //TODO : générer une execption avec le message d'erreur errorMessage
            }
        }
    };
}