// ���������� ���� ����������
#include "file.h"

void ErrorOutput(const char *chFuncFailMessage, int iErr, int iFlag)
{
    DWORD n = 0; // ��� ������
    const DWORD size = LAB_MAX_BUF_SIZE; // ������������ ������ ��������� �� ������
    char buf[LAB_MAX_BUF_SIZE]; // ����� ���������

    switch ( iFlag ) // ��������� �����
    {
        case LAB_FLAG_GET:
            n = GetLastError();
            FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM,
                           NULL,
                           n,
                           MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT),
                           buf,
                           size - 1,
                           NULL);
        break;

        case LAB_FLAG_ERRNO:
            n = errno;
            FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM,
                           NULL,
                           n,
                           MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT),
                           buf,
                           size - 1,
                           NULL);
        break;

        case LAB_FLAG_RESULT:
            FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM,
                           NULL,
                           iErr,
                           MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT),
                           buf,
                           size - 1,
                           NULL);
            break;

        default :
        sprintf(buf, "Error in programmer's function code is %d", iErr);
        break;
    }

    MessageBoxA(0, buf, chFuncFailMessage, MB_ICONERROR | MB_OK); // ����� ��������� �� ������
}

//---------------------------------------------------------------------------

int ReleaseHandle(HANDLE *h)
{
    int bRet = 0;
    
    //  ���� ����� ��������
    if ( *h != INVALID_HANDLE_VALUE )
    {
        bRet = CloseHandle(*h); // �������
        *h = INVALID_HANDLE_VALUE; // ������� ����������
    }
    
    return bRet;
}

//---------------------------------------------------------------------------

void ReleaseVoidMemory(void **vMemory)
{
    // ���� ��������� �� ������� ������ ��������
    if ( *vMemory != NULL )
    {
        free(*vMemory); // ����������
        *vMemory = NULL; // ������� ����������
    }
}

//---------------------------------------------------------------------------

int LabOpenFile(const char *chFileName, HANDLE *hFile, DWORD *dwFileSize)
{
    if ( chFileName == NULL ) // ���� ������ �����
    {
        ErrorOutput("Error in input buffer", LAB_BAD_BUF, LAB_FLAG_MY); // ���������
        return LAB_BAD_BUF; // ������� ������
    }

    // ������� ���� �� ������ � ������
    *hFile = CreateFileA(chFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if ( *hFile == INVALID_HANDLE_VALUE ) // ���� ������
    {
        ErrorOutput("Error in CreateFileA function", LAB_ERR, LAB_FLAG_GET); // ������� ���������
        return LAB_ERR; // ������� -1
    }

    // ��������� ������� �����
    *dwFileSize = GetFileSize(*hFile, NULL);

    if ( *dwFileSize == INVALID_FILE_SIZE ) // ���� ������
    {
        ErrorOutput("Error in GetFileSize function", LAB_ERR, LAB_FLAG_GET); // ������� ���������
        return LAB_ERR; // ������� -1
    }

    return LAB_OK;
}

//---------------------------------------------------------------------------

int LabReadFile(HANDLE hFile, char **chFileContent, DWORD *dwFileSize)
{
    BOOL bResult = FALSE; // ������������ �������� ��������
    DWORD dwNumberOfBytesRead = 0; // ���������� ��������� ����
    DWORD dwSum = 0; // ��������� ���������� ��������� ����
    DWORD dwFilePointer = INVALID_SET_FILE_POINTER; // �������� � �����

    if ( hFile == INVALID_HANDLE_VALUE ) // ���� ������ �����
    {
        ErrorOutput("Error in file", LAB_BAD_FILE, LAB_FLAG_MY); // ���������
        return LAB_BAD_FILE; // ������� ������
    }
    
    dwFilePointer = SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
    
    if ( dwFilePointer == INVALID_SET_FILE_POINTER ) // ���� ������
    {
        ErrorOutput("Error in SetFilePointer function", LAB_ERR, LAB_FLAG_GET); // ������� ���������
        return LAB_ERR; // ������� -1
    }

    // ��������� ������� �����
    *dwFileSize = GetFileSize(hFile, NULL);

    if ( *dwFileSize == INVALID_FILE_SIZE ) // ���� ������
    {
        ErrorOutput("Error in GetFileSize function", LAB_ERR, LAB_FLAG_GET); // ������� ���������
        return LAB_ERR; // ������� -1
    }

    // ��������� ������ ��� ���������� �����
    *chFileContent = (char *)malloc(*dwFileSize + 1);

    if ( *chFileContent == NULL ) // ���� ������ �� ����������
    {
        ErrorOutput("Error in malloc function", LAB_ERR, LAB_FLAG_ERRNO); // ������� ���������
        return LAB_ERR; // ������� -1
    }

    // ������ �����
    while ( dwSum < *dwFileSize )
    {
        bResult = ReadFile(hFile, (LPVOID)(*chFileContent + dwSum), *dwFileSize - dwSum, &dwNumberOfBytesRead, NULL);

        if ( bResult == FALSE ) // ���� ������
        {
            ErrorOutput("Error in ReadFile function", LAB_ERR, LAB_FLAG_GET); // ������� ���������
            return LAB_ERR; // ������� -1
        }
        else if ( dwNumberOfBytesRead == 0 )
        {
            ErrorOutput("Error in ReadFile function: dwNumberOfBytesRead is 0", LAB_BAD_FILE, LAB_FLAG_MY); // ������� ���������
            return LAB_BAD_FILE; // ������� -1
        }

        dwSum += dwNumberOfBytesRead;
    }

    *dwFileSize = dwSum;

    (*chFileContent)[*dwFileSize] = '\0'; // ����-����

    return LAB_OK;
}

//---------------------------------------------------------------------------

int LabWriteFile(HANDLE hFile, const char *chFileContent, DWORD *dwFileSize)
{
    BOOL bResult = FALSE; // ������������ �������� ��������
    DWORD dwFilePointer = INVALID_SET_FILE_POINTER; // �������� � �����
    
    if ( hFile == INVALID_HANDLE_VALUE ) // ���� ������ �����
    {
        ErrorOutput("Error in file", LAB_BAD_FILE, LAB_FLAG_MY); // ���������
        return LAB_BAD_FILE; // ������� ������
    }
    
    dwFilePointer = SetFilePointer(hFile, 0, NULL, FILE_END);
    
    if ( dwFilePointer == INVALID_SET_FILE_POINTER ) // ���� ������
    {
        ErrorOutput("Error in SetFilePointer function", LAB_ERR, LAB_FLAG_GET); // ������� ���������
        return LAB_ERR; // ������� -1
    }

    // ������ � ����
    bResult = WriteFile(hFile, (LPCVOID)(chFileContent), strlen(chFileContent), dwFileSize, NULL);

    if ( bResult == FALSE ) // ���� ������
    {
        ErrorOutput("Error in WriteFile function", LAB_ERR, LAB_FLAG_GET); // ������� ���������
        return LAB_ERR; // ������� -1
    }

    return LAB_OK;
}

//---------------------------------------------------------------------------

int LabCloseFile(HANDLE *hFile)
{
    int bRet = 0;
    
    bRet = ReleaseHandle(hFile);
    
    return bRet;
}

//---------------------------------------------------------------------------

int main()
{
    DWORD len = 0;
    int ret = LAB_OK;
    char *buf = NULL;
    char *command = NULL;
    char *chFileRead = NULL;
    char chFileName[] = {"ValeriyaDomanova.txt"};
    char chFileToWrite[] = {"Hello world\n"};
    unsigned long i = 0;
    HANDLE hFile = INVALID_HANDLE_VALUE;
    BOOL bRet = FALSE;
    
    // ��������� ������ ��� "�����" ����� � ��������
    buf = (char *)malloc(LAB_MAX_BUF_SIZE + 1);
    
    // ��������� ������ ��� ����� � ��������
    command = (char *)malloc(LAB_MAX_BUF_SIZE + 1);
    
    while ( 1 )
    {
        printf("Waiting for user input...\n");
        
        // ������� ������� ��� ���� ��������� ��������
        fgets(buf, LAB_MAX_BUF_SIZE - 2, stdin);
        
        printf("buf is %s\n", buf);

        // ���������� ������� ������� �������-�����������
        for ( i = 0; (i < strlen(buf)) && (buf[i] != ' ') && (buf[i] != '\t') && (buf[i] != '\n') ; i++ ) ; 

        strncpy(command, buf, i);
        command[i] = '\0';
        
        printf("command is %s\n", command);

        if ( strlen(command) == 0 )
        {
            fprintf(stderr, "command %s is empty\n", command);
        }
        else if ( strncasecmp(command, "open", strlen(command)) == 0 )
        {
            ret = LabOpenFile(chFileName, &hFile, &len);
        }
        else if ( strncasecmp(command, "write", strlen(command)) == 0 )
        {
            ret = LabWriteFile(hFile, chFileToWrite, &len);
        }
        else if ( strncasecmp(command, "read", strlen(command)) == 0 )
        {
            ret = LabReadFile(hFile, &chFileRead, &len);
            
            // ������� �� ����� ���������� �����
            printf("Content of the file is:\n%s\n", chFileRead);
            ReleaseVoidMemory((void **)&chFileRead);
        }
        else if ( strncasecmp(command, "close", strlen(command)) == 0 )
        {
            ret = LabCloseFile(&hFile);
        }
        else if ( strncasecmp(command, "end", strlen(command)) == 0 )
        {
            break;
        }
        else
        {
            fprintf(stderr, "command %s error\n", command);
        }
    }
    
    // ���������� ������, ���� �� �����������
    
    ReleaseVoidMemory((void **)&buf);
    ReleaseVoidMemory((void **)&command);
    ReleaseVoidMemory((void **)&chFileRead);
    
    // ������� ����, ���� �� ������
    
    bRet = ReleaseHandle(&hFile);
    
    return ret;
}
