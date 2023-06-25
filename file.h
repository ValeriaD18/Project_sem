#ifndef FILE_H
#define FILE_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <winerror.h>

//---------------------------------------------------------------------------

// ���� ������

#define LAB_OK 0
#define LAB_ERR -1
#define LAB_BAD_BUF -2
#define LAB_NOT_FOUND -3
#define LAB_STOP -4
#define LAB_SERVER_DISCONNECTED_OK -5
#define LAB_BAD_FILE -6

// ����� ��� ������ ������� � ������ ������

#define LAB_FLAG_WSA 0
#define LAB_FLAG_GET 1
#define LAB_FLAG_ERRNO 2
#define LAB_FLAG_MY 3
#define LAB_FLAG_RESULT 4

// ���������

#define LAB_MAX_BUF_SIZE 128
#define LAB_MIN_BUF_LEN 1
#define LAB_MAX_COUNT_CLIENTS FD_SETSIZE
#define LAB_BUF_SIZE 64
#define LAB_HOST_NAME_SIZE 64

//---------------------------------------------------------------------------

// ��������� �������:

// ����� ��������� �� ������
void ErrorOutput(
        const char *chFuncFailMessage, // ��������� ���������
        int iErr, // ����� ������, ������������ ��������, ��� ��������� ������
        int iFlag // ����, ��� �������� ��������� �� ������
        );
        
//---------------------------------------------------------------------------

int ReleaseHandle(
        HANDLE *h // ����� 
        );

//---------------------------------------------------------------------------

void ReleaseVoidMemory(
        void **vMemory // ��������� �� ��������� �� ������ ������ ����
        );

//---------------------------------------------------------------------------

int LabOpenFile(
        const char *chFileName, // ��� �����
        HANDLE *hFile, // ��������� �� �����
        DWORD *dwFileSize // ��������� �� ���������� � �������� �����   
        );

//---------------------------------------------------------------------------

int LabReadFile(
        HANDLE hFile, // �����
        char **chFileContent, // ��������� �� ��������� �� ������� �����
        DWORD *dwFileSize // ��������� �� ���������� � �������� �����
        );

//---------------------------------------------------------------------------

int LabWriteFile(
        HANDLE hFile, // �����
        const char *chFileContent, // ��������� �� ������� �����
        DWORD *dwFileSize // ��������� �� ���������� � �������� �����
        );

//---------------------------------------------------------------------------

int LabCloseFile(
        HANDLE *hFile // ��������� �� �����
        );

//---------------------------------------------------------------------------

#endif // FILE_H