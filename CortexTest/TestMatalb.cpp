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

        // https://msdn.microsoft.com/fr-fr/library/y471khhc.aspx
        TEST_METHOD(Test_strdup) {
            char buffer[] = "This is the buffer text";
            char *newstring;
            Logger::WriteMessage(buffer);
            //printf("Original: %s\n", buffer);
            newstring = _strdup(buffer);
            //printf("Copy:     %s\n", newstring);
            Logger::WriteMessage(newstring);
            free(newstring);
        }

        // https://stackoverflow.com/questions/7445054/passing-pointer-argument-in-matlab-to-a-c-dll-function-foochar
        void myGetVersion(char **str) {
            *str = _strdup("1.0.0");
        }

        TEST_METHOD(Test_passStringArgument) {
            char *str = NULL;
            myGetVersion(&str);
            printf("%s\n", str);
            free(str);
        }

        TEST_METHOD(TestGetConnexion) {
            char *ipCortexServer = "192.168.1.109";
            char *errorMessage = NULL;
            int retval;
            //sprintf_s(*errorMessage, 255, "pas de message");
            retval = getCortexConnexion(ipCortexServer, &errorMessage);
            Logger::WriteMessage(errorMessage);
            Assert::AreEqual(1, retval);
            free(errorMessage);
        }

        TEST_METHOD(TestGetObjectPositionCortex) {
            char *ipCortexServer = "192.168.1.109";
            char *errorMessage[256];// = "pas de message";
            int retval;
            wchar_t buff[256];

            int objectIndex = 0;
            float X = 0.0;
            float Y = 0.0;
            float Z = 0.0;
            double azimut = 0.0;
            double elevation = 0.0;

            getCortexConnexion(ipCortexServer, errorMessage);
            //if (retval) {
                retval = getObjectPositionCortexID(objectIndex, &X, &Y, &Z, &azimut, &elevation);
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
            //}
            //else {
                //TODO : générer une execption avec le message d'erreur errorMessage
            //}
            exitCortexConnexion();
        }

        TEST_METHOD(TestGetObjectPositionCortex1000) {
            char *ipCortexServer = "192.168.1.109";
            char *errorMessage[256];// = "pas de message";
            int retval;
            char buff[256];

            int objectIndex = 0;
            float X = 0.0;
            float Y = 0.0;
            float Z = 0.0;
            double azimut = 0.0;
            double elevation = 0.0;
            int cpt = 0;
            getCortexConnexion(ipCortexServer, errorMessage);
            while (cpt < 10000) {
                retval = getObjectPositionCortexID(objectIndex, &X, &Y, &Z, &azimut, &elevation);
                if (!retval) {
                    cpt++;
                    //sprintf_s(buff, 255, "%i", cpt);
                    //Logger::WriteMessage(buff);
                    //swprintf(buff, 255, L"position X  : X=%f", X);
                    //Logger::WriteMessage(buff);
                    //swprintf(buff, 255, L"position Y  : Y=%f", Y);
                    //Logger::WriteMessage(buff);
                    //swprintf(buff, 255, L"position Z  : Z=%f", Z);
                }
                else {
                    sprintf_s(buff, 255, "Erreur de retour de getObjectPositionCortex : %i x=%f", cpt, X);
                    Logger::WriteMessage(buff);
                    //cpt = 10001;
                }
            }
            exitCortexConnexion();
        }

        TEST_METHOD(TestGetObjectPositionCortexByName) {
            char *ipCortexServer = "192.168.1.109";
            char *errorMessage[256];// = "pas de message";
            int retval;
            wchar_t buff[256];

            int objectIndex = 0;
            char* objectName = "Lego_04";
            float X = 0.0;
            float Y = 0.0;
            float Z = 0.0;
            double azimut = 0.0;
            double elevation = 0.0;
            
            getCortexConnexion(ipCortexServer, errorMessage);
            retval = getObjectPositionCortexByName(objectName, &X, &Y, &Z, &azimut, &elevation);
            Assert::AreEqual(0, retval);
            swprintf(buff, 255, L"position X  : X=%f", X);
            Logger::WriteMessage(buff);
            swprintf(buff, 255, L"position Y  : Y=%f", Y);
            Logger::WriteMessage(buff);
            swprintf(buff, 255, L"position Z  : Z=%f", Z);
            Logger::WriteMessage(buff);

            exitCortexConnexion();
        }
    };
}