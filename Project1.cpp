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
void mother(); void doch(); void sinok();//действия потоков
HANDLE  hNamedPipe2;//глобальные переменные
LPCTSTR LpPipeName = TEXT("\\\\.\\pipe\\MyPipe");
//==========всё для отображения файлов
HANDLE hFileMap; LPCTSTR lpFileShre = TEXT("MyFileShre"); LPVOID lpFileMap;
void MappingServer(); void MappingSlave1(); void MappingSlave2();//действия потоков

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
            thread potokF0(MappingServer); thread potokF1(MappingSlave1); thread potokF2(MappingSlave2);
            potokF0.join();  potokF1.join(); potokF2.join();//запускаем потоки 
            break;
        }
        case 3: {exit(0); break;}
        }
    }
    _getch(); return 0;
}
/////=====ОТОБРАЖЕНИЕ ФАЙЛОВ
void MappingServer() {
    LPCWSTR szMsg = TEXT("Привет"); TCHAR  fnBuf[BUFSIZE]; LPVOID lpFileMap;
    hFileMap = CreateFileMapping((HANDLE)0xFFFFFFFF,NULL,PAGE_READWRITE,0,BUFSIZE,lpFileShre);
    if (hFileMap==0) { _tprintf(TEXT("CreateFileMapping провалилась,из-за ошибки (%d)\n"), GetLastError()); }
    else  cout << "\n=====/Создание беседы: Студенческая общага/=====\n";
    lpFileMap = MapViewOfFile(hFileMap,   // handle to map object
        FILE_MAP_ALL_ACCESS, // read/write permission
        0, 0, BUFSIZE);
    if (lpFileMap == 0) { _tprintf(TEXT("MapViewOfFile провалилась,из-за ошибки (%d)\n"), GetLastError()); }      
    CopyMemory(lpFileMap, szMsg, sizeof(szMsg));//запись данных в файл
    _tprintf(TEXT("Студент1: %s\n"), szMsg);
    Sleep(500);
    CopyMemory(fnBuf, lpFileMap, sizeof(fnBuf));//считываие данных из файла
    if (fnBuf != 0) { cout << "Студент1 прочёл сообщение\n"; }
    Sleep(1500);
    szMsg = TEXT("А мне пришла :3 Так что пойду поем. Заскакивайте");
    CopyMemory(lpFileMap, szMsg, sizeof(szMsg));//запись данных в файл
    _tprintf(TEXT("Студент1: %s\n"), szMsg);
    Sleep(1300);
    UnmapViewOfFile(lpFileMap);
    CloseHandle(hFileMap); cout << "=====/Закрытие беседы: Студенческая общага/=====\n\n";
}
void MappingSlave1() {
    LPCWSTR szMsg = TEXT("Привет :)"); TCHAR  fnBuf[BUFSIZE]; LPVOID lpFileMap;
    Sleep(500);
    hFileMap = OpenFileMapping( FILE_MAP_ALL_ACCESS,   // read/write access
        FALSE,                 // do not inherit the name
        lpFileShre);               // name of mapping object
    if (hFileMap == 0) { _tprintf(TEXT("OpenFileMapping провалилась,из-за ошибки (%d)\n"), GetLastError()); }
    lpFileMap = MapViewOfFile(hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, BUFSIZE);
    if (lpFileMap == 0) { _tprintf(TEXT("MapViewOfFile провалилась,из-за ошибки (%d)\n"), GetLastError()); }
    CopyMemory(fnBuf, lpFileMap, sizeof(fnBuf));//считываие данных из файла
    if (fnBuf != 0) { cout<<"Студент2 прочёл сообщение\n"; }
    CopyMemory(lpFileMap, szMsg, sizeof(szMsg));//запись данных в файл
    _tprintf(TEXT("Студент2: %s\n"), szMsg);
    Sleep(1000);
    CopyMemory(fnBuf, lpFileMap, sizeof(fnBuf));//считываие данных из файла
    if (fnBuf != 0) { cout << "Студент2 прочёл сообщение\n"; }
    szMsg = TEXT("Мне нет. Пока что только дотация");
    CopyMemory(lpFileMap, szMsg, sizeof(szMsg));//запись данных в файл
    _tprintf(TEXT("Студент2: %s\n"), szMsg);
    Sleep(500);
    CopyMemory(fnBuf, lpFileMap, sizeof(fnBuf));//считываие данных из файла
    if (fnBuf != 0) { cout << "Студент2 прочёл сообщение\n"; }
    szMsg = TEXT("Меня дважды звать не надо");
    CopyMemory(lpFileMap, szMsg, sizeof(szMsg));//запись данных в файл
    _tprintf(TEXT("Студент2: %s\n"), szMsg);
    UnmapViewOfFile(lpFileMap);
}
void MappingSlave2() {
    LPCWSTR szMsg = TEXT("Здарова. Вам стипендия пришла?"); TCHAR  fnBuf[BUFSIZE]; LPVOID lpFileMap;
    Sleep(1000);
    hFileMap = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, lpFileShre);  
    if (hFileMap == 0) { _tprintf(TEXT("OpenFileMapping провалилась,из-за ошибки (%d)\n"), GetLastError()); }
    lpFileMap = MapViewOfFile(hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, BUFSIZE);
    if (lpFileMap == 0) { _tprintf(TEXT("MapViewOfFile провалилась,из-за ошибки (%d)\n"), GetLastError()); }
    CopyMemory(fnBuf, lpFileMap, sizeof(fnBuf));//считываие данных из файла
    if (fnBuf != 0) { cout << "Студент3 прочёл сообщение\n"; }
    CopyMemory(lpFileMap, szMsg, sizeof(szMsg));//запись данных в файл
    _tprintf(TEXT("Студент3: %s\n"), szMsg);
    Sleep(1200);
    CopyMemory(fnBuf, lpFileMap, sizeof(fnBuf));//считываие данных из файла
    if (fnBuf != 0) { cout << "Студент3 прочёл сообщение\n"; }
    szMsg = TEXT("Уже бегу");
    CopyMemory(lpFileMap, szMsg, sizeof(szMsg));//запись данных в файл
    _tprintf(TEXT("Студент3: %s\n"), szMsg);
    UnmapViewOfFile(lpFileMap);
}
/////=====КАНАЛЫ
void mother() {
    BOOL fConnected; DWORD cbRead, cbWritten; LPCWSTR szBuf = TEXT("Привет"); TCHAR  fnBuf[BUFSIZE];
     HANDLE hNamedPipe = CreateNamedPipe(LpPipeName, PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, 512, 512, 5000, NULL);
    if (hNamedPipe == INVALID_HANDLE_VALUE) { _tprintf(TEXT("CreateNamedPipe провалилась,из-за ошибки (%d)\n"), GetLastError()); }
    fConnected = ConnectNamedPipe(hNamedPipe, NULL);
    if (!fConnected) { _tprintf(TEXT("ConnectNamedPipe провалилась,из-за ошибки (%d)\n"), GetLastError());  CloseHandle(hNamedPipe); }
    else  cout << "\n===== Семейный чат открыт =====";
    cout << "\nМама подключилась к чату"; 
    WriteFile(hNamedPipe,        // handle to pipe 
        szBuf,     // buffer to write from 
        sizeof(szBuf) + 1, // number of bytes to write 
        &cbWritten,   // number of bytes written 
        NULL);        // not overlapped I/O 
     _tprintf(TEXT("\nМама отправила сообщение:  %s"), szBuf);
    while (1) {
        if (ReadFile(hNamedPipe, fnBuf, BUFSIZE * sizeof(TCHAR), &cbRead, NULL)) {
            cout << "\nМама прочла сообщение";
            szBuf = TEXT("Как дела?");
            if (WriteFile(hNamedPipe, szBuf, sizeof(szBuf) + 1, &cbWritten, NULL)){ _tprintf(TEXT("\nМама отправила сообщение:  %s"), szBuf); }
            Sleep(700); break;
        }
        else { _tprintf(TEXT("WriteFile провалилась,из-за ошибки (%d)\n"), GetLastError()); break; }
    }
    DisconnectNamedPipe(hNamedPipe);
    fConnected = ConnectNamedPipe(hNamedPipe, NULL);
    if (!fConnected) { _tprintf(TEXT("ConnectNamedPipe провалилась,из-за ошибки (%d)\n"), GetLastError());CloseHandle(hNamedPipe); _getch(); }
    cout << "\nМама прочла сообщение";
    szBuf = TEXT("Окей, жду.");
    if (WriteFile(hNamedPipe, szBuf, sizeof(szBuf) + 1, &cbWritten, NULL)) { _tprintf(TEXT("\nМама отправила сообщение:  %s"), szBuf); }
    while (1) {
        if (ReadFile(hNamedPipe, fnBuf, BUFSIZE * sizeof(TCHAR), &cbRead, NULL)) {
            cout << "\nМама прочла сообщение";
            szBuf = TEXT("А как твои дела?");
            if (WriteFile(hNamedPipe, szBuf, sizeof(szBuf) + 1, &cbWritten, NULL)) { _tprintf(TEXT("\nМама отправила сообщение:  %s"), szBuf); }
            Sleep(2000);
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
    Sleep(3500);
    HANDLE hNamedPipe1 = CreateFile(LpPipeName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (hNamedPipe1 == INVALID_HANDLE_VALUE) { _tprintf(TEXT("Дочка не смогла написать,из-за ошибки (%d)\n"), GetLastError()); }
    else  Sleep(1000); cout << "\nДочь подключилась к чату";
    while (1) {
        if (ReadFile(hNamedPipe1, fnBuf, BUFSIZE * sizeof(TCHAR), &cbRead, NULL)) {
            cout << "\nДочь прочла сообщение";
            if (WriteFile(hNamedPipe1, szBuf, sizeof(szBuf) + 1, &cbWritten, NULL)) { _tprintf(TEXT("\nДочь отправила сообщение:  %s"), szBuf); }
            Sleep(100);
            cout << "\nДочь прочла сообщение";
            szBuf = TEXT("Хорошо. Сейчас брата толкну, тоже ответит");
            if (WriteFile(hNamedPipe1, szBuf, sizeof(szBuf) + 1, &cbWritten, NULL)) { _tprintf(TEXT("\nДочь отправила сообщение:  %s"), szBuf); }
            break;
        }
        else { _tprintf(TEXT("WriteFile провалилась,из-за ошибки (%d)\n"), GetLastError()); break; }
    }
    CloseHandle(hNamedPipe1); //_getch();
}
void sinok() {
    DWORD cbRead, cbWritten; LPCWSTR szBuf = TEXT("Привет!"); TCHAR  fnBuf[BUFSIZE];
    Sleep(5500);
    hNamedPipe2 = CreateFile(LpPipeName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (hNamedPipe2 == INVALID_HANDLE_VALUE) { _tprintf(TEXT("Сынок не смог написать,из-за ошибки (%d)\n"), GetLastError()); }
    else cout << "\nСын подключился к чату";
    while (1) {
        if (ReadFile(hNamedPipe2, fnBuf, BUFSIZE * sizeof(TCHAR), &cbRead, NULL)) {
            cout << "\nСын прочёл сообщение";
            if (WriteFile(hNamedPipe2, szBuf, sizeof(szBuf) + 1, &cbWritten, NULL)) { _tprintf(TEXT("\nСын отправил сообщение:  %s"), szBuf); }
            Sleep(100);
            cout << "\nСын прочёл сообщение";
            szBuf = TEXT("Тоже хорошо :3");
            if (WriteFile(hNamedPipe2, szBuf, sizeof(szBuf) + 1, &cbWritten, NULL)) { _tprintf(TEXT("\nСын отправил сообщение:  %s"), szBuf); }
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
    HANDLE hFile = CreateFile(lpszSlotName,    // С помощью функции CreateFile клиент открывает mailslot для записи сообщений
        GENERIC_WRITE,
        FILE_SHARE_READ,               // Требуется для записи в mailslot 
        (LPSECURITY_ATTRIBUTES)NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        (HANDLE)NULL);
    if (hFile == INVALID_HANDLE_VALUE) { cout << "Ошибка открытия почтового ящика\n"; }
    WriteSlot(hFile, lpszMessage2);    // Запись сообщения в почтовый ящик
}
BOOL ReadSlot(int rr){
    DWORD cbMessage, cMessage, cbRead;  BOOL fResult; LPTSTR lpszBuffer;
    TCHAR achID[80]; DWORD cAllMessages; HANDLE hEvent;  OVERLAPPED ov;
    cbMessage = cMessage = cbRead = 0;
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
    if (rr == 1) {fResult = GetMailslotInfo(hSlot1, (LPDWORD)NULL,&cbMessage, &cMessage, (LPDWORD)NULL); }
    if (rr == 2) { fResult = GetMailslotInfo(hSlot2, (LPDWORD)NULL, &cbMessage, &cMessage, (LPDWORD)NULL); }
    if (!fResult) { printf("Ошибка при получении информации о почтовом ящике %d.\n", GetLastError()); return FALSE; }
    if (cbMessage == MAILSLOT_NO_MESSAGE) { printf("Ожидаем сообщения...\n"); return TRUE; }
    cAllMessages = cMessage;
    while (cMessage != 0) { // Считываем все сообщения    
        StringCchPrintf((LPTSTR)achID, 80, TEXT("\nСообщение #%d из %d\n"), cAllMessages - cMessage + 1, cAllMessages);        // Создаем строку с номером сообщения
        lpszBuffer = (LPTSTR)GlobalAlloc(GPTR,lstrlen((LPTSTR)achID) * sizeof(TCHAR) + cbMessage);        // Выделяем память для сообщения. 
        if (NULL == lpszBuffer)
            return FALSE;
        lpszBuffer[0] = '\0';
        if (rr == 0) { fResult = ReadFile(hSlot, lpszBuffer, cbMessage, &cbRead, &ov); }        // Считываем сообщение из почтового ящика
        if (rr == 1) { fResult = ReadFile(hSlot1, lpszBuffer, cbMessage, &cbRead, &ov); }        // Считываем сообщение из почтового ящика
        if (rr == 2) { fResult = ReadFile(hSlot2, lpszBuffer, cbMessage, &cbRead, &ov); }        // Считываем сообщение из почтового ящика
        if (!fResult) {
            printf("Ошибка чтения сообщения: %d.\n", GetLastError());
            GlobalFree((HGLOBAL)lpszBuffer);
            return FALSE;
        }
        StringCbCat(lpszBuffer, lstrlen((LPTSTR)achID) * sizeof(TCHAR) + cbMessage, (LPTSTR)achID);        // Формируем строку с номером и текстом сообщения. 
        _tprintf(TEXT("Содержимое почтового ящика: %s\n"), lpszBuffer);
        GlobalFree((HGLOBAL)lpszBuffer);
        if (rr == 0) {
            fResult = GetMailslotInfo(hSlot, // дескриптор mailslot’а
                (LPDWORD)NULL,               // без ограничения размера сообщения
                &cbMessage,                   // размер следующего сообщения
                &cMessage,                    // количество сообщений в ящике 
                (LPDWORD)NULL);              // без таймаута чтения 
        }
        if (rr == 1) { fResult = GetMailslotInfo(hSlot1, (LPDWORD)NULL, &cbMessage,  &cMessage,(LPDWORD)NULL); }
        if (rr == 2) { fResult = GetMailslotInfo(hSlot2, (LPDWORD)NULL, &cbMessage,  &cMessage, (LPDWORD)NULL);}
        if (!fResult) {  printf("Ошибка при получении информации о mailslot’е (%d)\n", GetLastError());  return FALSE; }
    }
    CloseHandle(hEvent);
    return TRUE;
}
BOOL WriteSlot(HANDLE hSlott, LPCTSTR lpszMessage){
    BOOL fResult; DWORD cbWritten;
    fResult = WriteFile(hSlott, lpszMessage, (DWORD)(lstrlen(lpszMessage) + 1) * sizeof(TCHAR), &cbWritten, (LPOVERLAPPED)NULL);    // Запись сообщения в почтовый ящик
    if (!fResult) { printf("Ошибка при отправлении сообщения (%d)\n", GetLastError()); }
    else cout << "Сообщение отправлено успешно\n"; return TRUE;
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
        hSlot1 = CreateMailslot(lpszSlotName, 0,  MAILSLOT_WAIT_FOREVER,  (LPSECURITY_ATTRIBUTES)NULL);     
        if (hSlot1 == INVALID_HANDLE_VALUE) { cout << "CreateMailslot не удалось вызвать из-за \n" << GetLastError(); return FALSE; }
        else _tprintf(TEXT("Почтовый ящик %s успешно создан\n"), SlotName);  return TRUE;
    }
    if (rr == 2) {
        hSlot2 = CreateMailslot(lpszSlotName, 0,   MAILSLOT_WAIT_FOREVER, (LPSECURITY_ATTRIBUTES)NULL);    
        if (hSlot2 == INVALID_HANDLE_VALUE) { cout << "CreateMailslot не удалось вызвать из-за \n" << GetLastError(); return FALSE; }
        else _tprintf(TEXT("Почтовый ящик %s успешно создан\n"), SlotName);  return TRUE;
    }
}
