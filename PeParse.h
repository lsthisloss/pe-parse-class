#pragma once
#include <windows.h>



class PeParse {
public:
	PeParse(LPCWSTR file);
	~PeParse();
	void initClass();
	void initPEHeaders();
	void dumpIt();

	bool isPeFile();
	bool fileCreatingRoutine();
	bool openFileHandle();


protected:
	PeParse();
	LPCWSTR file_name;
	LPCWSTR ico_name;


	PIMAGE_DOS_HEADER dosHeader;
	PIMAGE_NT_HEADERS pNTHeader;

	DWORD importStartRVA, importEndRVA, imageBase;
	HANDLE hFile;
	HANDLE hFileMapping;
	LPVOID lpFileBase;

	IMAGE_DATA_DIRECTORY imageImportData;
	PIMAGE_IMPORT_DESCRIPTOR importDescriptor;


};
