#pragma semicolon 1
#pragma newdecls required
#include <sourcemod>
#include <sourcescramble>

native Address TestAddress(int bits);
native int ReadCellAddress(Address address);
native int ReadCharAddress(Address address);

public int main() {
	int cell = 123456;
	char text[] = "SourceScramble";
	if (ReadCellAddress(GetAddressOfCell(cell)) != cell) return 1;
	if (ReadCharAddress(GetAddressOfString(text)) != 'S') return 2;
	Address high = view_as<Address>(2147483647) + 1;
	if (TestAddress(-2147483647 - 1) != high) return 3;
	if (TestAddress(-1) != high + view_as<Address>(2147483647)) return 4;
	if (TestAddress(0) != Address_Null) return 5;
	return 0;
}
