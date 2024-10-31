#pragma once

#include <windows.h>

volatile LONG counter = 0; // Global counter to track timer interrupts

// Timer interrupt handler
VOID CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
    // Increment and print the counter
    InterlockedIncrement(&counter);
    std::cout << "Timer Interrupt: " << counter << std::endl;
    std::cout.flush();
}

void TimerInterruptTest()
{
    // Set up a timer that calls TimerProc every 1000 milliseconds (1 second)
    UINT_PTR timerId = SetTimer(NULL, 0, 1000, TimerProc);
    if (timerId == 0)
    {
        std::cerr << "Failed to create timer." << std::endl;
        return;
    }

    std::cout << "Timer started." << std::endl;

    int mainCounter = 0;

    // Main message loop
    MSG msg;
    while (true)
    {
        // Check for messages in the queue
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        mainCounter++;
        if(mainCounter >= 4)
        {
            mainCounter = 0;
            std::cout << "mainCounter : " << counter << std::endl;
        }
        Sleep(50);
    }

    // Clean up the timer
    KillTimer(NULL, timerId);
    std::cout << "Timer stopped." << std::endl;

    return;
}