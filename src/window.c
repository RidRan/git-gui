#include <windef.h>
#include <winuser.h>
#include <wingdi.h>

#include "input.c"

const char g_szClassName[] = "myWindowClass";
const int ID_TIMER = 1;

int marginX = 20;
int marginY = 43;

void DrawScreenToDisplay(HWND hwnd) {
    RECT rcClient;

    HDC hdc = GetDC(hwnd);
    HDC sdc = CreateCompatibleDC(hdc);
    
    GetClientRect(hwnd, &rcClient);

    HBITMAP hbmp = CreateBitmap(screen->width, screen->height, 1, 8*4, screen->pixels);
    SelectObject(sdc, hbmp);
    StretchBlt(hdc, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, sdc, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SRCCOPY);

	ResetScreen();

    DeleteDC(sdc);
    ReleaseDC(hwnd, hdc);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
    PAINTSTRUCT ps;          // paint data for BeginPaint and EndPaint  
    POINT ptClientUL;        // client area upper left corner  
    POINT ptClientLR;        // client area lower right corner  
    static POINT pt;         // x and y coordinates of cursor   
    static RECT rcClient;   // client-area rectangle  
	static RECT rcWindow;    // window-area rectangle

	switch(msg)
	{
		case WM_CREATE:
		{
			onStart();

            UINT ret;
            ret = SetTimer(hwnd, ID_TIMER, 16, NULL);
            if(ret == 0)
                MessageBox(hwnd, "Could not SetTimer()!", "Error", MB_OK | MB_ICONEXCLAMATION);
		}
		break;
		case WM_DESTROY:
			KillTimer(hwnd, ID_TIMER);

			PostQuitMessage(0);
		break;

		case WM_MOVE:
		break;
		case WM_SIZE: 
			GetClientRect(hwnd, &rcClient); 
			GetWindowRect(hwnd, &rcWindow);
			marginY = (rcWindow.bottom - rcWindow.top) - (rcClient.bottom - rcClient.top);
			marginX = (rcWindow.right - rcWindow.left) - (rcClient.right - rcClient.left);
			ptClientUL.x = rcClient.left; 
			ptClientUL.y = rcClient.top; 
			ptClientLR.x = rcClient.right; 
			ptClientLR.y = rcClient.bottom; 
			ClientToScreen(hwnd, &ptClientUL); 
			ClientToScreen(hwnd, &ptClientLR); 
			SetRect(&rcClient, ptClientUL.x, ptClientUL.y, ptClientLR.x, ptClientLR.y); 
      	break;
		case WM_GETMINMAXINFO:
		{
			// LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
			// lpMMI->ptMinTrackSize.x = SCREEN_WIDTH + marginX;
			// lpMMI->ptMinTrackSize.y = SCREEN_HEIGHT + marginY;
		}
		break;

		case WM_PAINT:
		{
			hdc = BeginPaint(hwnd, &ps);

			DrawScreenToDisplay(hwnd);
			GetClientRect(hwnd, &rcClient); 

			EndPaint(hwnd, &ps);
		}
		break;
		case WM_CLOSE:
			DestroyWindow(hwnd);
		break;
		case WM_QUIT:
		break;

		case WM_SYSCOMMAND:
		break;
		case WM_TIMER:
		{
            onTimer();
			DrawScreenToDisplay(hwnd);
		}
		break;


		case WM_SETCURSOR:
		break;
		case WM_WINDOWPOSCHANGING:
		break;
		case WM_WINDOWPOSCHANGED:
		break;
		case WM_GETICON:
		break;
		case WM_SETICON:
		break;
		case WM_MOUSEMOVE:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);

			GetClientRect(hwnd, &rcClient); 
	
            x = x * screen->width / (rcClient.right - rcClient.left) - rcClient.left + originOffset.x;
            y = y * screen->height / (rcClient.bottom - rcClient.top) - rcClient.top + originOffset.y;

			onMouseMove(x, y);
		}
		break;
		case WM_LBUTTONDOWN:
		{
            int x = LOWORD(lParam);
			int y = HIWORD(lParam);
	
            x = x * screen->width / (rcClient.right - rcClient.left) - rcClient.left + originOffset.x;
            y = y * screen->height / (rcClient.bottom - rcClient.top) - rcClient.top + originOffset.y;

            onMouseLeftDown(x, y);
        }
		break;
		case WM_LBUTTONUP:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
	
            x = x * screen->width / (rcClient.right - rcClient.left) - rcClient.left;
            y = y * screen->height / (rcClient.bottom - rcClient.top) - rcClient.top;

            onMouseLeftUp(x, y);
        }
		break;
		case WM_CAPTURECHANGED:
		break;
		case WM_MOVING:
		break;
		case WM_ENTERSIZEMOVE:
		break;
		case WM_EXITSIZEMOVE:
		break;

		case WM_NCHITTEST:
		break;
		case WM_NCMOUSEMOVE:
		break;
		case WM_NCMOUSELEAVE:
		//calls when the cursor leaves and enters the window
		break;
		case WM_NCLBUTTONDOWN:
		break;

		case WM_KEYDOWN:
			printf("Key down: 0x%x\n", (char) wParam);
		break;
		case WM_KEYUP:
			printf("Key up: 0x%x\n", wParam);
		break;
		case WM_CHAR:
			onKeyPress((char) wParam);
		break;
		
		default:
			printf("%d\n", msg);
			break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc;
	HWND hwnd;
	MSG Msg;

	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.style		 = 0;
	wc.lpfnWndProc	 = WndProc;
	wc.cbClsExtra	 = 0;
	wc.cbWndExtra	 = 0;
	wc.hInstance	 = hInstance;
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = g_szClassName;
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION);

	if(!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Window Registration Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		g_szClassName,
		"Test",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, SCREEN_WIDTH + marginX, SCREEN_HEIGHT + marginY,
		NULL, NULL, hInstance, NULL);

	if(hwnd == NULL)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while(GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}