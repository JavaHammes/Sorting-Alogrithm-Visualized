// -lgdi32 
#include <windows.h>
#include <stdio.h>
#include <shellapi.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <unistd.h>

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 1000

const char g_szClassName[] = "MainWindow";

int heightScale;
int offsetX;
long delay;
int algo;

int arr[65535]; 
int size;

void swap(int *a, int *b);
void randomNumbers(int arr[], int size);
void printArray (int arr[], int size);
int getMax(int arr[], int n);

void selectSortArr(int arr[], int size, HWND hwnd);
void bubbleSortArr(int arr[], int size, HWND hwnd);
void radixSortArr(int arr[], int size, HWND hwnd);
void insertionSortArr(int arr[], int size, HWND hwnd);
void heapSortArr(int arr[], int size, HWND hwnd);
void quickSortArr(int arr[], int low, int high, HWND hwnd);
void bucketSortArr(int arr[], int size, HWND hwnd);

void countSort(int arr[], int size, int exp, HWND hwnd);
void heapify(int arr[], int size, int i);
int partition(int arr[], int low, int high);

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
	printf("  ___          _   _           \n");
	printf(" / __| ___ _ _| |_(_)_ _  __ _ \n");  
	printf(" |__ |/ _ | '_|  _| | ' || _` |\n");  
	printf(" |___/|___/_|  |__|_|_||_|__, |\n");  
	printf("                         |___/ \n");  
	printf("--------------------------------------\n");
	printf("Enter amount of rectangles to be sorted:\n");	
	printf("--------------------------------------\n");
	printf("10 || 20 || 25 || 40 || 50 || 100 || 200 || 250\n");
	printf("--------------------------------------\n");
	printf(">> ");
	scanf("%d", &size);
	printf("--------------------------------------\n");
	if(1000 % size != 0 && size >= 10){
		printf("Invalid Size!");
	}

	randomNumbers(arr, size);
	heightScale = 1000 / size;
	offsetX = WINDOW_WIDTH / size;

	printf("Enter amount of delay (in millisec):\n");
	printf("--------------------------------------\n");
	printf(">> ");
	scanf("%d", &delay);
	printf("--------------------------------------\n");

	delay *= 1000;

	printf("Enter sorting algorithm:\n");
	printf("--------------------------------------\n");
	printf("[1] SELECTION SORT \n[2] BUBBLE SORT \n[3] RADIX SORT\n[4] INSERTION SORT\n[5] HEAP SORT\n[6] QUICK SORT\n[7] BUCKET SORT\n");
	printf("--------------------------------------\n");
	printf(">> ");
	scanf("%d", &algo);
	printf("--------------------------------------\n");

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	
	if(algo == 1){
		selectSortArr(arr, size, hwnd);
	}else if(algo == 2){
		bubbleSortArr(arr, size, hwnd);
	}else if(algo == 3){
		radixSortArr(arr, size, hwnd);
	}else if(algo == 4){
		insertionSortArr(arr, size, hwnd);
	}else if(algo == 5){
		heapSortArr(arr, size, hwnd);
	}else if(algo == 6){
		quickSortArr(arr, 0, size -1, hwnd);
	}else if(algo == 7){
		bucketSortArr(arr, size, hwnd);
	}else{
		printf("Wrong Input\n");
	}

	InvalidateRect(hwnd, NULL, 1);
	UpdateWindow(hwnd);

	printf("FINISHED !!! \n");
	printf("--------------------------------------\n");

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

void countSort(int arr[], int size, int exp, HWND hwnd){
	int output[size];
	int i, count[10] = {0};

	for(i = 0; i < size; i++){
		count[(arr[i] / exp) % 10]++;
	}

	for ( i = 1; i < 10; i++){
		count[i] += count[i-1];
	}

	for( i = size-1; i >= 0; i--){
		output[count[(arr[i] / exp) % 10] -1] = arr[i];
		count[(arr[i] / exp) % 10] --;
	}

	for( i = 0; i < size; i++){
		arr[i] = output[i];
		printArray(arr, size);
		updateWindowAndSleep(hwnd);
	}
}

void heapify(int arr[], int size, int i){
	int largest = i;
	int l = 2 * i +1;
	int r = 2 * i +2;

	if(l < size && arr[l] > arr[largest]){
		largest = l;
	}

	if(r < size && arr[r] > arr[largest]){
		largest = r;
	}

	if(largest != i){
		swap(&arr[i], &arr[largest]);
		heapify(arr, size, largest);
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
				swap(&arr[j], &arr[j+1]);
				printArray(arr, size);
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

void insertionSortArr(int arr[], int size, HWND hwnd){

	int i, key, j;
	for(i = 1; i < size; i++){
		key = arr[i];
		j = i - 1;

		while(j >= 0 && arr[j] > key) {
			arr[j + 1] = arr[j];
			j = j -1;
		}
		arr[j + 1] = key;
		updateWindowAndSleep(hwnd);
	}
}

void heapSortArr(int arr[], int size, HWND hwnd){
	
	int i;

	for(i = size / 2 - 1; i >= 0; i--){
		heapify(arr, size, i);
	}

	for(i = size - 1; i > 0; i--){
		swap(&arr[0], &arr[i]);
		updateWindowAndSleep(hwnd);
		heapify(arr, i , 0);
	}
}

int partition(int arr[], int low, int high){
	int pivot = arr[high];
	int i = (low -1);
	int j;

	for(j = low; j <= high -1; j++){
		if(arr[j] < pivot){
			i++;
			swap(&arr[i], &arr[j]);
		}
	}
	swap(&arr[i+1], &arr[high]);
	return (i+1);
}

void quickSortArr(int arr[], int low, int high, HWND hwnd){
	if(low < high){
		updateWindowAndSleep(hwnd);
		int pi = partition(arr, low, high);

		quickSortArr(arr, low, pi - 1, hwnd);
		quickSortArr(arr, pi+1, high, hwnd);
	}
}

void bucketSortArr(int arr[], int size, HWND hwnd){

	int * cpy = malloc(size * sizeof(int));
	memcpy(cpy, arr, size * sizeof(int));
	int i, j;
	int count[size];
	for( i = 0; i < size; i++){
		count[i] = 0;
	}

	for(i = 0; i < size; i++){
		(count[arr[i]])++;
	}

	for(i = 0, j = 0; i < size; i++){
		for(; count[i] > 0; (count[i])--){
			arr[j++] = i;
		}
		updateWindowAndSleep(hwnd);
	}

	arr[size-1] = getMax(cpy, size);
	updateWindowAndSleep(hwnd);

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
