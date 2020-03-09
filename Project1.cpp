#include "windows.h"
#include <conio.h> //для использования getch()
#include <iostream>// потоки воода-вывода
#include "time.h"//использовать временные задержки sleep(1000)
#include <stdio.h> 
#include <stdlib.h> 
#include <mutex> //для работы с потоками
#include <strsafe.h>
#include <tchar.h>//для print_f

#define BUFSIZE 512
#define CreateMailslot CreateMailslotW

using namespace std;//подключаем функции стандартных библиотек
//======всё для почтовых ящиков
HANDLE hSlot, hSlot1, hSlot2;//глобальные переменные
void WritingtoaMailslot(LPCTSTR lpszSlotName, LPCTSTR lpszMessage2);//отправка сообщений
BOOL WriteSlot(HANDLE hSlott, LPCTSTR lpszMessage);//запись сообщений
BOOL ReadSlot(int rr);//чтение сообщений
BOOL WINAPI MakeSlot(LPCTSTR lpszSlotName, LPCTSTR SlotName, int rr);//создание сервера почты
LPCTSTR SlotNameServer = TEXT("\\\\.\\mailslot\\server_mailslot");//ящик сервера 
LPCTSTR SlotNameClient1 = TEXT("\\\\.\\mailslot\\client1_mailslot");//ящик первого клиента 
LPCTSTR SlotNameClient2 = TEXT("\\\\.\\mailslot\\client2_mailslot");//ящик второго клиента
void ms0(); void ms1(); void ms2();//действия потоков
//==========всё для каналов
void mother(); void doch(); void sinok();
HANDLE  hNamedPipe2;//глобальные переменные
LPCTSTR LpPipeName = TEXT("\\\\.\\pipe\\MyPipe");

int main()
{
    setlocale(LC_ALL, "rus"); cout.imbue(locale("rus_rus.1251"));//выводит русские буквы
    while (exit)
    {
        cout << "Выберите действие:" << endl
            << "0: Почтовые ящики" << endl
            << "1: Каналы" << endl
            << "2: Отображение файлов" << endl
            << "3: Выход" << endl << "Выбор: ";
        int n; cin >> n;
        switch (n) {
        case 0: {
            thread potok0(ms0); thread potok1(ms1); thread potok2(ms2);
            potok0.join(); potok1.join(); potok2.join();//запускаем поток    
            cout << "Удаление почтовых ящиков" << endl;
            break;
        }
        case 1: {
            thread potok00(mother); thread potok11(doch); thread potok22(sinok);
            potok00.join();  potok11.join();  potok22.join();//запускаем потоки 
            break;
        }
        case 2: {

            break;
        }
        case 3: {exit(0); break;}
        }
    }
    _getch(); return 0;
}
/////=====КАНАЛЫ
void mother() {
    BOOL fConnected; DWORD cbRead, cbWritten; LPCWSTR szBuf = TEXT("Привет"); TCHAR  fnBuf[BUFSIZE];

    HANDLE hNamedPipe = CreateNamedPipe(LpPipeName, PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, 512, 512, 5000, NULL);
    if (hNamedPipe == INVALID_HANDLE_VALUE) { _tprintf(TEXT("CreateNamedPipe провалилась,из-за ошибки (%d)\n"), GetLastError()); }
    fConnected = ConnectNamedPipe(hNamedPipe, NULL);
    if (!fConnected) {
        _tprintf(TEXT("ConnectNamedPipe провалилась,из-за ошибки (%d)\n"), GetLastError());
        CloseHandle(hNamedPipe);
    }
    cout << "\n===== Семейный чат открыт =====";
    WriteFile(hNamedPipe,        // handle to pipe 
        szBuf,     // buffer to write from 
        sizeof(szBuf) + 1, // number of bytes to write 
        &cbWritten,   // number of bytes written 
        NULL);        // not overlapped I/O 
    cout << "\nМама подключилась к чату";
    _tprintf(TEXT("\nМама отправила сообщение:  %s"), szBuf);
    while (1) {
        if (ReadFile(hNamedPipe, fnBuf, BUFSIZE * sizeof(TCHAR), &cbRead, NULL)) {
            cout << "\nМама прочла сообщение";
            szBuf = TEXT("Как дела?");
            WriteFile(hNamedPipe, szBuf, sizeof(szBuf) + 1, &cbWritten, NULL);
            _tprintf(TEXT("\nМама отправила сообщение:  %s"), szBuf);
            Sleep(700); break;
        }
        else { _tprintf(TEXT("WriteFile провалилась,из-за ошибки (%d)\n"), GetLastError()); break; }
    }
    DisconnectNamedPipe(hNamedPipe);
    fConnected = ConnectNamedPipe(hNamedPipe, NULL);
    if (!fConnected) {
        _tprintf(TEXT("ConnectNamedPipe провалилась,из-за ошибки (%d)\n"), GetLastError());
        CloseHandle(hNamedPipe); _getch();
    }
    cout << "\nМама прочла сообщение";
    szBuf = TEXT("Окей, жду.");
    WriteFile(hNamedPipe, szBuf, sizeof(szBuf) + 1, &cbWritten, NULL);
    _tprintf(TEXT("\nМама отправила сообщение:  %s"), szBuf);
    while (1) {
        if (ReadFile(hNamedPipe, fnBuf, BUFSIZE * sizeof(TCHAR), &cbRead, NULL)) {
            cout << "\nМама прочла сообщение";
            szBuf = TEXT("А как твои дела?");
            WriteFile(hNamedPipe, szBuf, sizeof(szBuf) + 1, &cbWritten, NULL);
            _tprintf(TEXT("\nМама отправила сообщение:  %s"), szBuf);
            Sleep(1000);
            WriteFile(hNamedPipe, szBuf, sizeof(szBuf) + 1, &cbWritten, NULL);
            Sleep(1000);
            cout << "\nМама прочла сообщение";
            break;
        }
        else { _tprintf(TEXT("WriteFile провалилась,из-за ошибки (%d)\n"), GetLastError()); break; }
    } cout << "\nСын вышел из чата";  cout << "\nДочь вышла из чата";   cout << "\nМама вышла из чата";
    cout << "\n===== Семейный чат закрыт =====\n\n";
    CloseHandle(hNamedPipe);
}
void doch() {
    DWORD cbRead, cbWritten; LPCWSTR szBuf = TEXT("Привет :)"); TCHAR  fnBuf[BUFSIZE];
    Sleep(2000);
    HANDLE hNamedPipe1 = CreateFile(LpPipeName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (hNamedPipe1 == INVALID_HANDLE_VALUE) { _tprintf(TEXT("Дочка не смогла написать,из-за ошибки (%d)\n"), GetLastError()); }
    while (1) {
        if (ReadFile(hNamedPipe1, fnBuf, BUFSIZE * sizeof(TCHAR), &cbRead, NULL)) {
            cout << "\nДочь подключилась к чату";
            cout << "\nДочь прочла сообщение";
            WriteFile(hNamedPipe1, szBuf, sizeof(szBuf) + 1, &cbWritten, NULL);
            _tprintf(TEXT("\nДочь отправила сообщение:  %s"), szBuf);
            Sleep(100);
            cout << "\nДочь прочла сообщение";
            szBuf = TEXT("Хорошо. Сейчас брата толкну, тоже ответит");
            WriteFile(hNamedPipe1, szBuf, sizeof(szBuf) + 1, &cbWritten, NULL);
            _tprintf(TEXT("\nДочь отправила сообщение:  %s"), szBuf);////
            break;
        }
        else { _tprintf(TEXT("WriteFile провалилась,из-за ошибки (%d)\n"), GetLastError()); break; }
    }
    CloseHandle(hNamedPipe1); //_getch();
}
void sinok() {
    DWORD cbRead, cbWritten; LPCWSTR szBuf = TEXT("Привет!"); TCHAR  fnBuf[BUFSIZE];
    Sleep(5000);
    hNamedPipe2 = CreateFile(LpPipeName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (hNamedPipe2 == INVALID_HANDLE_VALUE) { _tprintf(TEXT("Сынок не смог написать,из-за ошибки (%d)\n"), GetLastError()); }
    while (1) {
        if (ReadFile(hNamedPipe2, fnBuf, BUFSIZE * sizeof(TCHAR), &cbRead, NULL)) {
            cout << "\nСын подключился к чату";
            cout << "\nСын прочёл сообщение";
            WriteFile(hNamedPipe2, szBuf, sizeof(szBuf) + 1, &cbWritten, NULL);
            _tprintf(TEXT("\nСын отправил сообщение:  %s"), szBuf);
            Sleep(100);
            cout << "\nСын прочёл сообщение";
            szBuf = TEXT("Тоже хорошо :3");
            WriteFile(hNamedPipe2, szBuf, sizeof(szBuf) + 1, &cbWritten, NULL);
            _tprintf(TEXT("\nСын отправил сообщение:  %s"), szBuf);////
            break;
        }
        else { _tprintf(TEXT("WriteFile провалилась,из-за ошибки (%d)\n"), GetLastError()); break; }
        Sleep(500);
    }
    CloseHandle(hNamedPipe2);
}
/////=====ПОЧТОВЫЕ ЯЩИКИ
void ms0() {
    MakeSlot(SlotNameServer, TEXT(" сервера "), 0); int h = 0; Sleep(5000);//ждём
    cout << "\n====|Почтовый ящик сервера|====\n";
    while (TRUE) {
        ReadSlot(0);
        Sleep(2800); h = h + 1;
        if (h == 3) { cout << "Конец ожидания" << endl; break; }
    }
    Sleep(5000);
    WritingtoaMailslot(SlotNameClient1, TEXT("Сообщение от сервера для первого клиента"));
    WritingtoaMailslot(SlotNameClient2, TEXT("Сообщение от сервера для второго клиента"));
}
void ms1() {
    Sleep(1000); WritingtoaMailslot(SlotNameServer, TEXT("Cообщение для сервера от клиента1")); Sleep(16000);
    MakeSlot(SlotNameClient1, TEXT(" клиента1 "), 1);  int h = 0; Sleep(5000);//ждём
    cout << "\n====|Почтовый ящик первого клиента|====\n";
    while (TRUE) {
        ReadSlot(1);
        Sleep(2800); h = h + 1;
        if (h == 3) { cout << "Конец ожидания" << endl; break; }
    }
}
void ms2() {
    Sleep(1000); WritingtoaMailslot(SlotNameServer, TEXT("Сообщение для сервера от клиента2")); Sleep(16000);
    MakeSlot(SlotNameClient2, TEXT(" клиента2 "), 2); int h = 0; Sleep(18000);//ждём
    cout << "\n====|Почтовый ящик второго клиента|====\n";
    while (TRUE) {
        ReadSlot(2);
        Sleep(2800); h = h + 1;
        if (h == 3) { cout << "Конец ожидания" << endl; break; }
    }
}
void WritingtoaMailslot(LPCTSTR lpszSlotName, LPCTSTR lpszMessage2) {
    // С помощью функции CreateFile клиент открывает mailslot для записи сообщений
    HANDLE hFile = CreateFile(lpszSlotName,
        GENERIC_WRITE,
        FILE_SHARE_READ,               // Требуется для записи в mailslot 
        (LPSECURITY_ATTRIBUTES)NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        (HANDLE)NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        cout << "Ошибка открытия почтового ящика\n";
    }
    // Запись сообщения в почтовый ящик
    WriteSlot(hFile, lpszMessage2);
}
BOOL ReadSlot(int rr)
{
    DWORD cbMessage, cMessage, cbRead;  BOOL fResult; LPTSTR lpszBuffer;
    TCHAR achID[80]; DWORD cAllMessages; HANDLE hEvent;  OVERLAPPED ov;
    cbMessage = cMessage = cbRead = 0;
    // Дескриптор почтового ящика "hSlot" определен глобально. 
    hEvent = CreateEvent(NULL, FALSE, FALSE, TEXT("ExampleSlot"));
    if (NULL == hEvent)
        return FALSE;
    ov.Offset = 0; ov.OffsetHigh = 0;  ov.hEvent = hEvent;

    if (rr == 0) {
        fResult = GetMailslotInfo(hSlot, // дескриптор mailslot’а
            (LPDWORD)NULL,               // без ограничения размера сообщения
            &cbMessage,                   // размер следующего сообщения
            &cMessage,                    // количество сообщений в ящике 
            (LPDWORD)NULL);              // без таймаута чтения 
    }
    if (rr == 1) {
        fResult = GetMailslotInfo(hSlot1, // дескриптор mailslot’а
            (LPDWORD)NULL,               // без ограничения размера сообщения
            &cbMessage,                   // размер следующего сообщения
            &cMessage,                    // количество сообщений в ящике 
            (LPDWORD)NULL);              // без таймаута чтения 
    }
    if (rr == 2) {
        fResult = GetMailslotInfo(hSlot2, // дескриптор mailslot’а
            (LPDWORD)NULL,               // без ограничения размера сообщения
            &cbMessage,                   // размер следующего сообщения
            &cMessage,                    // количество сообщений в ящике 
            (LPDWORD)NULL);              // без таймаута чтения 
    }

    if (!fResult) { printf("Ошибка при получении информации о почтовом ящике %d.\n", GetLastError()); return FALSE; }
    if (cbMessage == MAILSLOT_NO_MESSAGE) { printf("Ожидаем сообщения...\n"); return TRUE; }
    cAllMessages = cMessage;
    while (cMessage != 0)  // Считываем все сообщения
    {
        // Создаем строку с номером сообщения
        StringCchPrintf((LPTSTR)achID, 80, TEXT("\nСообщение #%d из %d\n"), cAllMessages - cMessage + 1, cAllMessages);
        // Выделяем память для сообщения. 
        lpszBuffer = (LPTSTR)GlobalAlloc(GPTR,
            lstrlen((LPTSTR)achID) * sizeof(TCHAR) + cbMessage);
        if (NULL == lpszBuffer)
            return FALSE;
        lpszBuffer[0] = '\0';
        // Считываем сообщение из почтового ящика
        if (rr == 0) { fResult = ReadFile(hSlot, lpszBuffer, cbMessage, &cbRead, &ov); }
        if (rr == 1) { fResult = ReadFile(hSlot1, lpszBuffer, cbMessage, &cbRead, &ov); }
        if (rr == 2) { fResult = ReadFile(hSlot2, lpszBuffer, cbMessage, &cbRead, &ov); }
        if (!fResult) {
            printf("Ошибка чтения сообщения: %d.\n", GetLastError());
            GlobalFree((HGLOBAL)lpszBuffer);
            return FALSE;
        }
        // Формируем строку с номером и текстом сообщения. 
        StringCbCat(lpszBuffer,
            lstrlen((LPTSTR)achID) * sizeof(TCHAR) + cbMessage,
            (LPTSTR)achID);
        // Выводим сообщение на экран. 
        _tprintf(TEXT("Содержимое почтового ящика: %s\n"), lpszBuffer);
        GlobalFree((HGLOBAL)lpszBuffer);
        if (rr == 0) {
            fResult = GetMailslotInfo(hSlot, // дескриптор mailslot’а
                (LPDWORD)NULL,               // без ограничения размера сообщения
                &cbMessage,                   // размер следующего сообщения
                &cMessage,                    // количество сообщений в ящике 
                (LPDWORD)NULL);              // без таймаута чтения 
        }
        if (rr == 1) {
            fResult = GetMailslotInfo(hSlot1, // дескриптор mailslot’а
                (LPDWORD)NULL,               // без ограничения размера сообщения
                &cbMessage,                   // размер следующего сообщения
                &cMessage,                    // количество сообщений в ящике 
                (LPDWORD)NULL);              // без таймаута чтения 
        }
        if (rr == 2) {
            fResult = GetMailslotInfo(hSlot2, // дескриптор mailslot’а
                (LPDWORD)NULL,               // без ограничения размера сообщения
                &cbMessage,                   // размер следующего сообщения
                &cMessage,                    // количество сообщений в ящике 
                (LPDWORD)NULL);              // без таймаута чтения 
        }
        if (!fResult) {
            printf("Ошибка при получении информации о mailslot’е (%d)\n", GetLastError());
            return FALSE;
        }
    }
    CloseHandle(hEvent);
    return TRUE;
}
BOOL WriteSlot(HANDLE hSlott, LPCTSTR lpszMessage)
{
    // Запись сообщения в почтовый ящик
    BOOL fResult; DWORD cbWritten;
    fResult = WriteFile(hSlott, lpszMessage, (DWORD)(lstrlen(lpszMessage) + 1) * sizeof(TCHAR), &cbWritten, (LPOVERLAPPED)NULL);
    if (!fResult) {
        printf("Ошибка при отправлении сообщения (%d)\n", GetLastError());
    }
    cout << "Сообщение отправлено успешно\n"; return TRUE;
}
BOOL WINAPI MakeSlot(LPCTSTR lpszSlotName, LPCTSTR SlotName, int rr) {
    if (rr == 0) {
        hSlot = CreateMailslot(lpszSlotName, 0,    //без максимального размера сообщения  
            MAILSLOT_WAIT_FOREVER,         //без таймаута при операциях   
            (LPSECURITY_ATTRIBUTES)NULL); //без атрибутов безопасности       
        if (hSlot == INVALID_HANDLE_VALUE) { cout << "CreateMailslot не удалось вызвать из-за \n" << GetLastError(); return FALSE; }
        else _tprintf(TEXT("\nПочтовый ящик %s успешно создан\n"), SlotName);  return TRUE;
    }
    if (rr == 1) {
        hSlot1 = CreateMailslot(lpszSlotName, 0,    //без максимального размера сообщения  
            MAILSLOT_WAIT_FOREVER,         //без таймаута при операциях   
            (LPSECURITY_ATTRIBUTES)NULL); //без атрибутов безопасности       
        if (hSlot1 == INVALID_HANDLE_VALUE) { cout << "CreateMailslot не удалось вызвать из-за \n" << GetLastError(); return FALSE; }
        else _tprintf(TEXT("Почтовый ящик %s успешно создан\n"), SlotName);  return TRUE;
    }
    if (rr == 2) {
        hSlot2 = CreateMailslot(lpszSlotName, 0,    //без максимального размера сообщения  
            MAILSLOT_WAIT_FOREVER,         //без таймаута при операциях   
            (LPSECURITY_ATTRIBUTES)NULL); //без атрибутов безопасности       
        if (hSlot2 == INVALID_HANDLE_VALUE) { cout << "CreateMailslot не удалось вызвать из-за \n" << GetLastError(); return FALSE; }
        else _tprintf(TEXT("Почтовый ящик %s успешно создан\n"), SlotName);  return TRUE;
    }
}


