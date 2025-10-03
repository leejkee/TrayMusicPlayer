//
// Created by 31305 on 2025/10/1.
//
#include <windows.h>
#include <shlwapi.h>
#include <string>

int WINAPI wWinMain(HINSTANCE hInstance
                    , HINSTANCE hPrevInstance
                    , PWSTR pCmdLine
                    , int nCmdShow)
{
    wchar_t currentDir[MAX_PATH];
    if (0 == GetCurrentDirectoryW(MAX_PATH, currentDir))
        return -1;

    wchar_t configFilePath[MAX_PATH];
    if (!PathCombineW(configFilePath, currentDir, L"settings\\init.json"))
        return -1;

    if (!PathFileExistsW(configFilePath))
    {
        MessageBoxW(nullptr
                    , configFilePath
                    , L"Config file not found: settings/init.json"
                    , MB_OK | MB_ICONERROR);
        return -1;
    }

    wchar_t workDir[MAX_PATH];
    if (!PathCombineW(workDir, currentDir, L"bin"))
        return -1;

    if (!PathFileExistsW(workDir))
    {
        MessageBoxW(nullptr
                    , workDir
                    , L"Work directory not exists: bin"
                    , MB_OK | MB_ICONERROR);
        return -1;
    }

    wchar_t exePath[MAX_PATH];
    if (!PathCombineW(exePath, workDir, L"traymusic.exe"))
        return -1;

    if (!PathFileExistsW(exePath))
    {
        MessageBoxW(nullptr
                    , exePath
                    , L"Executable file not exists: traymusic.exe"
                    , MB_OK | MB_ICONERROR);
        return -1;
    }

    STARTUPINFOW si{sizeof(si)};
    PROCESS_INFORMATION pi{};

    if (!CreateProcessW(exePath
                        , configFilePath
                        , nullptr
                        , nullptr
                        , FALSE
                        , 0
                        , nullptr
                        , workDir
                        , &si
                        , &pi))
    {
        MessageBoxW(nullptr
                    , exePath
                    , L"Failed to start traymusic.exe!"
                    , MB_OK | MB_ICONERROR);
        return -1;
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return 0;
}