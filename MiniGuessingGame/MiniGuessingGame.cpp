// MiniGuessingGame.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "MiniGuessingGame.h"

#include <windowsx.h>
#include <CommCtrl.h>
#include <gdiplus.h>
#include <vector>
#include <time.h>
#include <commdlg.h>

#pragma comment (lib, "gdiplus")
using namespace Gdiplus;
using namespace std;
#pragma comment(lib, "ComCtl32.lib")

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK DlgInputName_WndProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK DlgSetCountDown_WndProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);


BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void OnPaint(HWND hwnd);
void OnDestroy(HWND hwnd);



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	ULONG_PTR token;
	GdiplusStartupInput input = { 0 };
	input.GdiplusVersion = 1;
	GdiplusStartup(&token, &input, NULL);

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MINIGUESSINGGAME, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MINIGUESSINGGAME));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

	GdiplusShutdown(token);

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MINIGUESSINGGAME));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_BTNFACE+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MINIGUESSINGGAME);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 700, 500, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	InitCommonControls();
    switch (message)
    {
		HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
		HANDLE_MSG(hWnd, WM_PAINT, OnPaint);
		HANDLE_MSG(hWnd, WM_COMMAND, OnCommand);
		HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

// =============================================================
void DecreaseSeconds(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
void NextQuestion(HWND hwnd);


// ======================= MY VARIABLES =========================

#define bufferSizeQuestion 300
#define BUFFERSIZE 256

RECT client_Rect;

HWND btnStart, btnReset;

HWND hButtonSubmit1, hButtonSubmit2, hImageLabel, hQuestion;

HWND lbMinute, lbSecond;

HWND txbRound, txbScore, txbPlayer, txbHighest;

int currentRound, totalRounds = 10;
int totalTime = 10; // second
int currentSecond;

WCHAR iniPath[BUFFERSIZE] = L"config.ini";

Rect *RectDraw;

WCHAR fileName[] = L"config.ini";

bool isFirstStarted = true;
int posQuestion = -1;

int trueAnswerPos = 1;	// 1: button 1, 2: button 2

int lastPosAnswer = -1, lastPosTrueAnswer = -1;

int totalScore = 0;

vector<int> listScoreBySeconds;

WCHAR currentPlayer[20];

bool isPlaying = false;

struct SQuestion {
	WCHAR _question[bufferSizeQuestion];
	WCHAR _answer[bufferSizeQuestion];
	WCHAR _link[bufferSizeQuestion];
};

vector<SQuestion> ListQuestion;

// ======================= MY FUNCTIONS =========================

#pragma region My Functions

int getCurrentPath(WCHAR fileName[], WCHAR destination[])
{
	WCHAR curPath[BUFFERSIZE];

	// Tạo đường dẫn tuyệt đối tới file config
	int result = GetCurrentDirectory(BUFFERSIZE, curPath);
	wsprintf(destination, L"%s\\%s", curPath, fileName);
	return result;
}

void loadRound()
{
	WCHAR strBuffer[20];

	swprintf_s(strBuffer, sizeof(strBuffer) / sizeof(WCHAR), L"%d / %d", currentRound, totalRounds);

	SetWindowText(txbRound, strBuffer);
}

int InitQuestion()
{
	WCHAR buffer[bufferSizeQuestion];
	int count = 1;
	WCHAR bufferQuestion[bufferSizeQuestion], bufferAnswer[bufferSizeQuestion], bufferLink[bufferSizeQuestion];
	while (true)
	{
		wsprintf(bufferQuestion, L"question%d", count);
		wsprintf(bufferAnswer, L"answer%d", count);
		wsprintf(bufferLink, L"linkImg%d", count);

		SQuestion tmpQuestion;

		GetPrivateProfileString(L"section", bufferQuestion, L"Not Exists",
			tmpQuestion._question, bufferSizeQuestion, iniPath);
		GetPrivateProfileString(L"section", bufferAnswer, L"Not Exists",
			tmpQuestion._answer, bufferSizeQuestion, iniPath);
		GetPrivateProfileString(L"section", bufferLink, L"Not Exists",
			tmpQuestion._link, bufferSizeQuestion, iniPath);
		
		if (wcscmp(tmpQuestion._question, L"Not Exists") == 0 ||
			wcscmp(tmpQuestion._answer, L"Not Exists") == 0 ||
			wcscmp(tmpQuestion._link, L"Not Exists") == 0)
		{
			if (count == 1)
			{
				return -1;
			}
			else
				return count;
		}

		count++;

		ListQuestion.push_back(tmpQuestion);
	}
	return count;
}

void loadPlayerName()
{

}

void loadHightestScore()
{
	WCHAR buffer[BUFFERSIZE];
	WCHAR curPath[BUFFERSIZE];
	WCHAR configPath[BUFFERSIZE];

	// Tạo đường dẫn tuyệt đối tới file config
	GetCurrentDirectory(BUFFERSIZE, curPath);
	wsprintf(configPath, L"%s\\config.ini", curPath);

	WCHAR tmpGetScore[10];
	GetPrivateProfileString(L"app", L"highestScore", L"0", tmpGetScore, 10, configPath);

	SetWindowText(txbHighest, tmpGetScore);
}

void FormatText(WCHAR buffer[3]) {
	if (buffer[1] == '\0') {
		buffer[2] = '\0';
		buffer[1] = buffer[0];
		buffer[0] = '0';
	}
}

void DefaultTimer()
{
	currentSecond = totalTime;
	WCHAR buffer[3];
	wsprintf(buffer, L"%d", currentSecond / 60);
	FormatText(buffer);
	SetWindowText(lbMinute, buffer);
	wsprintf(buffer, L"%d", currentSecond % 60);
	FormatText(buffer);
	SetWindowText(lbSecond, buffer);
}

void DecreaseSeconds(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	currentSecond--;
	WCHAR buffer[3];

	wsprintf(buffer, L"%d", currentSecond / 60);
	FormatText(buffer);
	SetWindowText(lbMinute, buffer);
	wsprintf(buffer, L"%d", currentSecond % 60);
	FormatText(buffer);
	SetWindowText(lbSecond, buffer);

	if (currentSecond == 0)
	{
		KillTimer(hwnd, IDT_TIMER1);
		currentRound++;

		if (currentRound <= totalRounds)
		{
			NextQuestion(hwnd);
		}
		else
		{
			EnableWindow(hButtonSubmit1, false);
			EnableWindow(hButtonSubmit2, false);
		}

		InvalidateRect(hwnd, NULL, TRUE);
	}
}

void InitTimer(HWND hwnd)
{
	DefaultTimer();

	SetTimer(hwnd, IDT_TIMER1, 1000, (TIMERPROC)DecreaseSeconds);
}

#pragma endregion


BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
	LOGFONT lf;
	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
	HFONT hFont = CreateFont(lf.lfHeight, lf.lfWidth,
		lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
		lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
		lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
		lf.lfPitchAndFamily, lf.lfFaceName);

	// lay kich thuoc man hinh
	GetClientRect(hwnd, &client_Rect);

	// tao title cho chuong trinh
	HWND title = CreateWindowEx(NULL, L"STATIC", L"LEARNING ENGLISH", WS_CHILD | WS_VISIBLE
		| SS_CENTER, (client_Rect.right-500)/2 , 10, 500, 40, hwnd, NULL, hInst, NULL);
	
	HFONT fontTitle = CreateFont(30, lf.lfWidth,
		lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
		lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
		lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
		lf.lfPitchAndFamily, lf.lfFaceName);

	SetWindowFont(title, fontTitle, TRUE);


	#pragma region Panel Game Play

	// panel 1
	HWND panel_Play = CreateWindowEx(NULL, L"STATIC", L"", WS_CHILD | WS_BORDER
		| SS_CENTER, 0, 50, client_Rect.right * 0.65, client_Rect.bottom - 50, hwnd, NULL, hInst, NULL);
	
	RECT panel_Play_rect;
	GetClientRect(panel_Play, &panel_Play_rect);

	// rect to draw
	RectDraw = new Rect((panel_Play_rect.right - 230) / 2, 170, 230, 130);
	
	#pragma region Question
	// question
	hQuestion = CreateWindowEx(NULL, L"STATIC", L" QUESTION: ", WS_CHILD | WS_VISIBLE | WS_BORDER
		,20 , 60, panel_Play_rect.right - 40, 70, hwnd, NULL, hInst, NULL);
	
	SetWindowFont(hQuestion, hFont, TRUE);
	#pragma endregion


	#pragma region Button 1
	// button 1
	hButtonSubmit1 = CreateWindowEx(NULL, L"BUTTON", L"", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		130, 340, 80, 30, hwnd, (HMENU)IDC_BTN_CHOOSE1, hInst, NULL);
	SetWindowFont(hButtonSubmit1, hFont, TRUE);

	EnableWindow(hButtonSubmit1, false);
	#pragma endregion

	#pragma region Button 2
	// button 2
	hButtonSubmit2 = CreateWindowEx(NULL, L"BUTTON", L"", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		230, 340, 80, 30, hwnd, (HMENU)IDC_BTN_CHOOSE2, hInst, NULL);
	SetWindowFont(hButtonSubmit2, hFont, TRUE);

	EnableWindow(hButtonSubmit2, false);
	#pragma endregion

	#pragma endregion

	

	#pragma region PanelInfo

	HWND panel_Info = CreateWindowEx(NULL, L"STATIC", L"", WS_CHILD | WS_VISIBLE | WS_BORDER
		| SS_CENTER, client_Rect.right * 0.65, 50, client_Rect.right * 0.35,
		client_Rect.bottom - 50, hwnd, NULL, hInst, NULL);

	RECT panelInfo_Rect;
	GetClientRect(panel_Info, &panelInfo_Rect);


		#pragma region Panel Timer

		HWND panel_Timer = CreateWindowEx(NULL, L"STATIC", L"", WS_CHILD | WS_VISIBLE | WS_BORDER
			| SS_CENTER, panel_Play_rect.right + 10, 50 + 10, panelInfo_Rect.right - 20,
			70, hwnd, NULL, hInst, NULL);

		RECT panel_Timer_Rect;
		GetClientRect(panel_Timer, &panel_Timer_Rect);

		// minute
		lbMinute = CreateWindowEx(NULL, L"STATIC", L"00", WS_CHILD | WS_VISIBLE,
			panel_Play_rect.right + 10 + 50, 60 + (panel_Timer_Rect.bottom - 25) / 2, 30, 40,
			hwnd, NULL, hInst, NULL);

		SetWindowFont(lbMinute, fontTitle, true);

		// colon
		HWND lbColon = CreateWindowEx(NULL, L"STATIC", L":", WS_CHILD | WS_VISIBLE| SS_CENTER,
			panel_Play_rect.right + 10 + 50 + 30, 58 + (panel_Timer_Rect.bottom - 25) / 2, 30, 40,
			hwnd, NULL, hInst, NULL);

		SetWindowFont(lbColon, fontTitle, true);

		// second
		lbSecond = CreateWindowEx(NULL, L"STATIC", L"00", WS_CHILD | WS_VISIBLE,
			panel_Play_rect.right + 10 + 50 + 30 + 30, 60 + (panel_Timer_Rect.bottom - 25) / 2, 30, 40,
			hwnd, NULL, hInst, NULL);

		SetWindowFont(lbSecond, fontTitle, true);

		#pragma endregion



		#pragma region Panel Show info Game Play

		HWND panel_ShowInFoGame = CreateWindowEx(NULL, L"STATIC", L"", WS_CHILD | WS_VISIBLE | WS_BORDER
			| SS_CENTER, panel_Play_rect.right + 10, 140, panelInfo_Rect.right - 20,
			180, hwnd, NULL, hInst, NULL);

		// show round
		HWND lbRound = CreateWindowEx(NULL, L"STATIC", L"Round:", WS_CHILD | WS_VISIBLE,
			panel_Play_rect.right + 10 + 10, 150, 70, 40, hwnd, NULL, hInst, NULL);

		SetWindowFont(lbRound, hFont, true);

		txbRound = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | SS_CENTER,
			panel_Play_rect.right + 10 + 10 + 70 + 10, 145, 110, 20,
			hwnd, (HMENU)IDC_EDIT_ROUND, hInst, NULL);
		SetWindowFont(txbRound, hFont, true);

		EnableWindow(txbRound, false);

		// show score
		HWND lbScore = CreateWindowEx(NULL, L"STATIC", L"Score:", WS_CHILD | WS_VISIBLE,
			panel_Play_rect.right + 10 + 10, 190,
			70, 30, hwnd, NULL, hInst, NULL);

		SetWindowFont(lbScore, hFont, true);

		txbScore = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | SS_CENTER,
			panel_Play_rect.right + 10 + 10 + 70 + 10, 185,
			110, 20, hwnd, (HMENU)IDC_EDIT_SCORE, hInst, NULL);
		SetWindowFont(txbScore, hFont, true);
		EnableWindow(txbScore, false);

		// show player
		HWND lbPlayer = CreateWindowEx(NULL, L"STATIC", L"Player:", WS_CHILD | WS_VISIBLE, 
			panel_Play_rect.right + 10 + 10, 230,
			70, 30, hwnd, NULL, hInst, NULL);

		SetWindowFont(lbPlayer, hFont, true);

		txbPlayer = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | SS_CENTER,
			panel_Play_rect.right + 10 + 10 + 70 + 10, 225,
			110, 20, hwnd, (HMENU)IDC_EDIT_PLAYER, hInst, NULL);
		SetWindowFont(txbPlayer, hFont, true);
		EnableWindow(txbPlayer, false);

		// show highest score
		HWND lbHightest = CreateWindowEx(NULL, L"STATIC", L"Highest Score:", WS_CHILD | WS_VISIBLE, 
			panel_Play_rect.right + 10 + 10, 270,
			70, 30, hwnd, NULL, hInst, NULL);

		SetWindowFont(lbHightest, hFont, true);

		txbHighest = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | SS_CENTER,
			panel_Play_rect.right + 10 + 10 + 70 + 10, 265,
			110, 20, hwnd, (HMENU)IDC_EDIT_HIGHEST, hInst, NULL);
		SetWindowFont(txbHighest, hFont, true);
		EnableWindow(txbHighest, false);
		#pragma endregion



		#pragma region Panel Button Controls
		HWND panel_ButtonControl = CreateWindowEx(NULL, L"STATIC", L"", WS_CHILD | WS_VISIBLE | WS_BORDER
			| SS_CENTER, 
			panel_Play_rect.right + 10, 330, panelInfo_Rect.right - 20, panelInfo_Rect.bottom - 10 - 280,
			hwnd, NULL, hInst, NULL);

		btnStart = CreateWindowEx(NULL, L"BUTTON", L"Start", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			panel_Play_rect.right + 10 + 10, 340, 60, 30, hwnd, (HMENU)IDC_BTN_START, hInst, NULL);

		SetWindowFont(btnStart, hFont, TRUE);

		btnReset = CreateWindowEx(NULL, L"BUTTON", L"Reset", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			panel_Play_rect.right + 10 + 10 + 60 + 10, 340, 60, 30, hwnd, (HMENU)IDC_BTN_RESET, hInst, NULL);

		SetWindowFont(btnReset, hFont, TRUE);
		EnableWindow(btnReset, false);
		#pragma endregion


#pragma endregion

	return TRUE;
}

void NextQuestion(HWND hwnd)
{
	WCHAR tmp[BUFFERSIZE];
	int randomOtherAnswer = -1;

	// init timer
	InitTimer(hwnd);

	// load round
	loadRound();

	// load question
	do
	{
		posQuestion = rand() % ListQuestion.size();
	} while (posQuestion == lastPosTrueAnswer);

	lastPosTrueAnswer = posQuestion;

	wsprintf(tmp, L" QUESTION: %s", ListQuestion[posQuestion]._question);

	SetWindowText(hQuestion, tmp);

	// random answers
	trueAnswerPos = rand() % 2 + 1;

	do {
		randomOtherAnswer = rand() % ListQuestion.size();
	} while (randomOtherAnswer == posQuestion || randomOtherAnswer == lastPosAnswer
		|| randomOtherAnswer == lastPosTrueAnswer);

	lastPosAnswer = randomOtherAnswer;

	if (trueAnswerPos == 1)
	{
		SetWindowText(hButtonSubmit1, ListQuestion[posQuestion]._answer);
		SetWindowText(hButtonSubmit2, ListQuestion[randomOtherAnswer]._answer);
	}
	else
	{
		SetWindowText(hButtonSubmit1, ListQuestion[randomOtherAnswer]._answer);
		SetWindowText(hButtonSubmit2, ListQuestion[posQuestion]._answer);
	}
}

void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	WCHAR tmp[BUFFERSIZE];
	int result;
	int isLoaded;
	switch (id)
	{
	case IDC_BTN_START:

		srand(time(NULL));

		if (wcslen(currentPlayer) == 0)
		{
			MessageBox(hwnd, L"Please enter your name before playing game!", L"Notification", MB_OK | MB_ICONINFORMATION);
			break;
		}

		currentRound = 1;
		totalScore = 0;

		if (isFirstStarted == true)
		{
			// get current path at the first time start

			result = getCurrentPath(fileName, iniPath);
			if (result == 0)
			{
				MessageBox(0, L"Cant open config.ini. Program will be quit!", L"Notification", MB_OK | MB_ICONINFORMATION);
				delete RectDraw;
				PostQuitMessage(0);
			}

			// init list question at the first time start
			isLoaded = InitQuestion();
			if (isLoaded == -1)
			{
				MessageBox(0, L"Cant load questions. Program will be quit!", L"Notification", MB_OK | MB_ICONINFORMATION);
				delete RectDraw;
				PostQuitMessage(0);
			}

			isFirstStarted = false;
		}
		// game start
		isPlaying = true;

		// load player name
		SetWindowText(txbPlayer, currentPlayer);

		// load hightest score
		loadHightestScore();
		
		// load score
		wsprintf(tmp, L"%d", totalScore);
		SetWindowText(txbScore, tmp);

		// next question
		NextQuestion(hwnd);

		// disbale button start, enable submit 1, submit 2, reset
		EnableWindow(btnStart, false);
		EnableWindow(hButtonSubmit1, true);
		EnableWindow(hButtonSubmit2, true);
		EnableWindow(btnReset, true);

		InvalidateRect(hwnd, NULL, TRUE);

		break;
	case IDC_BTN_RESET:

		isPlaying = false;

		// kill timer
		KillTimer(hwnd, IDT_TIMER1);

		// ask save: check highest score . if > write highest score to ini

		GetWindowText(txbHighest, tmp, BUFFERSIZE);
		
		if (totalScore > wcstol(tmp, NULL, 10))
		{
			wsprintf(tmp, L"Congratulation %s. You just have gotten highest score.\n Do you wanna save your score?",currentPlayer);
			result = MessageBox(hwnd, tmp, L"Notification", MB_OKCANCEL | MB_ICONINFORMATION);
			if (result == 1)
			{
				wsprintf(tmp, L"%d", totalScore);
				WritePrivateProfileString(L"app", L"highestScore", tmp, iniPath);
				WritePrivateProfileString(L"app", L"playerGotIt", currentPlayer, iniPath);
				MessageBox(0, L"Update successfully!", L"Success", MB_OK | MB_ICONINFORMATION);
			}
		}

		// enable window
		EnableWindow(btnStart, true);
		EnableWindow(btnReset, false);
		EnableWindow(hButtonSubmit1, false);
		EnableWindow(hButtonSubmit2, false);

		break;
	case IDC_BTN_CHOOSE1:
		KillTimer(hwnd, IDT_TIMER1);
		if (trueAnswerPos == 1)
		{
			double percent = (double)currentSecond * 100 / totalTime;
			int score = percent * listScoreBySeconds[totalTime];

			totalScore += score;

			WCHAR tmp[BUFFERSIZE];
			wsprintf(tmp, L"%d", totalScore);
			SetWindowText(txbScore, tmp);
		}

		currentRound++;

		if (currentRound <= totalRounds)
		{
			NextQuestion(hwnd);
		}
		else
		{
			EnableWindow(hButtonSubmit1, false);
			EnableWindow(hButtonSubmit2, false);
		}

		InvalidateRect(hwnd, NULL, TRUE);

		break;
	case IDC_BTN_CHOOSE2:
		KillTimer(hwnd, IDT_TIMER1);
		if (trueAnswerPos == 2)
		{
			double percent = (double)currentSecond * 100 / totalTime;
			int score = percent * listScoreBySeconds[totalTime];

			totalScore += score;

			WCHAR tmp[BUFFERSIZE];
			wsprintf(tmp, L"%d", totalScore);
			SetWindowText(txbScore, tmp);
		}

		currentRound++;

		if (currentRound <= totalRounds)
		{
			NextQuestion(hwnd);
		}
		else
		{
			EnableWindow(hButtonSubmit1, false);
			EnableWindow(hButtonSubmit2, false);
		}

		InvalidateRect(hwnd, NULL, TRUE);

		break;

	case ID_OPTION_CHANGENAME:
		if (isPlaying == false)
		{
			result = DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1),
				hwnd, DlgInputName_WndProc);
		}
		else
		{
			MessageBox(hwnd, L"Game is playing! You can't set up anything", L"Notification", MB_OK | MB_ICONINFORMATION);
		}
		break;
	case ID_OPTION_SETCOUNTDOWN:

		if (isPlaying == false)
		{
			result = DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG2),
				hwnd, DlgSetCountDown_WndProc);
		}
		else
		{
			MessageBox(hwnd, L"Game is playing! You can't set up anything", L"Notification", MB_OK | MB_ICONINFORMATION);
		}
		break;
	case IDM_EXIT:
		delete RectDraw;
		PostQuitMessage(0);
		break;
	}
}

void OnPaint(HWND hwnd)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);

	{
		if (posQuestion != -1)
		{
			Graphics graphics(hdc);

			WCHAR buffer[BUFFERSIZE];
			WCHAR fileName[BUFFERSIZE] = L"picData";
			int result = getCurrentPath(fileName, buffer);
			if (result == 0)
			{
				MessageBox(0, L"Cant open picData folder. Program will be quit!", L"Notification", MB_OK | MB_ICONINFORMATION);
				delete RectDraw;
				PostQuitMessage(0);
			}
			
			wsprintf(buffer, L"%s\\%s", buffer, ListQuestion[posQuestion]._link);

			Image image(buffer);

			Pen pen(Color(0, 0, 0, 0), 1);

			graphics.DrawRectangle(&pen, *RectDraw);

			graphics.DrawImage(&image, *RectDraw);
		}
	}

	EndPaint(hwnd, &ps);
}

void OnDestroy(HWND hwnd)
{
	delete RectDraw;
	PostQuitMessage(0);
}


INT_PTR CALLBACK DlgInputName_WndProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		int id = LOWORD(wParam);
		switch (id) {
		case IDC_OK:

			GetDlgItemText(hDlg, IDC_EDIT1, currentPlayer, sizeof(currentPlayer) / sizeof(WCHAR));

			if (wcslen(currentPlayer) != 0)
			{
				EndDialog(hDlg, TRUE);
				return (INT_PTR)TRUE;
			}
			else
			{
				MessageBox(hDlg, L"Name is not allowed to be empty!", L"Notification", MB_OK | MB_ICONINFORMATION);
			}
			
			break;
		case IDC_CANCEL:
			EndDialog(hDlg, FALSE);
			return (INT_PTR)TRUE;
			break;
		}
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK DlgSetCountDown_WndProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	UNREFERENCED_PARAMETER(lParam);

	HWND hComboBox = GetDlgItem(hDlg, IDC_DGL2_COMBO);

	WCHAR values[5][3] = {
			L"2", L"3", L"5", L"7", L"10"
	};

	for (int i = 0; i <= 10; i++)
	{
		listScoreBySeconds.push_back(0);
	}

	listScoreBySeconds[2] = 48;
	listScoreBySeconds[3] = 37;
	listScoreBySeconds[5] = 25;
	listScoreBySeconds[7] = 17;
	listScoreBySeconds[10] = 9;

	WCHAR tmpValue[3];
	int index;

	switch (message)
	{
	case WM_INITDIALOG:
		// set value
		memset(&tmpValue, 0, sizeof(tmpValue));

		for (int k = 0; k < 5; k++)
		{
			wcscpy_s(tmpValue, sizeof(tmpValue) / sizeof(WCHAR), (WCHAR*)values[k]);

			SendMessage(hComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)tmpValue);
		}

		SendMessage(hComboBox, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

		return (INT_PTR)TRUE;

	case WM_COMMAND:
		int id = LOWORD(wParam);
		switch (id) {
		case ID_DLG2_OK:
			
			index = ComboBox_GetCurSel(hComboBox);
			ComboBox_GetItemData(hComboBox, index);

			ComboBox_GetLBText(hComboBox, index, tmpValue);

			totalTime = (int)wcstol(tmpValue, NULL, 10);

			//MessageBox(0, tmpSetValue, 0, 0);

			EndDialog(hDlg, TRUE);
			return (INT_PTR)TRUE;

			break;
		case WM_NOTIFY:
			switch (((LPNMHDR)lParam)->code)
			{
			case CBN_SELCHANGE:
				MessageBox(0, 0, 0, 0);
				break;
			}
			break;
		case ID_DLG2_CANCEL:
			EndDialog(hDlg, FALSE);
			return (INT_PTR)TRUE;
			break;
		}
	}

	return (INT_PTR)FALSE;
}