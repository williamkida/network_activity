#include "pch.h"
#pragma once
#pragma hdrstop
#pragma argsused
#define DO_EXPORTS
#include "network_activity.h"
//#include <stdio.h>
//#include <iomanip>
//#include <winsock2.h>
//#include <ws2tcpip.h>
//#include <ws2ipdef.h>
//#include <iphlpapi.h>
//#include <wchar.h>
//#include "combaseapi.h"


#pragma comment(lib, "iphlpapi.lib")
#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))
using namespace std;
extern int _libmain(unsigned long reason)
{
	return 1;
}
extern __declspec(dllexport) void __stdcall GetIpv4EnabledNics()
{
	printf("\n__________GetIPv4EnabledNics\n");
	std::wofstream ipv4enablednicslist("ipv4enablednicslist.txt");
	// Declare and initialize variables
	PIP_INTERFACE_INFO pInfo = NULL;
	ULONG ulOutBufLen = 0;

	DWORD dwRetVal = 0;
	int iReturn = 1;

	int i;

	// Make an initial call to GetInterfaceInfo to get
	// the necessary size in the ulOutBufLen variable
	dwRetVal = GetInterfaceInfo(NULL, &ulOutBufLen);
	if (dwRetVal == ERROR_INSUFFICIENT_BUFFER) {
		pInfo = (IP_INTERFACE_INFO*)MALLOC(ulOutBufLen);
	}
	// Make a second call to GetInterfaceInfo to get
	// the actual data we need
	dwRetVal = GetInterfaceInfo(pInfo, &ulOutBufLen);
	if (dwRetVal == NO_ERROR) {
		// WILL NOT COUNT LOOP BACK INTERFACE AS AN IPV4 ADAPTER
		printf("Number of Adapters: %ld\n\n", pInfo->NumAdapters);
		for (i = 0; i < pInfo->NumAdapters; i++) {
			printf("Adapter Index[%d]: %ld\n", i, pInfo->Adapter[i].Index);
			wprintf(L"Adapter Name[%d]: %s\n\n", i, pInfo->Adapter[i].Name);
			ipv4enablednicslist << pInfo->Adapter[i].Name << std::endl;
		}
		ipv4enablednicslist.close();
	}
	FREE(pInfo);
}
extern __declspec(dllexport) void __stdcall GetIpv4EnabledNicsLog()
{
	std::wofstream outlog("debug.txt");
	outlog << std::endl;
	outlog << "__________GetIPv4EnabledNicsLog" << std::endl;
	printf("\n__________GetIPv4EnabledNicsLog\n");
	std::wofstream ipv4enablednicslist("ipv4enablednicslist.txt");
	// Declare and initialize variables
	PIP_INTERFACE_INFO pInfo = NULL;
	ULONG ulOutBufLen = 0;

	DWORD dwRetVal = 0;
	int iReturn = 1;

	int i;

	// Make an initial call to GetInterfaceInfo to get
	// the necessary size in the ulOutBufLen variable
	dwRetVal = GetInterfaceInfo(NULL, &ulOutBufLen);
	if (dwRetVal == ERROR_INSUFFICIENT_BUFFER) {
		pInfo = (IP_INTERFACE_INFO*)MALLOC(ulOutBufLen);
	}
	// Make a second call to GetInterfaceInfo to get
	// the actual data we need
	dwRetVal = GetInterfaceInfo(pInfo, &ulOutBufLen);
	if (dwRetVal == NO_ERROR) {
		// WILL NOT COUNT LOOP BACK INTERFACE AS AN IPV4 ADAPTER
		printf("Number of Adapters: %ld\n\n", pInfo->NumAdapters);
		for (i = 0; i < pInfo->NumAdapters; i++) {
			printf("Adapter Index[%d]: %ld\n", i, pInfo->Adapter[i].Index);
			wprintf(L"Adapter Name[%d]: %s\n\n", i, pInfo->Adapter[i].Name);
			ipv4enablednicslist << pInfo->Adapter[i].Name << std::endl;
		}
		outlog << L"num IPv4 adapters = " << pInfo->NumAdapters << std::endl;
		outlog.close();
		ipv4enablednicslist.close();
	}
	FREE(pInfo);
}
extern __declspec(dllexport) wchar_t* __stdcall GetStartupDirectory() {
	PWSTR startup_path;
	SHGetKnownFolderPath(FOLDERID_Startup,
		0,     // no special options required
		NULL,  // no access token required
		&startup_path);
	wchar_t* startup_directory(startup_path);
	return startup_directory;
}
extern __declspec(dllexport) void __stdcall CreateShortcutInStartUp() {
	PWSTR startup_path;
	wchar_t buffer[MAX_PATH];
	wchar_t* app_path = &buffer[0];
	// get app path
	GetModuleFileName(NULL, app_path, MAX_PATH);
	// get startup path
	SHGetKnownFolderPath(FOLDERID_Startup,
		0,     // no special options required
		NULL,  // no access token required
		&startup_path);
	// extract path from app_path
	std::filesystem::path working_directory(app_path);
	std::wcout << L"Full path: " << working_directory << std::endl;
	std::wcout << L"Parent path: " << working_directory.parent_path() << std::endl;
	std::wcout << L"Filename: " << working_directory.filename() << std::endl;
	// set working directory
	std::wstring wideStringWorkingDirectory = working_directory.parent_path().wstring();
	wchar_t* path(startup_path);
	std::wstring linkName(path);
	linkName.append(L"\\NetworkActivityIndicator.lnk");
	CoTaskMemFree(static_cast<LPVOID>(startup_path));
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	HRESULT hres;
	IShellLink* psl;

	// Get a pointer to the IShellLink interface. It is assumed that CoInitialize
	// has already been called.
	hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&psl);
	if (SUCCEEDED(hres))
	{
		IPersistFile* ppf;

		// Set the path to the shortcut target and add the description. 
		psl->SetPath(app_path);
		psl->SetWorkingDirectory(wideStringWorkingDirectory.c_str());
		//psl->SetDescription(lpszDesc); 

		// Query IShellLink for the IPersistFile interface, used for saving the 
		// shortcut in persistent storage. 
		hres = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf);

		if (SUCCEEDED(hres))
		{
			WCHAR wsz[MAX_PATH];

			// Ensure that the string is Unicode. 
			//MultiByteToWideChar(CP_ACP, 0, buffer_pointer, -1, wsz, MAX_PATH); 

			// Add code here to check return value from MultiByteWideChar 
			// for success.

			// Save the link by calling IPersistFile::Save. 
			hres = ppf->Save(linkName.c_str(), TRUE);
			if (SUCCEEDED(hres)) {
				printf("successfully saved");
			}
			ppf->Release();
		}
		psl->Release();
		CoUninitialize();
	}
}
extern __declspec(dllexport) DWORD __stdcall GetOutOctets()
{
	printf("\n__________GetOutOctets\n");
	std::wifstream ipv4enablednicslist("ipv4enablednicslist.txt");
	// Declare and initialize variables.
	DWORD dwOutOctets = 0;
	DWORD dwSize = 0;
	DWORD dwRetVal = 0;

	unsigned int i, j;

	/* variables used for GetIfTable and GetIfEntry */
	MIB_IFTABLE* pIfTable;
	MIB_IFROW* pIfRow;

	// Allocate memory for our pointers.
	pIfTable = (MIB_IFTABLE*)MALLOC(sizeof(MIB_IFTABLE));
	if (pIfTable == NULL) {
		printf("Error allocating memory needed to call GetIfTable\n");
		return 1;
	}
	// Make an initial call to GetIfTable to get the
	// necessary size into dwSize
	dwSize = sizeof(MIB_IFTABLE);
	if (GetIfTable(pIfTable, &dwSize, FALSE) == ERROR_INSUFFICIENT_BUFFER) {
		FREE(pIfTable);
		pIfTable = (MIB_IFTABLE*)MALLOC(dwSize);
		if (pIfTable == NULL) {
			printf("Error allocating memory needed to call GetIfTable\n");
			return 1;
		}
	}
	// Make a second call to GetIfTable to get the actual
	// data we want.
	if ((dwRetVal = GetIfTable(pIfTable, &dwSize, FALSE)) == NO_ERROR) {
		printf("\tNum Entries: %ld\n\n", pIfTable->dwNumEntries);
		std::wstring line;
		while (std::getline(ipv4enablednicslist, line))
			for (i = 0; i < pIfTable->dwNumEntries; i++) {
				pIfRow = (MIB_IFROW*)&pIfTable->table[i];

				if (line != std::wstring(pIfRow->wszName)) {
					continue;
				}
				printf("\tIndex[%d]:\t %ld\n", i, pIfRow->dwIndex);
				wprintf(L"\tInterfaceName[%d]:\t %s", i, pIfRow->wszName);
				printf("\n");
				dwOutOctets += pIfRow->dwOutOctets;
				printf("\tDescription[%d]:\t ", i);
				for (j = 0; j < pIfRow->dwDescrLen; j++)
					printf("%c", pIfRow->bDescr[j]);
				printf("\n");
				printf("\tType[%d]:\t ", i);
				switch (pIfRow->dwType) {
				case IF_TYPE_OTHER:
					printf("Other\n");
					break;
				case IF_TYPE_ETHERNET_CSMACD:
					printf("Ethernet\n");
					break;
				case IF_TYPE_ISO88025_TOKENRING:
					printf("Token Ring\n");
					break;
				case IF_TYPE_PPP:
					printf("PPP\n");
					break;
				case IF_TYPE_SOFTWARE_LOOPBACK:
					printf("Software Lookback\n");
					break;
				case IF_TYPE_ATM:
					printf("ATM\n");
					break;
				case IF_TYPE_IEEE80211:
					printf("IEEE 802.11 Wireless\n");
					break;
				case IF_TYPE_TUNNEL:
					printf("Tunnel type encapsulation\n");
					break;
				case IF_TYPE_IEEE1394:
					printf("IEEE 1394 Firewire\n");
					break;
				default:
					printf("Unknown type %ld\n", pIfRow->dwType);
					break;
				}
				printf("\tMtu[%d]:\t\t %ld\n", i, pIfRow->dwMtu);
				printf("\tSpeed[%d]:\t %ld\n", i, pIfRow->dwSpeed);
				printf("\tPhysical Addr:\t ");
				if (pIfRow->dwPhysAddrLen == 0)
					printf("\n");
				for (j = 0; j < pIfRow->dwPhysAddrLen; j++) {
					if (j == (pIfRow->dwPhysAddrLen - 1))
						printf("%.2X\n", (int)pIfRow->bPhysAddr[j]);
					else
						printf("%.2X-", (int)pIfRow->bPhysAddr[j]);
				}
				printf("\tAdmin Status[%d]:\t %ld\n", i, pIfRow->dwAdminStatus);
				printf("\tOper Status[%d]:\t ", i);
				switch (pIfRow->dwOperStatus) {
				case IF_OPER_STATUS_NON_OPERATIONAL:
					printf("Non Operational\n");
					break;
				case IF_OPER_STATUS_UNREACHABLE:
					printf("Unreachable\n");
					break;
				case IF_OPER_STATUS_DISCONNECTED:
					printf("Disconnected\n");
					break;
				case IF_OPER_STATUS_CONNECTING:
					printf("Connecting\n");
					break;
				case IF_OPER_STATUS_CONNECTED:
					printf("Connected\n");
					break;
				case IF_OPER_STATUS_OPERATIONAL:
					printf("Operational\n");
					break;
				default:
					printf("Unknown status %ld\n", pIfRow->dwAdminStatus);
					break;
				}
				printf("\n");
			}
	}
	else {
		printf("GetIfTable failed with error: %d\n", dwRetVal);
		if (pIfTable != NULL) {
			FREE(pIfTable);
			pIfTable = NULL;
		}
		return 1;
		// Here you can use FormatMessage to find out why
		// it failed.
	}
	if (pIfTable != NULL) {
		FREE(pIfTable);
		pIfTable = NULL;
	}
	return dwOutOctets;
}
extern __declspec(dllexport) void __stdcall WriteActiveNicsList()
{
	printf("\n__________WriteActiveNicsList\n");
	std::wifstream ipv4enablednicslist("ipv4enablednicslist.txt");
	std::wofstream activenicslist("activenicslist.txt");
	// Declare and initialize variables.
	DWORD dwOutOctets = 0;
	DWORD dwSize = 0;
	DWORD dwRetVal = 0;

	unsigned int i, j;

	/* variables used for GetIfTable and GetIfEntry */
	MIB_IFTABLE* pIfTable;
	MIB_IFROW* pIfRow;

	// Allocate memory for our pointers.
	pIfTable = (MIB_IFTABLE*)MALLOC(sizeof(MIB_IFTABLE));
	if (pIfTable == NULL) {
		printf("Error allocating memory needed to call GetIfTable\n");

	}
	// Make an initial call to GetIfTable to get the
	// necessary size into dwSize
	dwSize = sizeof(MIB_IFTABLE);
	if (GetIfTable(pIfTable, &dwSize, FALSE) == ERROR_INSUFFICIENT_BUFFER) {
		FREE(pIfTable);
		pIfTable = (MIB_IFTABLE*)MALLOC(dwSize);
		if (pIfTable == NULL) {
			printf("Error allocating memory needed to call GetIfTable\n");

		}
	}
	// Make a second call to GetIfTable to get the actual
	// data we want.
	if ((dwRetVal = GetIfTable(pIfTable, &dwSize, FALSE)) == NO_ERROR) {
		printf("\tNum Entries: %ld\n\n", pIfTable->dwNumEntries);
		std::wstring line;
		while (std::getline(ipv4enablednicslist, line))
			for (i = 0; i < pIfTable->dwNumEntries; i++) {
				pIfRow = (MIB_IFROW*)&pIfTable->table[i];

				if (line != std::wstring(pIfRow->wszName)) {
					continue;
				}
				wchar_t description_array[100];
				wchar_t* description = &description_array[0];
				for (j = 0; j < pIfRow->dwDescrLen; j++)
					description_array[j] = pIfRow->bDescr[j];

				switch (pIfRow->dwOperStatus) {
				case IF_OPER_STATUS_OPERATIONAL:
					printf("Operational\n");

					activenicslist << description << std::endl;
					activenicslist << L"Interface ID: " << pIfRow->wszName << std::endl;
					activenicslist << L"Oper Status: Operational" << std::endl;
					activenicslist << std::endl;
					break;
				}
				printf("\n");
			}
	}
	else {
		printf("GetIfTable failed with error: %d\n", dwRetVal);
		if (pIfTable != NULL) {
			FREE(pIfTable);
			pIfTable = NULL;
		}

		// Here you can use FormatMessage to find out why
		// it failed.
	}
	if (pIfTable != NULL) {
		FREE(pIfTable);
		pIfTable = NULL;
	}
	activenicslist.close();
}
extern __declspec(dllexport) void __stdcall WriteActiveNicsListLog()
{
	printf("\n__________WriteActiveNicsListLog\n");
	std::wofstream outlog("debug.txt", std::ios::app);
	outlog << std::endl;
	outlog << "__________WriteActiveNicsListLog" << std::endl;
	std::wifstream ipv4enablednicslist("ipv4enablednicslist.txt");
	std::wofstream activenicslist("activenicslist.txt");
	// Declare and initialize variables.
	DWORD dwOutOctets = 0;
	DWORD dwSize = 0;
	DWORD dwRetVal = 0;

	unsigned int i, j;

	/* variables used for GetIfTable and GetIfEntry */
	MIB_IFTABLE* pIfTable;
	MIB_IFROW* pIfRow;

	// Allocate memory for our pointers.
	pIfTable = (MIB_IFTABLE*)MALLOC(sizeof(MIB_IFTABLE));
	if (pIfTable == NULL) {
		printf("Error allocating memory needed to call GetIfTable\n");
	}
	// Make an initial call to GetIfTable to get the
	// necessary size into dwSize
	dwSize = sizeof(MIB_IFTABLE);
	if (GetIfTable(pIfTable, &dwSize, FALSE) == ERROR_INSUFFICIENT_BUFFER) {
		FREE(pIfTable);
		pIfTable = (MIB_IFTABLE*)MALLOC(dwSize);
		if (pIfTable == NULL) {
			printf("Error allocating memory needed to call GetIfTable\n");

		}
	}
	// Make a second call to GetIfTable to get the actual
	// data we want.
	if ((dwRetVal = GetIfTable(pIfTable, &dwSize, FALSE)) == NO_ERROR) {
		std::wstring line;
		while (std::getline(ipv4enablednicslist, line))
			for (i = 0; i < pIfTable->dwNumEntries; i++) {
				pIfRow = (MIB_IFROW*)&pIfTable->table[i];

				if (line != std::wstring(pIfRow->wszName)) {
					continue;
				}
				outlog << line << std::endl;
				outlog << L"Out Octets: " << pIfRow->dwOutOctets << std::endl;

				wchar_t description_array[100];
				wchar_t* description = &description_array[0];
				for (j = 0; j < pIfRow->dwDescrLen; j++)
					description_array[j] = pIfRow->bDescr[j];
				switch (pIfRow->dwOperStatus) {
				case IF_OPER_STATUS_OPERATIONAL:
					printf("Operational\n");
					activenicslist << description << std::endl;
					activenicslist << L"Interface ID: " << pIfRow->wszName << std::endl;
					activenicslist << L"Oper Status: Operational" << std::endl;
					activenicslist << std::endl;
					break;
				}
			}
	}
	else {
		printf("GetIfTable failed with error: %d\n", dwRetVal);
		if (pIfTable != NULL) {
			FREE(pIfTable);
			pIfTable = NULL;
		}

		// Here you can use FormatMessage to find out why
		// it failed.
	}
	if (pIfTable != NULL) {
		FREE(pIfTable);
		pIfTable = NULL;
	}
	activenicslist.close();
	outlog.close();
}
extern __declspec(dllexport) DWORD __stdcall GetOutOctetsLog()
{
	printf("\n__________GetOutOctetsLog\n");
	std::wofstream outlog("debug.txt", std::ios::app);
	outlog << std::endl;
	outlog << "__________GetOctetsLog" << std::endl;
	std::wifstream ipv4enablednicslist("ipv4enablednicslist.txt");
	// Declare and initialize variables.
	DWORD dwOutOctets = 0;
	DWORD dwSize = 0;
	DWORD dwRetVal = 0;

	unsigned int i, j;

	/* variables used for GetIfTable and GetIfEntry */
	MIB_IFTABLE* pIfTable;
	MIB_IFROW* pIfRow;

	// Allocate memory for our pointers.
	pIfTable = (MIB_IFTABLE*)MALLOC(sizeof(MIB_IFTABLE));
	if (pIfTable == NULL) {
		printf("Error allocating memory needed to call GetIfTable\n");
		return 1;
	}
	// Make an initial call to GetIfTable to get the
	// necessary size into dwSize
	dwSize = sizeof(MIB_IFTABLE);
	if (GetIfTable(pIfTable, &dwSize, FALSE) == ERROR_INSUFFICIENT_BUFFER) {
		FREE(pIfTable);
		pIfTable = (MIB_IFTABLE*)MALLOC(dwSize);
		if (pIfTable == NULL) {
			printf("Error allocating memory needed to call GetIfTable\n");
			return 1;
		}
	}
	// Make a second call to GetIfTable to get the actual
	// data we want.
	if ((dwRetVal = GetIfTable(pIfTable, &dwSize, FALSE)) == NO_ERROR) {
		std::wstring line;
		while (std::getline(ipv4enablednicslist, line))
			for (i = 0; i < pIfTable->dwNumEntries; i++) {
				pIfRow = (MIB_IFROW*)&pIfTable->table[i];

				if (line != std::wstring(pIfRow->wszName)) {
					continue;
				}

				dwOutOctets += pIfRow->dwOutOctets;
				outlog << line << std::endl;
				outlog << L"Out Octets: " << pIfRow->dwOutOctets << std::endl;
			}
	}
	else {
		printf("GetIfTable failed with error: %d\n", dwRetVal);
		if (pIfTable != NULL) {
			FREE(pIfTable);
			pIfTable = NULL;
		}
		return 1;
		// Here you can use FormatMessage to find out why
		// it failed.
	}
	if (pIfTable != NULL) {
		FREE(pIfTable);
		pIfTable = NULL;
	}
	outlog.close();
	return dwOutOctets;
}
extern __declspec(dllexport) DWORD __stdcall GetInOctets()
{
	printf("\n__________GetInOctets\n");
	std::wifstream ipv4enablednicslist("ipv4enablednicslist.txt");

	// Declare and initialize variables.
	DWORD dwInOctets = 0;
	DWORD dwSize = 0;
	DWORD dwRetVal = 0;

	unsigned int i, j;

	/* variables used for GetIfTable and GetIfEntry */
	MIB_IFTABLE* pIfTable;
	MIB_IFROW* pIfRow;

	// Allocate memory for our pointers.
	pIfTable = (MIB_IFTABLE*)MALLOC(sizeof(MIB_IFTABLE));
	if (pIfTable == NULL) {
		printf("Error allocating memory needed to call GetIfTable\n");
		return 1;
	}
	// Make an initial call to GetIfTable to get the
	// necessary size into dwSize
	dwSize = sizeof(MIB_IFTABLE);
	if (GetIfTable(pIfTable, &dwSize, FALSE) == ERROR_INSUFFICIENT_BUFFER) {
		FREE(pIfTable);
		pIfTable = (MIB_IFTABLE*)MALLOC(dwSize);
		if (pIfTable == NULL) {
			printf("Error allocating memory needed to call GetIfTable\n");
			return 1;
		}
	}
	// Make a second call to GetIfTable to get the actual
	// data we want.
	if ((dwRetVal = GetIfTable(pIfTable, &dwSize, FALSE)) == NO_ERROR) {
		printf("\tNum Entries: %ld\n\n", pIfTable->dwNumEntries);
		std::wstring line;
		while (std::getline(ipv4enablednicslist, line))
			for (i = 0; i < pIfTable->dwNumEntries; i++) {
				pIfRow = (MIB_IFROW*)&pIfTable->table[i];
				if (line != std::wstring(pIfRow->wszName)) {
					continue;
				}
				printf("\tIndex[%d]:\t %ld\n", i, pIfRow->dwIndex);
				wprintf(L"\tInterfaceName[%d]:\t %s", i, pIfRow->wszName);
				printf("\n");
				dwInOctets += pIfRow->dwInOctets;
				//break; // exiting the loop
				printf("\tDescription[%d]:\t ", i);
				for (j = 0; j < pIfRow->dwDescrLen; j++)
					printf("%c", pIfRow->bDescr[j]);
				printf("\n");
				printf("\tType[%d]:\t ", i);
				switch (pIfRow->dwType) {
				case IF_TYPE_OTHER:
					printf("Other\n");
					break;
				case IF_TYPE_ETHERNET_CSMACD:
					printf("Ethernet\n");
					break;
				case IF_TYPE_ISO88025_TOKENRING:
					printf("Token Ring\n");
					break;
				case IF_TYPE_PPP:
					printf("PPP\n");
					break;
				case IF_TYPE_SOFTWARE_LOOPBACK:
					printf("Software Lookback\n");
					break;
				case IF_TYPE_ATM:
					printf("ATM\n");
					break;
				case IF_TYPE_IEEE80211:
					printf("IEEE 802.11 Wireless\n");
					break;
				case IF_TYPE_TUNNEL:
					printf("Tunnel type encapsulation\n");
					break;
				case IF_TYPE_IEEE1394:
					printf("IEEE 1394 Firewire\n");
					break;
				default:
					printf("Unknown type %ld\n", pIfRow->dwType);
					break;
				}
				printf("\tMtu[%d]:\t\t %ld\n", i, pIfRow->dwMtu);
				printf("\tSpeed[%d]:\t %ld\n", i, pIfRow->dwSpeed);
				printf("\tPhysical Addr:\t ");
				if (pIfRow->dwPhysAddrLen == 0)
					printf("\n");
				for (j = 0; j < pIfRow->dwPhysAddrLen; j++) {
					if (j == (pIfRow->dwPhysAddrLen - 1))
						printf("%.2X\n", (int)pIfRow->bPhysAddr[j]);
					else
						printf("%.2X-", (int)pIfRow->bPhysAddr[j]);
				}
				printf("\tAdmin Status[%d]:\t %ld\n", i, pIfRow->dwAdminStatus);
				printf("\tOper Status[%d]:\t ", i);
				switch (pIfRow->dwOperStatus) {
				case IF_OPER_STATUS_NON_OPERATIONAL:
					printf("Non Operational\n");
					break;
				case IF_OPER_STATUS_UNREACHABLE:
					printf("Unreachable\n");
					break;
				case IF_OPER_STATUS_DISCONNECTED:
					printf("Disconnected\n");
					break;
				case IF_OPER_STATUS_CONNECTING:
					printf("Connecting\n");
					break;
				case IF_OPER_STATUS_CONNECTED:
					printf("Connected\n");
					break;
				case IF_OPER_STATUS_OPERATIONAL:
					printf("Operational\n");
					break;
				default:
					printf("Unknown status %ld\n", pIfRow->dwAdminStatus);
					break;
				}
				printf("\n");
			}
	}
	else {
		printf("GetIfTable failed with error: %d \n", dwRetVal);
		if (pIfTable != NULL) {
			FREE(pIfTable);
			pIfTable = NULL;
		}
		return 1;
		// Here you can use FormatMessage to find out why
		// it failed.
	}
	if (pIfTable != NULL) {
		FREE(pIfTable);
		pIfTable = NULL;
	}
	return dwInOctets;
}
extern __declspec(dllexport) DWORD __stdcall GetInOctetsLog()
{
	printf("\n__________GetInOctetsLog\n");
	std::wofstream outlog("debug.txt", std::ios::app);
	outlog << std::endl;
	outlog << "----------GetInOctetsLog" << std::endl;
	std::wifstream ipv4enablednicslist("ipv4enablednicslist.txt");
	//outlog << L"Nic Name: " << nicName << std::endl;

	// Declare and initialize variables.
	DWORD dwInOctets = 0;
	DWORD dwSize = 0;
	DWORD dwRetVal = 0;

	unsigned int i, j;

	/* variables used for GetIfTable and GetIfEntry */
	MIB_IFTABLE* pIfTable;
	MIB_IFROW* pIfRow;

	// Allocate memory for our pointers.
	pIfTable = (MIB_IFTABLE*)MALLOC(sizeof(MIB_IFTABLE));
	if (pIfTable == NULL) {
		printf("Error allocating memory needed to call GetIfTable\n");
		return 1;
	}
	// Make an initial call to GetIfTable to get the
	// necessary size into dwSize
	dwSize = sizeof(MIB_IFTABLE);
	if (GetIfTable(pIfTable, &dwSize, FALSE) == ERROR_INSUFFICIENT_BUFFER) {
		FREE(pIfTable);
		pIfTable = (MIB_IFTABLE*)MALLOC(dwSize);
		if (pIfTable == NULL) {
			printf("Error allocating memory needed to call GetIfTable\n");
			return 1;
		}
	}
	// Make a second call to GetIfTable to get the actual
	// data we want.
	if ((dwRetVal = GetIfTable(pIfTable, &dwSize, FALSE)) == NO_ERROR) {
		std::wstring line;
		while (std::getline(ipv4enablednicslist, line))
			for (i = 0; i < pIfTable->dwNumEntries; i++) {
				pIfRow = (MIB_IFROW*)&pIfTable->table[i];

				if (line != std::wstring(pIfRow->wszName)) {
					continue;
				}

				dwInOctets += pIfRow->dwInOctets;
				outlog << line << std::endl;
				outlog << L"In Octets: " << pIfRow->dwInOctets << std::endl;
			}
	}
	else {
		printf("GetIfTable failed with error: %d\n", dwRetVal);
		if (pIfTable != NULL) {
			FREE(pIfTable);
			pIfTable = NULL;
		}
		return 1;
		// Here you can use FormatMessage to find out why
		// it failed.
	}
	if (pIfTable != NULL) {
		FREE(pIfTable);
		pIfTable = NULL;
	}
	outlog.close();
	return dwInOctets;
}

