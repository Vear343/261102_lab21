#include <windows.h>
#include <string>
#include <stdio.h>
#include <iostream>

HWND textfield, TextBox_1, TextBox_2, plus, minus, multiple, divine;

char szClassName[] = "TextEntry";
char N_1[64];
char N_2[64];

/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch(Message) {
		case WM_CREATE:

		textfield = CreateWindowA("STATIC",
								 "Please input two numbers",
								 WS_VISIBLE | WS_CHILD,
								 20, 20, 180, 25,
								 hwnd, NULL, NULL, NULL);

		TextBox_1 = CreateWindowA("EDIT",
								"",
								WS_BORDER | WS_CHILD | WS_VISIBLE | ES_NUMBER,
								40, 50, 140, 25,
								hwnd, NULL, NULL, NULL);

		TextBox_2 = CreateWindowA("EDIT",
								"",
								WS_BORDER | WS_CHILD | WS_VISIBLE | ES_NUMBER,
								40, 80, 140, 25,
								hwnd, NULL, NULL, NULL);

		plus = CreateWindowA("BUTTON",
								 "+",
								 WS_VISIBLE | WS_CHILD | WS_BORDER,
								 50, 120, 25, 25,
								 hwnd, (HMENU) 1, NULL, NULL);

		minus = CreateWindowA("BUTTON",
								 "-",
								 WS_VISIBLE | WS_CHILD | WS_BORDER,
								 85, 120, 25, 25,
								 hwnd, (HMENU) 2, NULL, NULL);
		multiple = CreateWindowA("BUTTON",
								 "*",
								 WS_VISIBLE | WS_CHILD | WS_BORDER,
								 120, 120, 25, 25,
								 hwnd, (HMENU) 3, NULL, NULL);
		divine = CreateWindowA("BUTTON",
								 "/",
								 WS_VISIBLE | WS_CHILD | WS_BORDER,
								 155, 120, 25, 25,
								 hwnd, (HMENU) 4, NULL, NULL);
		break;
		
		case WM_COMMAND:
		{
			int id = LOWORD(wParam);

			// Only react to buttons (1–4)
			if (id < 1 || id > 4)
				return 0;

			double num_1, num_2, result_f;
			char result_t[64];

			GetWindowTextA(TextBox_1, N_1, 64);
			GetWindowTextA(TextBox_2, N_2, 64);

			if (strlen(N_1) == 0 || strlen(N_2) == 0)
			{
				MessageBoxA(hwnd, "Please enter both numbers!", "Error", MB_OK);
				return 0;
			}

			try
			{
				num_1 = std::stod(N_1);
				num_2 = std::stod(N_2);
			}
			catch (...)
			{
				MessageBoxA(hwnd, "Invalid number format!", "Error", MB_OK);
				return 0;
			}

			switch (id)
			{
			case 1:
				result_f = num_1 + num_2;
				break; 
			case 2:
				result_f = num_1 - num_2;
				break;
			case 3:
				result_f = num_1 * num_2; 
				break;
			case 4:
				if (num_2 == 0)
				{
					MessageBoxA(hwnd, "Cannot divide by zero!", "Error", MB_OK);
					return 0;
				}
				result_f = num_1 / num_2;
				break;
			}

			sprintf(result_t, "%.2f", result_f);
			MessageBoxA(hwnd, result_t, "Result", MB_OK);
			break;

		}
		break;

		/* Upon destruction, tell the main thread to stop */
		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}
		
		/* All other messages (a lot of them) are processed using default procedures */
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

/* The 'main' function of Win32 GUI programs: this is where execution starts */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc; /* A properties struct of our window */
	HWND hwnd; /* A 'HANDLE', hence the H, or a pointer to our window */
	MSG msg; /* A temporary location for all messages */

	/* zero out the struct and set the stuff we want to modify */
	memset(&wc,0,sizeof(wc));
	wc.cbSize	 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WndProc; /* This is where we will send messages to */
	wc.hInstance	 = hInstance;
	wc.hCursor	 = LoadCursor(NULL, IDC_ARROW);
	
	/* White, COLOR_WINDOW is just a #define for a system color, try Ctrl+Clicking it */
	wc.hbrBackground = CreateSolidBrush(RGB(255,69,0));
	wc.lpszClassName = "WindowClass";
	wc.hIcon	 = LoadIcon(NULL, IDI_APPLICATION); /* Load a standard icon */
	wc.hIconSm	 = LoadIcon(NULL, IDI_APPLICATION); /* use the name "A" to use the project icon */

	if(!RegisterClassEx(&wc)) {
		MessageBoxA(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	hwnd = CreateWindowExA(WS_EX_CLIENTEDGE,"WindowClass","My Calculator",WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, /* x */
		CW_USEDEFAULT, /* y */
		250, /* width */
		200, /* height */
		NULL,NULL,hInstance,NULL);

	if(hwnd == NULL) {
		MessageBoxA(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	/*
		This is the heart of our program where all input is processed and 
		sent to WndProc. Note that GetMessage blocks code flow until it receives something, so
		this loop will not produce unreasonably high CPU usage
	*/
	while(GetMessage(&msg, NULL, 0, 0) > 0) { /* If no error is received... */
		TranslateMessage(&msg); /* Translate key codes to chars if present */
		DispatchMessage(&msg); /* Send it to WndProc */
	}
	return msg.wParam;
}