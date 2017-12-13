#include <stdio.h>
#include <process.h>

#include "../ProcessCommon/IPC_MemoryMappedFile.h"

void main()
{
    char* pString = new char;
    IPC_MemoryMappedFile oMemoryMappedFileA;
    oMemoryMappedFileA.openShareMemory(1024 * 1024, "sunjj");
    oMemoryMappedFileA.readData(pString, 7);

    printf("%s\n", "Hello world___B");
    printf("%s\n", pString);
    getchar();
    system("pause");
}