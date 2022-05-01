#include "PeParse.h"
#include <iostream>
#include <string>
#include <fstream>
#include <cmath>

using namespace std;

PeParse::PeParse()
{
	initClass();
}

PeParse::~PeParse() {

	CloseHandle(hFile);
	CloseHandle(hFileMapping);
	UnmapViewOfFile(lpFileBase);

}

void PeParse::initClass()
{
	dosHeader = 0;
	pNTHeader = 0;
	file_name = 0;
	ico_name = 0;
	hFile = INVALID_HANDLE_VALUE;
	lpFileBase = 0;
	importStartRVA = 0;
	importEndRVA = 0;
	importDescriptor = 0;
	imageBase = 0;

}

bool PeParse::isPeFile()
{
	bool status = false;
	if (dosHeader && dosHeader->e_magic == IMAGE_DOS_SIGNATURE && pNTHeader && pNTHeader->Signature == IMAGE_NT_SIGNATURE)
	{
		status = true;
	}
	return status;
}
bool PeParse::openFileHandle()
{
	if (file_name) {
		hFile = CreateFile(file_name, GENERIC_READ, FILE_SHARE_READ, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	}
	return (hFile != INVALID_HANDLE_VALUE);
}

bool PeParse::fileCreatingRoutine() {
	bool status_handle;
	status_handle = openFileHandle();
	if (!status_handle) return false;

	hFileMapping = CreateFileMapping(hFile, nullptr, PAGE_READONLY | SEC_IMAGE, 0, 0, nullptr);
	if (hFileMapping == 0)
	{
		CloseHandle(hFile);
		cout << "Failed: CreateFileMapping";
		return false;
	}
	lpFileBase = MapViewOfFile(hFileMapping, FILE_MAP_READ, 0, 0, 0);
	if (!lpFileBase)
	{
		CloseHandle(hFile);
		CloseHandle(hFileMapping);
		UnmapViewOfFile(lpFileBase);
		cout << "Failed: MapViewOfFile";
		return false;
	}
	return true;
}

PeParse::PeParse(LPCWSTR file)
{
	initClass();
	file_name = file;
	if (file_name)
	{
	if (!fileCreatingRoutine()) { cout << "Failed: fileCreatingRoutine"; return; }
		initPEHeaders();
		if (!isPeFile()) { cout << "Failed: isPeFile"; return; }

	}
}
void PeParse::initPEHeaders() {
	dosHeader = (PIMAGE_DOS_HEADER)lpFileBase;
	pNTHeader = PIMAGE_NT_HEADERS(ULONG_PTR(lpFileBase) + dosHeader->e_lfanew);

	imageImportData = pNTHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
	importStartRVA = imageImportData.VirtualAddress;
	importEndRVA = imageImportData.Size;
	importDescriptor = PIMAGE_IMPORT_DESCRIPTOR(ULONG_PTR(lpFileBase) + importStartRVA);

	imageBase = pNTHeader->OptionalHeader.ImageBase;

}
void PeParse::dumpIt()
{
	if (!importStartRVA || !importEndRVA) {
		cout << "No Import directory!" << endl;
	}
	int counterW = 0;
	//print import dll
	for (; importDescriptor->FirstThunk; importDescriptor++)
	{
		char* str = (char*)lpFileBase + importDescriptor->Name;
		cout << "	" << str << endl;
		int length = strlen(str);
		//counting 'w'
		for (int i = 0; i < length; ++i)
		{
			if (str[i] == 'w')
			{
				counterW++;
				break;
			}
		}
	}

	cout << endl << " [*] Counter of dll's with letter 'w' :	" << counterW << endl;

}

int main()
{

	wstring path;
	wstring path_ico;

	cout << " Enter path to .exe:	";
	wcin >> path;

	PeParse peFile(path.c_str());
	cout << endl << " [-] Import table dll's [-] " << endl << endl;
	peFile. dumpIt();

	system("pause");

}
