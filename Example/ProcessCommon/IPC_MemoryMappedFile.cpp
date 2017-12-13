#include "IPC_MemoryMappedFile.h"
#include <assert.h>

// 创建文件内核对象
// CreateFile
// 
// 创建文件映射内核对象
// CreateFileMapping
// 
// 创建文件映射视图
// MapViewOfFile()
// 
// 撤销文件映射视图
// UnmapViewOfFile()
// 
// 强制保存文件映射视图
// FlushViewOfFile()
//
// 关闭内核句柄
// CloseHandle()

IPC_MemoryMappedFile::IPC_MemoryMappedFile()
    :m_pWriteData(nullptr), m_pReadData(nullptr), m_pBaseData(nullptr),
     m_FileHandle(nullptr), m_MappingFileHandle(nullptr), m_nShareMemorySize(0)
{

}

IPC_MemoryMappedFile::~IPC_MemoryMappedFile()
{
    closeShareMemory();
}

bool IPC_MemoryMappedFile::createShareMemoryWithFile(unsigned int nShareMemorySize, const char* pFileName, const char* pShareMemoryName)
{
    m_FileHandle = CreateFile(pFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 
        nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

    if (m_FileHandle == INVALID_HANDLE_VALUE)
    {
        assert(false);
        return false;
    }

    m_MappingFileHandle = CreateFileMapping(m_FileHandle, nullptr, PAGE_READWRITE, 0, nShareMemorySize, pShareMemoryName);
    if (m_MappingFileHandle == nullptr)
    {
        assert(false);
        return false;
    }

    m_pBaseData = (char*)MapViewOfFile(m_MappingFileHandle, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
    if (m_pBaseData = nullptr)
    {
        CloseHandle(m_MappingFileHandle);
        m_MappingFileHandle = nullptr;

        assert(false);
        return false;
    }

    memset(m_pBaseData, nShareMemorySize, 0);
    m_nShareMemorySize = nShareMemorySize;
    m_pReadData = m_pBaseData;
    m_pWriteData = m_pBaseData;

    return true;
}

bool IPC_MemoryMappedFile::createShareMemoryWithPage(unsigned int nShareMemorySize, const char* pShareMemoryName)
{
    m_MappingFileHandle = CreateFileMapping(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 0, nShareMemorySize, pShareMemoryName);
    if (m_MappingFileHandle == nullptr)
    {
        assert(false);
        return false;
    }

    m_pBaseData = (char*)MapViewOfFile(m_MappingFileHandle, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
    if (m_pBaseData == nullptr)
    {
        CloseHandle(m_MappingFileHandle);
        m_MappingFileHandle = nullptr;

        assert(false);
        return false;
    }

    m_nShareMemorySize = nShareMemorySize;
    m_pReadData = m_pBaseData;
    m_pWriteData = m_pBaseData;
    return true;
}

bool IPC_MemoryMappedFile::readData(void* pData, unsigned int nDataSize)
{
    if ((m_pReadData + nDataSize) <= m_pBaseData + m_nShareMemorySize)
    {
        memmove(pData, m_pReadData, nDataSize);
        memset(m_pReadData, 0, nDataSize);
        //m_pReadData += nDataSize;
        return true;
    }

    return false;
}

bool IPC_MemoryMappedFile::writeData(void* pData, unsigned int nDataSize)
{
    if (pData == nullptr)
    {
        assert(false);
        return false;
    }

    if ((m_pWriteData + nDataSize) <= (m_pBaseData + m_nShareMemorySize))
    {
        memmove(m_pWriteData, pData, nDataSize);
        FlushViewOfFile(m_pWriteData, nDataSize);
        //m_pWriteData += nDataSize;
        return true;
    }

    return false;
}

bool IPC_MemoryMappedFile::openShareMemory(unsigned int nShareMemorySize, const char* pShareMemoryName)
{
    m_MappingFileHandle = OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE, false, pShareMemoryName);
    if (m_MappingFileHandle == nullptr)
    {
        assert(false);
        return false;
    }

    m_pBaseData = (char*)MapViewOfFile(m_MappingFileHandle, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
    if (m_pBaseData == nullptr)
    {
        CloseHandle(m_MappingFileHandle);
        m_MappingFileHandle = nullptr;

        assert(false);
        return true;
    }

    m_nShareMemorySize = nShareMemorySize;
    m_pReadData = m_pBaseData;
    m_pWriteData = m_pBaseData;
    return true;
}

void IPC_MemoryMappedFile::closeShareMemory()
{
    if (m_pBaseData)
    {
        UnmapViewOfFile(m_pBaseData);
        m_pBaseData = nullptr;
    }

    if (m_FileHandle)
    {
        CloseHandle(m_FileHandle);
        m_FileHandle = nullptr;
    }

    if (m_MappingFileHandle)
    {
        CloseHandle(m_MappingFileHandle);
        m_MappingFileHandle = nullptr;
    }

    m_pReadData = nullptr;
    m_pWriteData = nullptr;
    m_nShareMemorySize = 0;
}

void IPC_MemoryMappedFile::unMapShareMemory()
{
    if (m_pBaseData)
    {
        UnmapViewOfFile(m_pBaseData);
        m_pBaseData = nullptr;
    }
}
