#pragma once

cell_t sm_CellRefToAddress(IPluginContext *pContext, const cell_t *params);
cell_t sm_CharArrayToAddress(IPluginContext *pContext, const cell_t *params);

// SM 1.13 plugins pass a hidden result reference for 64-bit Address returns.
bool UsesWideAddress(IPluginContext *pContext);
cell_t ReturnNativeAddress(IPluginContext *pContext, const cell_t *params, uintptr_t address);

extern const sp_nativeinfo_t g_UtilNatives[];
