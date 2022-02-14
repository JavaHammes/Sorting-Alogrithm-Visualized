// -lgdi32 -luser32 -lkernel32 -lcomctl32 -lm -mwindows
#include <windows.h>
#include <stdio.h>
#include <shellapi.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

const char g_szClassName[] = "MainWindow";

const UINT WINDOW_WIDTH = 1000;
const UINT WINDOW_HEIGHT = 1000;

int heightScale;
int offsetX;
long delay;

int arr[65535]; 
int *array;
int size;

void swap(int *a, int *b);
void randomNumbers(int arr[], int size);
void printArray (int arr[], int size);
int getMax(int arr[], int n);

void selectSortArr(int arr[], int size, HWND hwnd);
void bubbleSortArr(int arr[], int size, HWND hwnd);
void radixSortArr(int arr[], int size, HWND hwnd);

void countSortArr(int arr[], int n, int exp);

void updateWindowAndSleep(HWND hwnd);


void paintRectangles(PAINTSTRUCT ps){
	int i;
	UINT posX;
	UINT height;

	for ( i = 0; i < size; i++){
		posX = offsetX * i;
		height = WINDOW_HEIGHT - heightScale * arr[i];
		Rectangle(ps.hdc, posX, WINDOW_HEIGHT, posX + offsetX, height);
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
	wc.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH); 
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

	srand(time(NULL));
	LPWSTR *szArglist;
  	int nArgs;

   	szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
   	if( NULL == szArglist ){
      		wprintf(L"CommandLineToArgvW() Failed!\n");
     	 	return 0;
   	}else if ( nArgs != 3 ){
      		wprintf(L"Usage: program.exe [SIZE] [DELAY]\n");
		return 0;
	}else{
		int sizeL = _wtoi(szArglist[1]);

		if(1000 % sizeL != 0){
			wprintf(L"Invalid size!\n");
			return 0;
		}	

		delay = _wtoi(szArglist[2]) * 1000;
		randomNumbers(arr, sizeL);
		size = sizeL;
		heightScale = 1000 / size;
		offsetX = WINDOW_WIDTH / size;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	//selectSortArr(arr, size, hwnd);	
	//bubbleSortArr(arr,size,hwnd);
	radixSortArr(arr,size,hwnd);

	InvalidateRect(hwnd, NULL, 1);
	UpdateWindow(hwnd);

	while(GetMessage(&Msg, NULL, 0, 0) > 0){
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	

	return Msg.wParam;
}

void randomNumbers(int arr[], int size){	
	int i,j;

	for ( i = 1; i <= size; i++ ){
		arr[i-1] = i;
	}

	for ( j = size - 1; j > 0; j-- ){
		int k = rand() % (j + 1);

		swap(&arr[j], &arr[k]);
	}
}

void swap(int *a, int *b){
	int *temp = (int*) malloc(sizeof(int));
	*temp = *a;
	*a = *b;
	*b = *temp;
}

int getMax(int arr[], int n){
	int mx = arr[0];
	int i;
	for( i = 1; i < n; i++){
		if (arr[i] > mx){
			mx = arr[i];
		}
	}
	return mx;
}

void countSort(int arr[], int n, int exp, HWND hwnd){
	int output[n];
	int i, count[10] = {0};

	for(i = 0; i < n; i++){
		count[(arr[i] / exp) % 10]++;
	}

	for ( i = 1; i < 10; i++){
		count[i] += count[i-1];
	}

	for( i = n-1; i >= 0; i--){
		output[count[(arr[i] / exp) % 10] -1] = arr[i];
		count[(arr[i] / exp) % 10] --;
	}

	for( i = 0; i < n; i++){
		arr[i] = output[i];
		updateWindowAndSleep(hwnd);
	}
}

void printArray(int arr[], int size){	
	int i;

	for( i = 0; i < size; i++ ){
		printf("%d ", arr[i]);
	}

	printf("\n");
}

void selectSortArr(int arr[], int size, HWND hwnd){

	int i, j, min_idx;

	for ( i = 0; i < size-1; i++ ){
		min_idx = i;
		for ( j = i+1; j < size; j++ ){
			if ( arr[j] < arr [min_idx] ){
				min_idx = j;
			}
		}
		swap(&arr[min_idx], &arr[i]);
		updateWindowAndSleep(hwnd);
	}	
}

void bubbleSortArr(int arr[], int size, HWND hwnd){

	int i, j;

	for( i = 0; i < size-1; i++){
		for(j = 0; j < size-i-1; j++){
			if(arr[j] > arr[j+1]){
				printArray(arr, size);
				swap(&arr[j], &arr[j+1]);
				updateWindowAndSleep(hwnd);
			}
		}
	}
}

void radixSortArr(int arr[], int size, HWND hwnd){

	int m = getMax(arr, size);
	int exp;

	for(exp = 1; m /exp > 0; exp *= 10){
		countSort(arr, size, exp, hwnd);
	}
}

void updateWindowAndSleep(HWND hwnd){
	InvalidateRect(hwnd, NULL, 1);
	UpdateWindow(hwnd);

	if(delay <= 100000){
		usleep(delay);
	}else{
		sleep(delay / 1000000);
	}
}
