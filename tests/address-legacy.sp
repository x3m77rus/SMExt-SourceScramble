#pragma semicolon 1
#pragma newdecls required
// Model the pre-int64 SourceMod Address API without the new public marker.
enum Address { Address_Null = 0 };
native Address GetAddressOfCell(any& cell);
native Address GetAddressOfString(char[] text);
native Address TestAddress(int bits);
native int ReadCellAddress(Address address);
native int ReadCharAddress(Address address);

public int main() {
	int cell = 654321;
	char text[] = "SourceScramble";
	if (ReadCellAddress(GetAddressOfCell(cell)) != cell) return 1;
	if (ReadCharAddress(GetAddressOfString(text)) != 'S') return 2;
	if (TestAddress(-2147483647 - 1) != view_as<Address>(-2147483647 - 1)) return 3;
	if (TestAddress(-1) != view_as<Address>(-1)) return 4;
	if (TestAddress(0) != Address_Null) return 5;
	return 0;
}
