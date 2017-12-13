#ifndef IPCMEMORYMAPPEDFILE_H
#define IPCMEMORYMAPPEDFILE_H

//////////////////////////////////////////////////////////////////////////
//内存映射文件封装类
#include <Windows.h>
#include "ExportDLL.h"

class MYDLL_API IPC_MemoryMappedFile
{
public:
    
    IPC_MemoryMappedFile();
    ~IPC_MemoryMappedFile();

    //基于硬件文件的内存映射文件
    bool createShareMemoryWithFile(unsigned int nShareMemorySize, const char* pFileName, const char* pShareMemoryName);

    //基于页文件的内存映射文件
    bool createShareMemoryWithPage(unsigned int nShareMemorySize, const char* pShareMemoryName);

    //读数据
    bool readData(void* pData, unsigned int nDataSize);

    //写数据
    bool writeData(void* pData, unsigned int nDataSize);

    //打开内存映射文件
    bool openShareMemory(unsigned int nShareMemorySize, const char* pShareMemoryName);

    //关闭内存映射文件
    void closeShareMemory();

    //撤销内存映射文件
    void unMapShareMemory();

private:
    HANDLE m_FileHandle;
    HANDLE m_MappingFileHandle;

    char* m_pBaseData;
    char* m_pReadData;
    char* m_pWriteData;

    unsigned int m_nShareMemorySize; //文件大小，字节
};
#endif