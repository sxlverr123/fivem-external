#include <Windows.h>
#include <thread>
#include <future>
#include <Cheat/Cheat.hpp>
#include <FrameWork/FrameWork.hpp>
#include <tchar.h>

static BOOL CheckForUIAccess(DWORD* pdwErr, BOOL* pfUIAccess) /* https://discord.gg/daPJ7BWT7k */
{
    BOOL result = FALSE;
    HANDLE hToken;

    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
    {
        DWORD dwRetLen;

        if (GetTokenInformation(hToken, TokenUIAccess, pfUIAccess, sizeof(*pfUIAccess), &dwRetLen))
        {
            result = TRUE;
        }
        else
        {
            *pdwErr = GetLastError(); /* https://discord.gg/daPJ7BWT7k */
        }
        CloseHandle(hToken); /* https://discord.gg/daPJ7BWT7k */
    }
    else
    {
        *pdwErr = GetLastError(); /* https://discord.gg/daPJ7BWT7k */
    }

    return result;
}

DWORD WINAPI Unload()
{
    SafeCall(ExitProcess)(0); /* https://discord.gg/daPJ7BWT7k */
    return TRUE;
}

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    Cheat::Initialize(); /* https://discord.gg/daPJ7BWT7k */
}