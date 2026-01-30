#ifndef UNICODE
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE
#endif

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>

#define MAX_LOADSTRING 100
#define IDM_ABOUT 104
#define IDM_EXIT 105
#define IDD_ABOUTBOX 103

static TCHAR szWindowClass[] = _T("AlinDOSWindowClass");
static TCHAR szTitle[] = _T("AlinD OS");
HINSTANCE hInst;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK AboutDialogProc(HWND, UINT, WPARAM, LPARAM);
void CreateAboutDialog(HWND hWnd);
void AddMenus(HWND hWnd);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
                   LPSTR lpCmdLine, int nCmdShow)
{
    hInst = hInstance;
    
    WNDCLASSEX wcex;
    wcex.cbSize         = sizeof(WNDCLASSEX);
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL,
            _T("Pencere sınıfı kaydedilemedi!"),
            _T("AlinD OS - Hata"),
            MB_ICONERROR);
        return 1;
    }

    HWND hWnd = CreateWindowEx(
        WS_EX_OVERLAPPEDWINDOW,
        szWindowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (!hWnd)
    {
        MessageBox(NULL,
            _T("Pencere oluşturulamadı!"),
            _T("AlinD OS - Hata"),
            MB_ICONERROR);
        return 1;
    }

    AddMenus(hWnd);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

void AddMenus(HWND hWnd)
{
    HMENU hMenuBar = CreateMenu();
    HMENU hFileMenu = CreateMenu();
    HMENU hHelpMenu = CreateMenu();

    AppendMenu(hFileMenu, MF_STRING, IDM_EXIT, _T("Çıkış\tAlt+F4"));
    AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hFileMenu, _T("Dosya"));

    AppendMenu(hHelpMenu, MF_STRING, IDM_ABOUT, _T("Hakkında..."));
    AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hHelpMenu, _T("Yardım"));

    SetMenu(hWnd, hMenuBar);
}

void CreateAboutDialog(HWND hWnd)
{
    MessageBox(hWnd,
        _T("AlinD OS\n\n")
        _T("Sürüm: 1.0\n\n")
        _T("Geliştirici: AlinD Team\n\n")
        _T("© 2026 AlinD. Tüm hakları saklıdır."),
        _T("AlinD OS Hakkında"),
        MB_OK | MB_ICONINFORMATION);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            switch (wmId)
            {
            case IDM_ABOUT:
                CreateAboutDialog(hWnd);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            
            RECT rect;
            GetClientRect(hWnd, &rect);
            
            HBRUSH hBrush = CreateSolidBrush(RGB(20, 30, 48));
            FillRect(hdc, &rect, hBrush);
            DeleteObject(hBrush);
            
            SetBkMode(hdc, TRANSPARENT);
            SetTextColor(hdc, RGB(255, 255, 255));
            
            HFONT hFont = CreateFont(
                48, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
                CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
                VARIABLE_PITCH, _T("Segoe UI")
            );
            
            SelectObject(hdc, hFont);
            DrawText(hdc, _T("AlinD OS'a Hoş Geldiniz!"), -1, &rect,
                     DT_SINGLELINE | DT_CENTER | DT_VCENTER);
            
            DeleteObject(hFont);
            EndPaint(hWnd, &ps);
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
