#include "stdafx.h"
#include "TestConstantes.h"

#include "../dllCortex/Cortex.h"
#include "../Cortex_Matlab/Cortex_Matlab.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CortexTest
{
    TEST_CLASS(TestCortex)
    {
    public:

        /**
         * Sélectionne l'adresse IP de la machine qui est sur le même réseau que l'adresse distante passée en paramètre
         * les valeurs à tester sont écrite dans le fichier TestConstantes.h
         */
        TEST_METHOD(TestFindLocalIP) {
            char *expectedIP = "100.64.212.129"; // cette IP est celle de la machine sur laquelle le test est lancé.
            char localIP[20];
            bool retval = findLocalIP(IP_CORTEX, localIP);
            Assert::IsTrue(retval, L"aucune adresse IP correspondante trouvée. Vérifier TestConstantes.h");
            Assert::AreEqual(expectedIP, localIP, L"Vérifier les adresses IP dans le fichier TestConstantes.h");
        }

        /** Test d'utilisation de la méthode _strdup
         */
        // https://msdn.microsoft.com/fr-fr/library/y471khhc.aspx
        TEST_METHOD(Test_strdup) {
            char buffer[] = "This is the buffer text";
            char *newstring;
            Logger::WriteMessage(buffer);
            newstring = _strdup(buffer);
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

        /** On controle la connexion à la platform Cortex*/
        TEST_METHOD(TestGetConnexion) {
            char *errorMessage = NULL;
            int retval;
            retval = getCortexConnexion(IP_CORTEX, &errorMessage);
            Logger::WriteMessage(errorMessage);
            Assert::AreEqual(1, retval);
            if (strcmp(errorMessage, "Ok c'est bon on a initialise une connexion, mais l'application cortex n'est pas bien démarrée")) {
                Assert::Fail(L"getFrame ne fonctionne pas");
            }
            free(errorMessage);
        }

        /**
         * On essaie de lire la position du premier mobile géré par Cortex
         */
        TEST_METHOD(TestGetObjectPositionCortex) {
            char *errorMessage[256];// = "pas de message";
            int retval;
            wchar_t buff[256];

            int objectIndex = 0;
            float X = 0.0;
            float Y = 0.0;
            float Z = 0.0;
            double azimut = 0.0;
            double elevation = 0.0;

            getCortexConnexion(IP_CORTEX, errorMessage);
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
            int nbCycle = 10; //Selon le nom du test cette valeur devrait être 1000

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
            getCortexConnexion(IP_CORTEX, errorMessage);
            while (cpt < nbCycle) {
                retval = getObjectPositionCortexID(objectIndex, &X, &Y, &Z, &azimut, &elevation);
                if (!retval) {
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
                cpt++;
            }
            exitCortexConnexion();
        }

        TEST_METHOD(TestGetObjectPositionCortexByName) {
            char* objectName = "Lego_04"; //Il faut que cette objet soit déclaré parmi les mobiles gérés par Cortex

            char *errorMessage[256];// = "pas de message";
            int retval;
            wchar_t buff[256];

            int objectIndex = 0;
            float X = 0.0;
            float Y = 0.0;
            float Z = 0.0;
            double azimut = 0.0;
            double elevation = 0.0;
            
            getCortexConnexion(IP_CORTEX, errorMessage);
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