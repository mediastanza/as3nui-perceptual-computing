#include "FlashRuntimeExtensions.h"

extern "C"
{
	__declspec(dllexport) void AirPcInitializer(void** extData, FREContextInitializer* ctxInitializer, FREContextFinalizer* ctxFinalizer);
	__declspec(dllexport) void AirPcFinalizer(void* extData);

	FREObject Pc_start(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
	FREObject Pc_stop(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
	FREObject Pc_getGeoNode(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
}