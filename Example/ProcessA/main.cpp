#include <stdio.h>
#include <process.h>

#include "../ProcessCommon/IPC_MemoryMappedFile.h"

void main()
{
    char* pString = "hello world";
    IPC_MemoryMappedFile oMemoryMappedFileA;
    oMemoryMappedFileA.createShareMemoryWithPage(1024 * 1024, "sunjj");
    oMemoryMappedFileA.writeData(pString, 12);

    
    printf("%s\n", "Hello world___A");
    printf("%s\n", pString);

    getchar();

    char* pStrings = new char[100];
    oMemoryMappedFileA.readData(pStrings, 5);
    printf("%s\n", "Hello world___A");

    printf("%s\n", pStrings);
    system("pause");
}