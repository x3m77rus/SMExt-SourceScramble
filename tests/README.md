# Address ABI regression tests

These tests bind the actual `native_utils.cpp` functions into an x86 SourcePawn
VM. They check cell/string reference addresses, hidden 64-bit result arguments,
zero extension of addresses with bit 31 set, null results, and legacy single-cell
results. `TestAddress(int)` also exercises argument shifting on an address-returning
native with one cell argument, as used by the handle property getters.

From the workspace root in PowerShell:

```powershell
$compiler = 'C:\src\servers\srcds_test\addons\sourcemod\scripting\oldspcomp64.exe'
$includes = 'C:\src\servers\srcds_test\addons\sourcemod\scripting\include'
New-Item -ItemType Directory -Force output/SMExt-SourceScramble/release | Out-Null
& $compiler extensions/SMExt-SourceScramble/tests/address-wide.sp "-i$includes" -iextensions/SMExt-SourceScramble/scripting/include -E -o output/SMExt-SourceScramble/release/address-wide.test.smx
& $compiler extensions/SMExt-SourceScramble/tests/address-legacy.sp -p extensions/SMExt-SourceScramble/tests/legacy-prefix.inc -E -o output/SMExt-SourceScramble/release/address-legacy.test.smx
docker run --rm --platform linux/amd64 -v "${PWD}:/workspace" sourcemod-ext-x86 bash /workspace/extensions/SMExt-SourceScramble/tests/run-address-tests-linux.sh
```

The runner links against the matching SourcePawn VM static libraries already
cached in `output/toolchains/sourcepawn13-x86/`. A different directory inside
the container can be selected with `SOURCEPAWN_TEST_TOOLS`. These optional tests
need those libraries; the normal extension build does not. The tests do not
download or build a compiler, require a separate Docker image, or touch server
memory. Test SMX files and executables stay outside the installable package.
