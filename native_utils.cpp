#include "extension.h"
#include "native_utils.h"
#include <cstring>

bool UsesWideAddress(IPluginContext *pContext) {
	return pContext->GetRuntime()->FindPubvarByName("__Int64_Address__", nullptr) == SP_ERROR_NONE;
}

cell_t ReturnNativeAddress(IPluginContext *pContext, const cell_t *params, uintptr_t address) {
	if (UsesWideAddress(pContext)) {
		cell_t *result;
		if (int err = pContext->LocalToPhysAddr(params[1], &result); err != SP_ERROR_NONE) {
			return pContext->ThrowNativeErrorEx(err, "Could not access Address return value");
		}
		// Zero-extend x86 addresses, including pointers with bit 31 set.
		uint64_t value = address;
		std::memcpy(result, &value, sizeof(value));
		return 0;
	}
	return static_cast<cell_t>(address);
}

cell_t sm_CellRefToAddress(IPluginContext *pContext, const cell_t *params) {
#ifndef PLATFORM_64BITS
	cell_t* value;
	if (pContext->LocalToPhysAddr(params[1 + UsesWideAddress(pContext)], &value) != SP_ERROR_NONE) {
		return pContext->ThrowNativeError("Failed to convert cell reference to address.");
	}
	return ReturnNativeAddress(pContext, params, reinterpret_cast<uintptr_t>(value));
#else
	return pContext->ThrowNativeError("GetAddressOfCell is not implemented for 64-bit platforms");
#endif

}

cell_t sm_CharArrayToAddress(IPluginContext *pContext, const cell_t *params) {
#ifndef PLATFORM_64BITS
	char* buffer;
	if (pContext->LocalToString(params[1 + UsesWideAddress(pContext)], &buffer) != SP_ERROR_NONE) {
		return pContext->ThrowNativeError("Failed to convert string reference to address.");
	}
	return ReturnNativeAddress(pContext, params, reinterpret_cast<uintptr_t>(buffer));
#else
	return pContext->ThrowNativeError("GetAddressOfString is not implemented for 64-bit platforms");
#endif
}

const sp_nativeinfo_t g_UtilNatives[] = {
	{ "GetAddressOfCell", sm_CellRefToAddress },
	{ "GetAddressOfString", sm_CharArrayToAddress },
	{ nullptr, nullptr },
};
