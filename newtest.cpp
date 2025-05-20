// INCLUDES ====================================================================
#include <windows.h>

// FUNCTION PROTOTYPES =========================================================
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


// entry point for a Windows application =======================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int nWinMode)
{
    // define the window class name
    static const TCHAR szAppName[] = TEXT("MinimalWindowsApp");

    // create an instance of the window class structure
    WNDCLASSEX wc;

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = (HICON)LoadImage(NULL, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
    wc.hIconSm = (HICON)LoadImage(NULL, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
    wc.hCursor = (HCURSOR)LoadImage(NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = szAppName;

    if (0 == RegisterClassEx(&wc))
    {
        MessageBox(NULL, TEXT("Can't Register the Window Class!"), szAppName, MB_OK | MB_ICONERROR);
        return E_FAIL;
    }

    // define the application title
    static const TCHAR szAppTitle[] = TEXT("Win32 API Skeletal Application");

    // create the window
    HWND hwnd = CreateWindow(szAppName, szAppTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL, hInstance, NULL);

    // check if the window was created, exit if fail
    if (NULL == hwnd)
    {
        MessageBox(NULL, TEXT("Unable to Create the Main Window!"), szAppName, MB_OK | MB_ICONERROR);
        return E_FAIL;
    }

    // show and update the window
    ShowWindow(hwnd, SW_SHOWDEFAULT);
    UpdateWindow(hwnd);

    static BOOL bRet;
    static MSG  msg;

    // enter the main message loop
    while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0) // 0 = WM_QUIT
    {
        // check for error
        if (-1 == bRet)
        {
            // handle the error and possibly exit

            // for this app simply report error and exit
            MessageBox(NULL, TEXT("Unable to Continue!"), szAppName, MB_OK | MB_ICONERROR);
            return E_FAIL;
        }
        else
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    // the app is done, exit normally, return control to Windows
    return (int)msg.wParam;
}


// processes the messages that Windows sends to the application ================
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // choose which Windows messages you want to use
    switch (message)
    {
    case WM_PAINT:
        HDC         hdc;
        PAINTSTRUCT ps;
        hdc = BeginPaint(hwnd, &ps);

        // draw some text centered in the client area
        RECT rect;
        GetClientRect(hwnd, &rect);
        DrawText(hdc, TEXT("Hello, Windows!"), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

        EndPaint(hwnd, &ps);
        return S_OK;

    case WM_DESTROY:
        // exit the application
        PostQuitMessage(0);
        return S_OK;
    }

    // let Windows process any unhandled messages
    return DefWindowProc(hwnd, message, wParam, lParam);
}