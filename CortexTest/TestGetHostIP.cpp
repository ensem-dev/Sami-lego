#include "stdafx.h"

#include <winsock2.h>  //DWORD....
#include <IPHlpApi.h>   //GAA_FLAG_INCLUDE_PREFIX
#include <WS2tcpip.h>    //inet_ntop qui remplace inet_ntoa

#pragma comment(lib, "IPHLPAPI.lib")  // Link with Iphlpapi.lib
#pragma comment(lib, "Ws2_32.lib")   // Ajouté pour inet_ntop()

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace GetHostIpTest
{

    TEST_CLASS(TestGetHostIp)
    {
    public:

        // https://msdn.microsoft.com/en-us/library/windows/desktop/aa366309(v=vs.85).aspx
        // https://msdn.microsoft.com/en-us/library/windows/desktop/aa365949(v=vs.85).aspx
        TEST_METHOD(TestGetIpAddrTable) {
            char *IpNetwork = "192.168.1.109";
            int ipSubnet = 0;
            char buff[256];
            MIB_IPADDRTABLE *pIPAddrTable;
            long dwRetVal = 0;
            DWORD dwSize = 0;
            int cptIPs = 0;
            IN_ADDR IPAddr;
            IN_ADDR IPSubnet;
            IN_ADDR IPSubnet2;
            char ipBuff[20];

            inet_pton(AF_INET, IpNetwork, &ipSubnet);
            sprintf_s(buff, 255, "IP HostCortex : %d", ipSubnet);
            Logger::WriteMessage(buff);

            pIPAddrTable = (MIB_IPADDRTABLE*)malloc(sizeof(MIB_IPADDRTABLE));
            Assert::IsNotNull(pIPAddrTable);
            if (pIPAddrTable) {
                dwRetVal = GetIpAddrTable(pIPAddrTable, &dwSize, 0);
                Assert::IsTrue(ERROR_INSUFFICIENT_BUFFER == dwRetVal);
                if (ERROR_INSUFFICIENT_BUFFER == dwRetVal) {
                    free(pIPAddrTable);
                    pIPAddrTable = (MIB_IPADDRTABLE*)malloc(dwSize);
                    Assert::IsNotNull(pIPAddrTable);
                    if (pIPAddrTable) {
                        dwRetVal = GetIpAddrTable(pIPAddrTable, &dwSize, 0);
                        Assert::IsTrue(NO_ERROR == dwRetVal);
                        if (NO_ERROR == dwRetVal) {
                            for (cptIPs = 0; cptIPs < (int)pIPAddrTable->dwNumEntries; cptIPs++) {
                                sprintf_s(buff, 255, "IP addr[%i]", cptIPs);
                                Logger::WriteMessage(buff);
                                sprintf_s(buff, 255, "\t%ld", pIPAddrTable->table[cptIPs].dwAddr);
                                Logger::WriteMessage(buff);

                                IPAddr.S_un.S_addr = (u_long)pIPAddrTable->table[cptIPs].dwAddr;
                                inet_ntop(AF_INET, &IPAddr, ipBuff, 19);
                                sprintf_s(buff, 255, "\t%s", ipBuff);
                                Logger::WriteMessage(buff);
                                IPSubnet.S_un.S_addr = pIPAddrTable->table[cptIPs].dwAddr & pIPAddrTable->table[cptIPs].dwMask;
                                inet_ntop(AF_INET, &IPSubnet, ipBuff, 19);
                                sprintf_s(buff, 255, "\t subnet %s", ipBuff);
                                Logger::WriteMessage(buff);

                                IPSubnet2.S_un.S_addr = ipSubnet & pIPAddrTable->table[cptIPs].dwMask;
                                inet_ntop(AF_INET, &IPSubnet2, ipBuff, 19);
                                sprintf_s(buff, 255, "\t subnet2 %s", ipBuff);
                                Logger::WriteMessage(buff);
                                if (IPSubnet.S_un.S_addr == IPSubnet2.S_un.S_addr) {
                                    Logger::WriteMessage("on a trouver la bonne ip");
                                    //sprintf_s(buff, 255, "\t%s", ipBuff);
                                    //Logger::WriteMessage(buff);
                                }
                            }
                        }
                        else {
                            //TODO il faut traiter les erreur de GetIpAddrTable
                            Assert::Fail(L"GetIpAddrTable à retourné une erreur !");
                        }
                        free(pIPAddrTable);
                    }
                    else {
                        //TODO il y a eu un PB avec Malloc
                        Assert::Fail(L"Un problème avec malloc. le pointeur n'est pas initialisé");
                    }
                }
            }
        }
        
        /*        TEST_METHOD(TestFindIpOnNetwork) {
            char *IpNetwork = "192.168.1.109";
            char buff[256]="CouCOU";
            PIP_ADAPTER_INFO pAdapterInfo;
            PIP_ADAPTER_INFO pAdapter = NULL;
            long dwRetVal = 0;
            ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
            IP_ADDRESS_STRING ip;
            //char ip[16];
            IP_ADDRESS_STRING mask;
            pAdapterInfo = (IP_ADAPTER_INFO *)malloc(sizeof(IP_ADAPTER_INFO));
            if (pAdapterInfo) {
                dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen);
                if (ERROR_BUFFER_OVERFLOW == dwRetVal) {
                    free(pAdapterInfo);
                    pAdapterInfo = (IP_ADAPTER_INFO *)malloc(sizeof(IP_ADAPTER_INFO));
                    dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen);
                    if (NO_ERROR == dwRetVal) {
                        pAdapter = pAdapterInfo;
                        while (pAdapter) {
                            ip = pAdapter->IpAddressList.IpAddress;
                            mask = pAdapter->IpAddressList.IpMask;
                            for (int i = 0; i < 16; i++) {
                                //strcat(buff, pAdapter->IpAddressList.IpAddress[i]);
                               // Logger::WriteMessage(buff);
                            }
                            //unsigned long ipaddr = INADDR_NONE;
                            //ipaddr = inet_addr("168.192.1.103");
                            //IPAddr 
                            //unsigned long inet = inet_addr((const char*)&ip);
                            
                        }
                    }
                    else {
                        //TODO on doit manque de place mémoire pour récupérer les # IPs. ça ne devrait pas arriver
                    }
                }
                else {
                    //TODO normalement on ne devrait jamais arriver ici. 
                }
            }

        }*/


/*        // https://msdn.microsoft.com/en-us/library/windows/desktop/aa365917(v=vs.85).aspx
        TEST_METHOD(TestGetAdaptersInfo) {
            wchar_t buff[256];

            UINT i;
            struct tm newtime;
            char buffer[32];
            errno_t error;

            PIP_ADAPTER_INFO pAdapterInfo;
            long dwRetVal = 0;
            ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
            PIP_ADAPTER_INFO pAdapter = NULL;

            pAdapterInfo = (IP_ADAPTER_INFO *)malloc(sizeof(IP_ADAPTER_INFO));
            Assert::IsNotNull(pAdapterInfo, L"Error allocating memory needed to call GetAdaptersinfo");
            // Make an initial call to GetAdaptersInfo to get
            // the necessary size into the ulOutBufLen variable
            dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen);
            Assert::AreEqual(ERROR_BUFFER_OVERFLOW, dwRetVal);
            pAdapterInfo = (IP_ADAPTER_INFO *)malloc(ulOutBufLen);
            Assert::IsNotNull(pAdapterInfo);
            dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen);
            Assert::AreEqual(NO_ERROR, dwRetVal);
            pAdapter = pAdapterInfo;
            while (pAdapter) {
                swprintf(buff, 255, L"Adapter name: %S", pAdapter->AdapterName);
                Logger::WriteMessage(buff);
                swprintf(buff, 255, L"Adapter Desc: %S", pAdapter->Description);
                Logger::WriteMessage(buff);
                //Logger::WriteMessage(L"Adapter Addr:");
                swprintf(buff, 255, L"Adapter Desc: %S", pAdapter->IpAddressList.IpAddress);
                Logger::WriteMessage(buff);*/

                /*                   printf("\tComboIndex: \t%d\n", pAdapter->ComboIndex);
                printf("\tAdapter Addr: \t");
                for (i = 0; i < pAdapter->AddressLength; i++) {
                if (i == (pAdapter->AddressLength - 1))
                printf("%.2X\n", (int)pAdapter->Address[i]);
                else
                printf("%.2X-", (int)pAdapter->Address[i]);
                }
                printf("\tIndex: \t%d\n", pAdapter->Index);
                printf("\tType: \t");
                switch (pAdapter->Type) {
                case MIB_IF_TYPE_OTHER:
                printf("Other\n");
                break;
                case MIB_IF_TYPE_ETHERNET:
                printf("Ethernet\n");
                break;
                case MIB_IF_TYPE_TOKENRING:
                printf("Token Ring\n");
                break;
                case MIB_IF_TYPE_FDDI:
                printf("FDDI\n");
                break;
                case MIB_IF_TYPE_PPP:
                printf("PPP\n");
                break;
                case MIB_IF_TYPE_LOOPBACK:
                printf("Lookback\n");
                break;
                case MIB_IF_TYPE_SLIP:
                printf("Slip\n");
                break;
                default:
                printf("Unknown type %ld\n", pAdapter->Type);
                break;
                }

                printf("\tIP Address: \t%s\n",
                pAdapter->IpAddressList.IpAddress.String);
                printf("\tIP Mask: \t%s\n", pAdapter->IpAddressList.IpMask.String);

                printf("\tGateway: \t%s\n", pAdapter->GatewayList.IpAddress.String);
                printf("\t***\n");

                if (pAdapter->DhcpEnabled) {
                printf("\tDHCP Enabled: Yes\n");
                printf("\t  DHCP Server: \t%s\n",
                pAdapter->DhcpServer.IpAddress.String);

                printf("\t  Lease Obtained: ");

                error = _localtime32_s(&newtime, (__time32_t*)&pAdapter->LeaseObtained);
                if (error)
                printf("Invalid Argument to _localtime32_s\n");
                else {
                // Convert to an ASCII representation
                error = asctime_s(buffer, 32, &newtime);
                if (error)
                printf("Invalid Argument to asctime_s\n");
                else

                printf("%s", buffer);
                }

                printf("\t  Lease Expires:  ");
                error = _localtime32_s(&newtime, (__time32_t*)&pAdapter->LeaseExpires);
                if (error)
                printf("Invalid Argument to _localtime32_s\n");
                else {
                // Convert to an ASCII representation
                error = asctime_s(buffer, 32, &newtime);
                if (error)
                printf("Invalid Argument to asctime_s\n");
                else

                printf("%s", buffer);
                }
                }
                else
                printf("\tDHCP Enabled: No\n");

                if (pAdapter->HaveWins) {
                printf("\tHave Wins: Yes\n");
                printf("\t  Primary Wins Server:    %s\n",
                pAdapter->PrimaryWinsServer.IpAddress.String);
                printf("\t  Secondary Wins Server:  %s\n",
                pAdapter->SecondaryWinsServer.IpAddress.String);
                }
                else
                printf("\tHave Wins: No\n");*/
 /*               pAdapter = pAdapter->Next;
                Logger::WriteMessage("--------------------------------------------");
            }
            free(pAdapterInfo);
        }*/

        /**
        * Pour mémoire car on y a passé presque une journée entière avant de trouver la bonne fonction GetAdapterInfo !!
        * https://msdn.microsoft.com/en-us/library/windows/desktop/aa366058(v=vs.85).aspx
        * https://msdn.microsoft.com/en-us/library/windows/desktop/aa365915(v=vs.85).aspx
        */
/*        TEST_METHOD(TestGetAdaptersAddresses) {
            wchar_t buff[256];
            DWORD dwSize = 0;
            long dwRetVal = 0;
            unsigned int i = 0;
            // Set the flags to pass to GetAdaptersAddresses
            ULONG flags = GAA_FLAG_INCLUDE_PREFIX;
            // default to unspecified address family (both)
            ULONG family = AF_INET; // IPv4 -> All : AF_UNSPEC;
            LPVOID lpMsgBuf = NULL;
            PIP_ADAPTER_ADDRESSES pAddresses = NULL;
            ULONG outBufLen = 0;
            PIP_ADAPTER_ADDRESSES pCurrAddresses = NULL;
            PIP_ADAPTER_UNICAST_ADDRESS pUnicast = NULL;
            PIP_ADAPTER_ANYCAST_ADDRESS pAnycast = NULL;
            PIP_ADAPTER_MULTICAST_ADDRESS pMulticast = NULL;
            IP_ADAPTER_DNS_SERVER_ADDRESS *pDnServer = NULL;
            IP_ADAPTER_PREFIX *pPrefix = NULL;

            outBufLen = sizeof(IP_ADAPTER_ADDRESSES);
            // Make an initial call to GetAdaptersAddresses to get the size needed into the outBufLen variable
            long retval = GetAdaptersAddresses(family, flags, NULL, pAddresses, &outBufLen);
            Assert::AreEqual(ERROR_BUFFER_OVERFLOW, retval);
            pAddresses = (IP_ADAPTER_ADDRESSES *)malloc(outBufLen);
            Assert::IsNotNull(pAddresses, L"Memory allocation failed for IP_ADAPTER_ADDRESSES struct");
            // Make a second call to GetAdapters Addresses to get the actual data we want
            swprintf(buff, 255, L"Memory allocated for GetAdapterAddresses = %d bytes", outBufLen);
            Logger::WriteMessage(buff);
            dwRetVal = GetAdaptersAddresses(family, flags, NULL, pAddresses, &outBufLen);
            Assert::AreEqual(NO_ERROR, dwRetVal);
            // If successful, output some information from the data we received
            pCurrAddresses = pAddresses;
            while (pCurrAddresses) {
                swprintf(buff, 255, L"Length of the IP_ADAPTER_ADDRESS struct: %ld\n\tIfIndex (IPv4 interface): %u", pCurrAddresses->Length, pCurrAddresses->IfIndex);
                Logger::WriteMessage(buff);
                swprintf(buff, 255, L"\tAdapter name: %S", pCurrAddresses->AdapterName);
                Logger::WriteMessage(buff);
                pUnicast = pCurrAddresses->FirstUnicastAddress;
                if (pUnicast != NULL) {
                    for (i = 0; pUnicast != NULL; i++) pUnicast = pUnicast->Next;
                    swprintf(buff, 255, L"\tNumber of Unicast Addresses: %d", i);
                    Logger::WriteMessage(buff);
                }else {
                    Logger::WriteMessage(L"\tNo Unicast Addresses");
                }
                pAnycast = pCurrAddresses->FirstAnycastAddress;
                if (pAnycast) {
                    for (i = 0; pAnycast != NULL; i++) pAnycast = pAnycast->Next;
                    swprintf(buff, 255, L"\tNumber of Anycast Addresses: %d", i);
                    Logger::WriteMessage(buff);
                    //printf("\tNumber of Anycast Addresses: %d\n", i);
                } else {
                    Logger::WriteMessage(L"\tNo Anycast Addresses");
                    //printf("\tNo Anycast Addresses\n");
                }
                pMulticast = pCurrAddresses->FirstMulticastAddress;
                if (pMulticast) {
                    for (i = 0; pMulticast != NULL; i++) pMulticast = pMulticast->Next;
                    //printf("\tNumber of Multicast Addresses: %d\n", i);
                    swprintf(buff, 255, L"\tNumber of Multicast Addresses: %d", i);
                    Logger::WriteMessage(buff);
                } else {
                    Logger::WriteMessage(L"\tNo Multicast Addresses");
                    //printf("\tNo Multicast Addresses\n");
                }
                pDnServer = pCurrAddresses->FirstDnsServerAddress;
                if (pDnServer) {
                   for (i = 0; pDnServer != NULL; i++) pDnServer = pDnServer->Next;
                   //printf("\tNumber of DNS Server Addresses: %d\n", i);
                   swprintf(buff, 255, L"\tNumber of DNS Server Addresses: %d", i);
                   Logger::WriteMessage(buff);
                } else {
                   Logger::WriteMessage(L"\tNo DNS Server Addresses");
                   //printf("\tNo DNS Server Addresses\n");
                }
                swprintf(buff, 255, L"\tDNS Suffix: %wS", pCurrAddresses->DnsSuffix);
                Logger::WriteMessage(buff);
                //printf("\tDNS Suffix: %wS\n", pCurrAddresses->DnsSuffix);
                swprintf(buff, 255, L"\tDescription: %wS", pCurrAddresses->Description);
                Logger::WriteMessage(buff);
                //printf("\tDescription: %wS\n", pCurrAddresses->Description);
                swprintf(buff, 255, L"\tFriendly name: %wS", pCurrAddresses->FriendlyName);
                Logger::WriteMessage(buff);
                //printf("\tFriendly name: %wS\n", pCurrAddresses->FriendlyName);
                if (pCurrAddresses->PhysicalAddressLength != 0) {
                    swprintf(buff, 255, L"\tPhysical address: ");
                    Logger::WriteMessage(buff);
                    //printf("\tPhysical address: ");
                    for (i = 0; i < pCurrAddresses->PhysicalAddressLength; i++) {
                        if (i == (pCurrAddresses->PhysicalAddressLength - 1)) {
                            swprintf(buff, 255, L"%.2X", (int)pCurrAddresses->PhysicalAddress[i]);
                            Logger::WriteMessage(buff);
                            //printf("%.2X\n", (int)pCurrAddresses->PhysicalAddress[i]);
                        } else {
                            swprintf(buff, 255, L"%.2X - ",(int)pCurrAddresses->PhysicalAddress[i]);
                            Logger::WriteMessage(buff);
                            //printf("%.2X-", (int)pCurrAddresses->PhysicalAddress[i]);
                        }
                    }
                }
                swprintf(buff, 255, L"\tFlags: %ld", pCurrAddresses->Flags);
                Logger::WriteMessage(buff);
                //printf("\tFlags: %ld\n", pCurrAddresses->Flags);
                swprintf(buff, 255, L"\tMtu: %lu", pCurrAddresses->Mtu);
                Logger::WriteMessage(buff);
                //printf("\tMtu: %lu\n", pCurrAddresses->Mtu);
                swprintf(buff, 255, L"\tIfType: %ld", pCurrAddresses->IfType);
                Logger::WriteMessage(buff);
                //printf("\tIfType: %ld\n", pCurrAddresses->IfType);
                swprintf(buff, 255, L"\tOperStatus: %ld", pCurrAddresses->OperStatus);
                Logger::WriteMessage(buff);
                //printf("\tOperStatus: %ld\n", pCurrAddresses->OperStatus);
                swprintf(buff, 255, L"\tIpv6IfIndex (IPv6 interface): %u", pCurrAddresses->Ipv6IfIndex);
                Logger::WriteMessage(buff);
                //printf("\tIpv6IfIndex (IPv6 interface): %u\n", pCurrAddresses->Ipv6IfIndex);
                Logger::WriteMessage(L"\tZoneIndices (hex): ");
                //printf("\tZoneIndices (hex): ");
                for (i = 0; i < 16; i++) {
                    swprintf(buff, 255, L"%lx", pCurrAddresses->ZoneIndices[i]);
                    Logger::WriteMessage(buff);
                    //printf("%lx ", pCurrAddresses->ZoneIndices[i]);
                }                
                Logger::WriteMessage("\n");
                pPrefix = pCurrAddresses->FirstPrefix;
                if (pPrefix) {
                   for (i = 0; pPrefix != NULL; i++) pPrefix = pPrefix->Next;
                   //printf("\tNumber of IP Adapter Prefix entries: %d\n", i);
                   swprintf(buff, 255, L"\tNumber of IP Adapter Prefix entries: %d", i);
                   Logger::WriteMessage(buff);
                }
                else {
                    Logger::WriteMessage("\tNo IP Adapter Prefix entries");

                }
                Logger::WriteMessage("--------------------------------------------");
                pCurrAddresses = pCurrAddresses->Next;
           }
           free(pAddresses);
        }*/

    };
}