#!/usr/bin/env bash
set -euo pipefail
source_dir=/workspace/extensions/SMExt-SourceScramble
sp=/workspace/sdk/sourcemod/sourcepawn
tools="${SOURCEPAWN_TEST_TOOLS:-/workspace/output/toolchains/sourcepawn13-x86}"
out=/workspace/output/SMExt-SourceScramble/release
test -f "$tools/libsourcepawn_static/linux-x86/liblibsourcepawn_static.a"
clang++-20 -m32 -std=c++20 -DNDEBUG -D_LINUX -DLINUX -DPOSIX -DMETA_NO_HL2SDK \
    -DSP_JIT_V1 -DSP_JIT_V2 \
    -I"$source_dir" -I/workspace/sdk/sourcemod/public \
    -I/workspace/sdk/sourcemod/public/extensions \
    -I/workspace/sdk/sourcemod/public/amtl -I/workspace/sdk/sourcemod/public/amtl/amtl \
    -I/workspace/sdk/metamod-source/core -I/workspace/sdk/metamod-source/core/sourcehook \
    -I"$sp" -I"$sp/include" -I"$sp/vm" -I"$sp/third_party" -I"$sp/third_party/amtl" \
    -I"$sp/third_party/mimalloc/include" -I"$sp/third_party/capstone/include" -I"$tools/includes" \
    "$source_dir/tests/address-native.cpp" "$source_dir/native_utils.cpp" \
    -Wl,--start-group "$tools/libsourcepawn_static/linux-x86/liblibsourcepawn_static.a" \
    "$tools/third_party/zlib/z/linux-x86/libz.a" \
    "$tools/third_party/amtl/amtl/libamtl/linux-x86/liblibamtl.a" \
    "$tools/third_party/mimalloc/linux-x86/libmimalloc.a" \
    "$tools/third_party/capstone/linux-x86/libcapstone.a" -Wl,--end-group \
    -static-libstdc++ -static-libgcc -lm -lpthread -lrt -ldl -o "$out/address-native-test"
"$out/address-native-test" "$out/address-wide.test.smx" "$out/address-legacy.test.smx"
