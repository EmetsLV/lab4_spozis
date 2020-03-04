// Project1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "windows.h"
#include <conio.h> //для использования getch()
//#include "process.h"
#include <iostream>// потоки воода-вывода
#include "time.h"//использовать временные задержки sleep(1000)
#include <stdio.h> 
#include <stdlib.h> 
#include <mutex> //для работы с потоками
#include <strsafe.h>
#include <tchar.h>//для print_f

#define CreateMailslot CreateMailslotW
using namespace std;//подключаем функции стандартных библиотек
//======всё для почтовых ящиков
HANDLE hSlot,hSlot1,hSlot2 ;//глобальные переменные
void WritingtoaMailslot(LPCTSTR lpszSlotName, LPCTSTR lpszMessage2);//отправка сообщений
BOOL WriteSlot(HANDLE hSlott, LPCTSTR lpszMessage);//запись сообщений
BOOL ReadSlot(int rr);//чтение сообщений
BOOL WINAPI MakeSlot(LPCTSTR lpszSlotName, LPCTSTR SlotName, int rr);//создание сервера почты
LPCTSTR SlotNameServer = TEXT("\\\\.\\mailslot\\server_mailslot");//ящик сервера 
LPCTSTR SlotNameClient1 = TEXT("\\\\.\\mailslot\\client1_mailslot");//ящик первого клиента 
LPCTSTR SlotNameClient2 = TEXT("\\\\.\\mailslot\\client2_mailslot");//ящик второго клиента
void pp0(); void pp1(); void pp2();//действия потоков

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
            thread potok0(pp0); thread potok1(pp1); thread potok2(pp2);
            potok0.join();//запускаем поток
            Sleep(9000);//ждём
            potok1.join(); potok2.join();//запускаем поток    
            cout << "Удаление почтовых ящиков" << endl;
            break;
        }
        case 1: {
            
            break;
        }
        case 2: {
            
            break;
        }
        case 3: {
            exit(0); break;
        }
        }
    }
    _getch();return 0;
}

void pp0() {
    MakeSlot(SlotNameServer, TEXT(" сервера "),0); int h = 0; Sleep(5000);//ждём
    cout << "\n====|Почтовый ящик сервера|====\n";
    while (TRUE) {        
        ReadSlot(0);
        Sleep(2800);h=h+1;
        if (h == 3) { cout << "Конец ожидания" << endl; break; }
    }
    Sleep(3000);
    WritingtoaMailslot(SlotNameClient1, TEXT("Сообщение от сервера для первого клиента"));
    WritingtoaMailslot(SlotNameClient2, TEXT("Сообщение от сервера для второго клиента"));
}
void pp1() {
    WritingtoaMailslot(SlotNameServer,TEXT("Первое сообщение для сервера ")); Sleep(16000);
    MakeSlot(SlotNameClient1, TEXT(" клиента1 "),1);  int h = 0; Sleep(5000);//ждём
    cout << "\n====|Почтовый ящик первого клиента|====\n";
    while (TRUE) {
        ReadSlot(1);
        Sleep(2800); h = h + 1;
        if (h == 3) { cout << "Конец ожидания" << endl; break; }
    }
}
void pp2() {
    WritingtoaMailslot(SlotNameServer, TEXT("Второе сообщение для сервера ")); Sleep(16000);
    MakeSlot(SlotNameClient2, TEXT(" клиента2 "),2); int h = 0; Sleep(19000);//ждём
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
    if (hFile == INVALID_HANDLE_VALUE){
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
   
    if (rr==0) {
        fResult = GetMailslotInfo(hSlot, // дескриптор mailslot’а
            (LPDWORD)NULL,               // без ограничения размера сообщения
            &cbMessage,                   // размер следующего сообщения
            &cMessage,                    // количество сообщений в ящике 
            (LPDWORD)NULL);              // без таймаута чтения 
    }
    if (rr==1) {
        fResult = GetMailslotInfo(hSlot1, // дескриптор mailslot’а
            (LPDWORD)NULL,               // без ограничения размера сообщения
            &cbMessage,                   // размер следующего сообщения
            &cMessage,                    // количество сообщений в ящике 
            (LPDWORD)NULL);              // без таймаута чтения 
    }
    if (rr==2) {
        fResult = GetMailslotInfo(hSlot2, // дескриптор mailslot’а
            (LPDWORD)NULL,               // без ограничения размера сообщения
            &cbMessage,                   // размер следующего сообщения
            &cMessage,                    // количество сообщений в ящике 
            (LPDWORD)NULL);              // без таймаута чтения 
    }
    
    if (!fResult) {printf("Ошибка при получении информации о почтовом ящике %d.\n", GetLastError()); return FALSE;}
    if (cbMessage == MAILSLOT_NO_MESSAGE)  { printf("Ожидаем сообщения...\n"); return TRUE; }
    cAllMessages = cMessage;
    while (cMessage != 0)  // Считываем все сообщения
    {
        // Создаем строку с номером сообщения
        StringCchPrintf((LPTSTR)achID,  80, TEXT("\nСообщение #%d из %d\n"), cAllMessages - cMessage + 1, cAllMessages);
        // Выделяем память для сообщения. 
        lpszBuffer = (LPTSTR)GlobalAlloc(GPTR,
            lstrlen((LPTSTR)achID) * sizeof(TCHAR) + cbMessage);
        if (NULL == lpszBuffer)
            return FALSE;
        lpszBuffer[0] = '\0';
        // Считываем сообщение из почтового ящика
        if (rr==0) { fResult = ReadFile(hSlot, lpszBuffer, cbMessage, &cbRead, &ov); }
        if (rr==1) { fResult = ReadFile(hSlot1, lpszBuffer, cbMessage, &cbRead, &ov); }
        if (rr==2) { fResult = ReadFile(hSlot2, lpszBuffer, cbMessage, &cbRead, &ov); }
        if (!fResult)  {
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
        if (rr==0) {
            fResult = GetMailslotInfo(hSlot, // дескриптор mailslot’а
                (LPDWORD)NULL,               // без ограничения размера сообщения
                &cbMessage,                   // размер следующего сообщения
                &cMessage,                    // количество сообщений в ящике 
                (LPDWORD)NULL);              // без таймаута чтения 
        }
        if (rr==1) {
            fResult = GetMailslotInfo(hSlot1, // дескриптор mailslot’а
                (LPDWORD)NULL,               // без ограничения размера сообщения
                &cbMessage,                   // размер следующего сообщения
                &cMessage,                    // количество сообщений в ящике 
                (LPDWORD)NULL);              // без таймаута чтения 
        }
        if (rr==2) {
            fResult = GetMailslotInfo(hSlot2, // дескриптор mailslot’а
                (LPDWORD)NULL,               // без ограничения размера сообщения
                &cbMessage,                   // размер следующего сообщения
                &cMessage,                    // количество сообщений в ящике 
                (LPDWORD)NULL);              // без таймаута чтения 
        }
        if (!fResult){
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
    fResult = WriteFile(hSlott,lpszMessage,(DWORD)(lstrlen(lpszMessage) + 1) * sizeof(TCHAR),&cbWritten,(LPOVERLAPPED)NULL);    
    if (!fResult){
        printf("Ошибка при отправлении сообщения (%d)\n", GetLastError());
    }
    cout << "Сообщение отправлено успешно\n";return TRUE;
}

BOOL WINAPI MakeSlot(LPCTSTR lpszSlotName, LPCTSTR SlotName, int rr) {   
    if (rr==0) {
        hSlot = CreateMailslot(lpszSlotName, 0,    //без максимального размера сообщения  
            MAILSLOT_WAIT_FOREVER,         //без таймаута при операциях   
            (LPSECURITY_ATTRIBUTES)NULL); //без атрибутов безопасности       
        if (hSlot == INVALID_HANDLE_VALUE) { cout << "CreateMailslot не удалось вызвать из-за \n" << GetLastError(); return FALSE; }
        else _tprintf(TEXT("Почтовый ящик %s успешно создан\n"), SlotName);  return TRUE;
    }
    if (rr==1) {
        hSlot1 = CreateMailslot(lpszSlotName, 0,    //без максимального размера сообщения  
            MAILSLOT_WAIT_FOREVER,         //без таймаута при операциях   
            (LPSECURITY_ATTRIBUTES)NULL); //без атрибутов безопасности       
        if (hSlot1 == INVALID_HANDLE_VALUE) { cout << "CreateMailslot не удалось вызвать из-за \n" << GetLastError(); return FALSE; }
        else _tprintf(TEXT("Почтовый ящик %s успешно создан\n"), SlotName);  return TRUE;
    }
    if (rr==2) {
        hSlot2 = CreateMailslot(lpszSlotName, 0,    //без максимального размера сообщения  
            MAILSLOT_WAIT_FOREVER,         //без таймаута при операциях   
            (LPSECURITY_ATTRIBUTES)NULL); //без атрибутов безопасности       
        if (hSlot2 == INVALID_HANDLE_VALUE) { cout << "CreateMailslot не удалось вызвать из-за \n" << GetLastError(); return FALSE; }
        else _tprintf(TEXT("Почтовый ящик %s успешно создан\n"), SlotName);  return TRUE;
    }
}





// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
