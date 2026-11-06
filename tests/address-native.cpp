#include "extension.h"
#include "native_utils.h"
#include "environment.h"
#include "base-runtime.h"
#include <cstdio>
#include <cstring>
#include <memory>

static cell_t TestAddress(IPluginContext* context, const cell_t* params) {
    auto bits = static_cast<uint32_t>(params[1 + UsesWideAddress(context)]);
    return ReturnNativeAddress(context, params, static_cast<uintptr_t>(bits));
}

static uintptr_t ReadAddress(IPluginContext* context, const cell_t* params) {
    if (!UsesWideAddress(context)) return static_cast<uintptr_t>(params[1]);
    cell_t* value;
    if (context->LocalToPhysAddr(params[1], &value) != SP_ERROR_NONE) return 0;
    uint64_t address;
    std::memcpy(&address, value, sizeof(address));
    return static_cast<uintptr_t>(address);
}

static cell_t ReadCellAddress(IPluginContext* context, const cell_t* params) {
    return *reinterpret_cast<cell_t*>(ReadAddress(context, params));
}
static cell_t ReadCharAddress(IPluginContext* context, const cell_t* params) {
    return *reinterpret_cast<unsigned char*>(ReadAddress(context, params));
}

int main(int argc, char** argv) {
    static_assert(sizeof(void*) == 4, "Run address ABI tests on x86");
    auto env = sp::Environment::New();
    if (!env) return 1;
    int status = 0;
    for (int arg = 1; arg < argc; ++arg) {
        SourcePawn::ExceptionHandler errors(env);
        std::unique_ptr<sp::BaseRuntime> runtime(env->LoadBinaryFromFile(argv[arg]));
        if (!runtime) { status = 1; break; }
        runtime->InstallBuiltinNatives();
        const sp_nativeinfo_t natives[] = {
            {"GetAddressOfCell", sm_CellRefToAddress},
            {"GetAddressOfString", sm_CharArrayToAddress},
            {"TestAddress", TestAddress},
            {"ReadCellAddress", ReadCellAddress},
            {"ReadCharAddress", ReadCharAddress},
        };
        for (const auto& native : natives) {
            uint32_t index;
            if (runtime->FindNativeByName(native.name, &index) == SP_ERROR_NONE)
                runtime->UpdateNativeBinding(index, native.func, 0, nullptr);
        }
        auto main = runtime->GetFunctionByName("main");
        cell_t result = -1;
        if (!main || !main->Invoke(&result) || result != 0) {
            std::fprintf(stderr, "%s failed (%d): %s\n", argv[arg], result,
                         errors.HasException() ? errors.Message() : "unexpected address");
            status = 1;
            break;
        }
        std::printf("Address native ABI passed: %s\n", argv[arg]);
    }
    env->Shutdown();
    delete env;
    return status;
}
