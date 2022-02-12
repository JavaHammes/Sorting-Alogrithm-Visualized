// -lgdi32 -luser32 -lkernel32 -lcomctl32 -lm -mwindows
#include <windows.h>
#include <stdio.h>

const char g_szClassName[] = "MainWindow";

const UINT WINDOW_WIDTH = 900;
const UINT WINDOW_HEIGHT = 450;

const UINT HEIGHT_SCALE = 20;
const UINT OFFSET_X = 10;

int arr[] = {10,2,9,3,8,4,7,5,6,1};
const UINT size = sizeof(arr)/sizeof(arr[0]);

void paintRectangles(PAINTSTRUCT ps){
	int i;
	UINT posX;
	UINT height;
	

	for ( i = 0; i < size; i++){
		posX = 10 + OFFSET_X * i;
		height = (WINDOW_HEIGHT - 50) - HEIGHT_SCALE * arr[i];
		Rectangle(ps.hdc, posX, WINDOW_HEIGHT - 50, posX + OFFSET_X, height);
		printf("index: %d\nheight: %d\n", i, height);
	}	

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
    	HDC hdc;
	RECT rcTmp;
	PAINTSTRUCT ps;
	
	switch(msg){
	case WM_CREATE:
		hdc = GetDC(hwnd);

		break;
	case WM_PAINT:
	{
		BeginPaint(hwnd, &ps);

		paintRectangles(ps);

		EndPaint(hwnd, &ps);		
	}
	break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmpLine, int nCmdShow){
	WNDCLASSEX wc;
	HWND hwnd;
	MSG Msg;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH) GetStockObject(GRAY_BRUSH); 
	wc.lpszMenuName = NULL;
	wc.lpszClassName = g_szClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if(!RegisterClassEx(&wc)){
		MessageBox(NULL, "Window Registration Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			g_szClassName,
			"Test Window",
			WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU,
			CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT, 
			NULL, NULL, hInstance, NULL);

	if(hwnd == NULL){
		MessageBox(NULL, "Window Creation Failed", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while(GetMessage(&Msg, NULL, 0, 0) > 0){
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	return Msg.wParam;
}
