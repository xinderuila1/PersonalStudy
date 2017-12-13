#ifndef IPCMEMORYMAPPEDFILE_H
#define IPCMEMORYMAPPEDFILE_H

//////////////////////////////////////////////////////////////////////////
//�ڴ�ӳ���ļ���װ��
#include <Windows.h>
#include "ExportDLL.h"

class MYDLL_API IPC_MemoryMappedFile
{
public:
    
    IPC_MemoryMappedFile();
    ~IPC_MemoryMappedFile();

    //����Ӳ���ļ����ڴ�ӳ���ļ�
    bool createShareMemoryWithFile(unsigned int nShareMemorySize, const char* pFileName, const char* pShareMemoryName);

    //����ҳ�ļ����ڴ�ӳ���ļ�
    bool createShareMemoryWithPage(unsigned int nShareMemorySize, const char* pShareMemoryName);

    //������
    bool readData(void* pData, unsigned int nDataSize);

    //д����
    bool writeData(void* pData, unsigned int nDataSize);

    //���ڴ�ӳ���ļ�
    bool openShareMemory(unsigned int nShareMemorySize, const char* pShareMemoryName);

    //�ر��ڴ�ӳ���ļ�
    void closeShareMemory();

    //�����ڴ�ӳ���ļ�
    void unMapShareMemory();

private:
    HANDLE m_FileHandle;
    HANDLE m_MappingFileHandle;

    char* m_pBaseData;
    char* m_pReadData;
    char* m_pWriteData;

    unsigned int m_nShareMemorySize; //�ļ���С���ֽ�
};
#endif