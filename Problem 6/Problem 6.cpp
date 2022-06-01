// PROBLEM6.cpp : Defines the entry point for the application.
//
                                                                               
#include "framework.h"
#include "Problem 6.h"
#include "CPoint.h"
#include "CLine.h"
#include "CPolygon.h"
#include "CEllipse.h"
#include "CRectangle.h"
#include "CCircle.h"
#include "CTringale.h"
#include "CSquare.h"

#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

#define MAX_LOADSTRING 100

#define PAINT_CIRCLE 1001
#define PAINT_LINE 1002
#define PAINT_POLYGON 1003
#define PAINT_RECTANGLE 1004
#define PAINT_SQUARE 1005
#define PAINT_TRIANGLE 1006
#define PAINT_SEMICIRLCE 1007
#define PAINT_ELLIPSE 1008

#define SHOW_DIALOG 2411 
#define DELETE_SHAPE 2412

#define RADIAN 0.01

#define BLACK RGB(0, 0, 0)
#define RED RGB(255, 0, 0)
#define YELLOW RGB(255, 255, 0)
#define ORANGE RGB(255, 128, 0)
#define LIGHT_BLUE RGB(204, 255, 255)

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

HWND hMain;
HWND aValue, bValue;

// list store shapes intersect with lines
std::vector<std::string> listIntersect;         

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    DialogProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// Add controls to main windows

void addButtons(HWND); // Add Button for user to use
void registerDialogClass(HINSTANCE); // Register Dialog class
void displayDialog(HWND); // Display Dialog window

bool isDrawing(bool); // Return true if you are drawing something, else return false

// Process Events Sent to the Window

// When LButton Up
void onLButtonUp(HWND, CShape* [], int&, int, int, int, int, bool&, RECT); 
// When the mouse move
void onMouseMove(HWND, CShape* [], int&, int, int, int, int); 
// When the keys were pressed
void onKeyDown(HWND, WPARAM, CShape* [], int&, int&, bool&, RECT); 
// When the line was painted
void onPaintLine(HWND, HWND, HWND, RECT, CLine&, CShape* [], int&, bool&); 
// Redefine the line when the window resize
void findTheLine(CPoint, CLine&, RECT, double, double); 

// Paint the UI 
void drawBoderAndMenu(HDC, RECT&, CShape* [], int, int&);
void drawLeftMenu1(HDC, RECT&, CShape* [], int, int&);
void drawLeftMenu2(HDC, RECT&, CShape* [], int, int&);
// Draw Descartes Coordinate System
void drawDescartes(HDC, RECT, HPEN); 

// Something need function
RECT rePaintMenu(RECT);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PROBLEM6, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Register Dialog Class
    registerDialogClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow)) {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PROBLEM6));

    MSG msg;

    // Main message loo:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
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

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROBLEM6));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_PROBLEM6);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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

    hMain = CreateWindowW(szWindowClass, L"Problem 6", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hMain)
    {
        return FALSE;
    }

    ShowWindow(hMain, nCmdShow);
    UpdateWindow(hMain);

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

const int MAX_SHAPE = 300;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // Container of our shape
    static int indexObject = 0; // Index of current shape 
    static int numberObject = 0; // Numbers of shape 
    static CLine line(CPoint(0, 0), CPoint(0, 0)); // Your primary line
    static CShape* object[MAX_SHAPE]; // Pointer of our shape

    // Mouse variable
    static int px, py;
    static int cx, cy;

    // Drawing status
    static bool drawSomething = false;

    // Drawing tools
    static HPEN hPenDot0;
    static HPEN hPenDot1;
    static HPEN hPenDot2;
    static HPEN hPenDot3;
    static HPEN hPenDot4;
    static HBRUSH mainWindowBackground;

    // Drawing area variables
    PAINTSTRUCT ps;
    HDC hdc;
    RECT rect;
    RECT rect2;
    GetClientRect(hWnd, &rect);
    GetClientRect(hWnd, &rect2);
    rect2.left = 0; rect2.top = 100;

    // Your main code begin here
    switch (message)
    {
    case WM_CREATE:
    {
        hPenDot0 = CreatePen(PS_SOLID, 2, BLACK);
        hPenDot1 = CreatePen(PS_SOLID, 2, YELLOW);
        hPenDot2 = CreatePen(PS_SOLID, 2, ORANGE);
        hPenDot3 = CreatePen(PS_SOLID, 2, RED);
        hPenDot4 = CreatePen(PS_SOLID, 1, BLACK);
        mainWindowBackground = CreateSolidBrush(LIGHT_BLUE);
        addButtons(hWnd);
        break;
    }
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case IDM_ABOUT:
        {
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        }
        case IDM_EXIT:
        {
            DestroyWindow(hWnd);
            break;
        }
        case SHOW_DIALOG:
        {
            displayDialog(hWnd);
            break;
        }
        case PAINT_LINE:
        {
            onPaintLine(hWnd, aValue, bValue, rect2, line, object, 
                numberObject, drawSomething);
            InvalidateRect(hWnd, NULL, TRUE);
            SetFocus(hWnd);
            break;
        }
        case PAINT_CIRCLE:
        {
            if (isDrawing(drawSomething))
            {
                delete object[numberObject - 1];
                --numberObject;
            }
            drawSomething = true;
            object[numberObject] = new CCircle;
            ++numberObject;
            InvalidateRect(hWnd, NULL, TRUE);
            SetFocus(hWnd);
            break;
        }
        case PAINT_POLYGON:
        {
            if (isDrawing(drawSomething))
            {
                delete object[numberObject - 1];
                --numberObject;
            }
            drawSomething = true;
            object[numberObject] = new CPolygon;
            ++numberObject;
            InvalidateRect(hWnd, NULL, TRUE);
            SetFocus(hWnd);
            break;
        }
        case PAINT_SQUARE:
        {
            if (isDrawing(drawSomething))
            {
                delete object[numberObject - 1];
                --numberObject;
            }
            drawSomething = true;
            object[numberObject] = new CSquare;
            ++numberObject;
            InvalidateRect(hWnd, NULL, TRUE);
            SetFocus(hWnd);
            break;
        }
        case PAINT_RECTANGLE:
        {
            if (isDrawing(drawSomething))
            {
                delete object[numberObject - 1];
                --numberObject;
            }
            drawSomething = true;
            object[numberObject] = new CRectangle;
            ++numberObject;
            InvalidateRect(hWnd, NULL, TRUE);
            SetFocus(hWnd);
            break;
        }
        case PAINT_SEMICIRLCE:
        {
            if (isDrawing(drawSomething))
            {
                delete object[numberObject - 1];
                --numberObject;
            }
            drawSomething = true;
            object[numberObject] = new SemiCircle;
            ++numberObject;
            InvalidateRect(hWnd, NULL, TRUE);
            SetFocus(hWnd);
            break;
        }
        case PAINT_TRIANGLE:
        {
            if (isDrawing(drawSomething))
            {
                delete object[numberObject - 1];
                --numberObject;
            }
            drawSomething = true;
            object[numberObject] = new CTringale;
            ++numberObject;
            InvalidateRect(hWnd, NULL, TRUE);
            SetFocus(hWnd);
            break;
        }
        case PAINT_ELLIPSE:
        {
            if (isDrawing(drawSomething))
            {
                delete object[numberObject - 1];
                --numberObject;
            }
            drawSomething = true;
            object[numberObject] = new CEllipse;
            ++numberObject;
            InvalidateRect(hWnd, NULL, TRUE);
            SetFocus(hWnd);
            break;
        }
        case DELETE_SHAPE:
        {
            if (isDrawing(drawSomething))
            {
                delete object[numberObject - 1];
                --numberObject;
                drawSomething = false;
            }
            if (MessageBox(hWnd, L"Do you want to delete all shapes?", 
                L"Delete Warning!", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
            {
                for (int i = 0; i < numberObject; ++i)
                {
                    delete object[i];
                }
                numberObject = 0;
                InvalidateRect(hWnd, NULL, TRUE);
            }
            break;
        }
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;
    }
    case WM_CLOSE:
    {
        if (MessageBox(hWnd, L"Do you really want to exit?", 
            L"Problem 6", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
        {
            DeleteObject(hPenDot0);
            DeleteObject(hPenDot1);
            DeleteObject(hPenDot2);
            DeleteObject(hPenDot3);
            DeleteObject(hPenDot4);
            DeleteObject(mainWindowBackground);
            DestroyWindow(hWnd);
            break;
        }
        break;
    }
    case WM_KEYDOWN:
    {
        onKeyDown(hWnd, wParam, object, numberObject, indexObject, 
            drawSomething, rect);
        break;
    }
    case WM_LBUTTONDOWN:
    {
        if (isDrawing(drawSomething))
        {
            px = cx = GET_X_LPARAM(lParam);
            py = cy = GET_Y_LPARAM(lParam);
            SetCapture(hWnd);
            SetFocus(hWnd);
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;
    }
    case WM_LBUTTONUP:
    {
        if (GetCapture() == hWnd && isDrawing(drawSomething))
        {
            onLButtonUp(hWnd, object, numberObject, px, py, cx, cy,
                drawSomething, rect);
        }
        ReleaseCapture();
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    }
    case WM_MOUSEMOVE:
    {
        if (GetCapture() == hWnd && isDrawing(drawSomething))
        {
            cx = GET_X_LPARAM(lParam);
            cy = GET_Y_LPARAM(lParam);
            onMouseMove(hWnd, object, numberObject, px, py, cx, cy);
        }
        break;
    }
    case WM_PAINT:
    {

        hdc = BeginPaint(hWnd, &ps);

        // Repaint the workingspace
        FillRect(hdc, &rect2, mainWindowBackground);

        // Draw the line border
        static int height = 0;
        drawBoderAndMenu(hdc, rect, object, numberObject, height);

        // Show what are you doing
        if (isDrawing(drawSomething))
        {
            drawLeftMenu1(hdc, rect, object, numberObject, height);
        }
        else if (numberObject > 0)
        {
            drawLeftMenu2(hdc, rect, object, indexObject, height);
        }

        listIntersect.clear(); // Always remember to clear the vector

        // Drawing our object
        SelectObject(hdc, GetStockObject(HOLLOW_BRUSH));
        for (int i = 0; i < numberObject; ++i)
        {
            if (line.isVaild() && object[i]->intersect(line))
            {
                std::string objectName = object[i]->getName();
                listIntersect.push_back(objectName);
                SetDCBrushColor(hdc, RED);
                SelectObject(hdc, GetStockObject(DC_BRUSH));
                SelectObject(hdc, hPenDot3);
            }
            else
            {
                SetDCBrushColor(hdc, YELLOW);
                SelectObject(hdc, GetStockObject(DC_BRUSH));
                SelectObject(hdc, hPenDot1);
            }
            if (i == indexObject)
            {
                SetDCBrushColor(hdc, ORANGE);
                SelectObject(hdc, GetStockObject(DC_BRUSH));
                SelectObject(hdc, hPenDot2);
            }
            object[i]->draw(hdc);
        }
        if (line.isVaild()) // If the line is created then draw it
        {
            SelectObject(hdc, hPenDot4);
            line.draw(hdc);
        }
        drawDescartes(hdc, rect2, hPenDot0);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        break;
    }
    case WM_SIZE:
    {
        if (line.isVaild())
        {
            CPoint ourOrigin(960, 590);
            findTheLine(ourOrigin, line, rect2, line.getA(), line.getB());
        }
        break;
    }
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
    case WM_INITDIALOG: {
        return (INT_PTR)TRUE;
    }
    case WM_COMMAND:
    {
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    }
    return (INT_PTR)FALSE;
}

void addButtons(HWND hWnd)
{
    // Create buttons for drawing shape 
    CreateWindowW(L"Button", L"Polygon", WS_TABSTOP | WS_VISIBLE | WS_CHILD, 
        160, 0, 90, 30, hWnd, (HMENU)PAINT_POLYGON, NULL, NULL);
    CreateWindowW(L"Button", L"Circle", WS_TABSTOP | WS_VISIBLE | WS_CHILD,
        250, 0, 90, 30, hWnd, (HMENU)PAINT_CIRCLE, NULL, NULL);
    CreateWindowW(L"Button", L"Rectangle", WS_TABSTOP | WS_VISIBLE | WS_CHILD,
        340, 0, 90, 30, hWnd, (HMENU)PAINT_RECTANGLE, NULL, NULL);
    CreateWindowW(L"Button", L"Square", WS_TABSTOP | WS_VISIBLE | WS_CHILD,
        430, 0, 90, 30, hWnd, (HMENU)PAINT_SQUARE, NULL, NULL);
    CreateWindowW(L"Button", L"Semi-circle", WS_TABSTOP | WS_VISIBLE | WS_CHILD,
        160, 30, 90, 30, hWnd, (HMENU)PAINT_SEMICIRLCE, NULL, NULL);
    CreateWindowW(L"Button", L"Ellipse", WS_TABSTOP | WS_VISIBLE | WS_CHILD, 
        250, 30, 90, 30, hWnd, (HMENU)PAINT_ELLIPSE, NULL, NULL);
    CreateWindowW(L"Button", L"Triangle", WS_TABSTOP | WS_VISIBLE | WS_CHILD, 
        340, 30, 90, 30, hWnd, (HMENU)PAINT_TRIANGLE, NULL, NULL);

    // Create buttons for enter value for the line 
    CreateWindowW(L"Static", L"ax + by = 0 line", WS_VISIBLE | WS_CHILD | 
        SS_CENTER | WS_BORDER, 0, 0, 90, 40, hWnd, NULL, NULL, NULL);
    CreateWindowW(L"Static", L"a = ", WS_VISIBLE | WS_CHILD | WS_BORDER | 
        SS_CENTER, 0, 40, 30, 20, hWnd, NULL, NULL, NULL);
    CreateWindowW(L"Static", L"b = ", WS_VISIBLE | WS_CHILD | WS_BORDER | 
        SS_CENTER, 0, 60, 30, 20, hWnd, NULL, NULL, NULL);
    aValue = CreateWindowW(L"Edit", L"0", WS_VISIBLE | WS_CHILD | WS_BORDER |
        WS_TABSTOP | WS_GROUP | ES_AUTOHSCROLL, 
        30, 40, 60, 20, hWnd, NULL, NULL, NULL);
    bValue = CreateWindowW(L"Edit", L"0", WS_VISIBLE | WS_CHILD | WS_BORDER | 
        WS_TABSTOP | WS_GROUP | ES_AUTOHSCROLL,
        30, 60, 60, 20, hWnd, NULL, NULL, NULL);
    CreateWindowW(L"Button", L"Draw", WS_VISIBLE | WS_CHILD,
        90, 40, 70, 40, hWnd, (HMENU)PAINT_LINE, NULL, NULL);

    // Create a button to show which shape the line intersect with
    CreateWindowW(L"Button", L"Info", WS_VISIBLE | WS_CHILD,
        90, 0, 70, 40, hWnd, (HMENU)SHOW_DIALOG, NULL, NULL);

    // Create a button to delete all shape
    CreateWindow(L"Button", L"Delete", WS_VISIBLE | WS_CHILD,
        550, 0, 90, 30, hWnd, (HMENU)DELETE_SHAPE, NULL, NULL);
}

LRESULT CALLBACK DialogProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CLOSE:
    {
        EnableWindow(hMain, true);
        DestroyWindow(hWnd);
        break;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        RECT rect;
        GetClientRect(hWnd, &rect);
        int height = DrawTextW(hdc, (LPCWSTR)L"The line intersect with:", -1, 
            &rect, DT_SINGLELINE | DT_CENTER);
        OffsetRect(&rect, 0, height);
        for (int i = 0; i < (int)listIntersect.size(); ++i)
        {
            std::string s = std::to_string(i + 1) + ".    " + listIntersect[i];
            std::wstring stemp = std::wstring(s.begin(), s.end());
            LPCWSTR sw = stemp.c_str();
            height = DrawTextW(hdc, sw, -1, &rect, DT_SINGLELINE | DT_LEFT);
            OffsetRect(&rect, 0, height);
        }
        break;
    }
    default:
        return DefWindowProcW(hWnd, message, wParam, lParam);
    }
    return 0;
}

void registerDialogClass(HINSTANCE hInst)
{
    WNDCLASSW dialog = { 0 };
    dialog.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    dialog.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_PROBLEM6));
    dialog.hCursor = LoadCursor(NULL, IDC_ARROW);
    dialog.hInstance = hInst;
    dialog.lpszClassName = L"dialogClass";
    dialog.lpfnWndProc = DialogProc;
    RegisterClassW(&dialog);
}

void displayDialog(HWND hWnd)
{
    HWND hDialog = CreateWindowW(L"dialogClass", L"List", WS_VISIBLE | 
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 300, 400, 
        hWnd, NULL, NULL, NULL);
    EnableWindow(hWnd, false);
}

bool isDrawing(bool drawSomething)
{
    return drawSomething;
}

void onLButtonUp(HWND hWnd, CShape* object[], int& numberObject, 
    int px, int py, int cx, int cy, bool& drawSomething, RECT rect)
{
    std::string nameObject = object[numberObject - 1]->getName();
    if (nameObject == "Polygon" || nameObject == "Triangle")
    {
        if (py < MIN_Y)
        {
            MessageBeep(MB_ICONWARNING);
        }
        else
        {
            bool flags = object[numberObject - 1]->add(CPoint(px, py));
            if (!flags)
            {
                if (nameObject == "Polygon")
                {
                    MessageBeep(MB_ICONWARNING);
                    MessageBox(hWnd, L"You can only draw a convex polygon", 
                        L"Warning!", MB_OK);
                }
                else if (nameObject == "Triangle")
                {
                    if (object[numberObject - 1]->getSide() != 3)
                    {
                        MessageBeep(MB_ICONWARNING);
                        MessageBox(hWnd, L"Triangle's vertex cannot be collinear!", 
                            L"Warning!", MB_OK);
                    }
                    else
                    {
                        drawSomething = false;
                        RECT tempRect = rePaintMenu(rect);
                        InvalidateRect(hWnd, &tempRect, TRUE);
                    }
                }
            }
            else
            {
                InvalidateRect(hWnd, &object[numberObject - 1]->getRect(), TRUE);
            }
        }
    }
    if (nameObject == "Circle" || nameObject == "Rectangle" ||
        nameObject == "Square" || nameObject == "Ellipse" || 
        nameObject == "SemiCircle")
    {
        if (px == cx && py == cy)
        {
            delete object[numberObject - 1];
            --numberObject;
        }
        drawSomething = false;
    }
}

void onMouseMove(HWND hWnd, CShape* object[], int& numberObject,
    int px, int py, int cx, int cy)
{
    std::string nameObject = object[numberObject - 1]->getName();
    if (nameObject == "Ellipse" || nameObject == "Square" || 
        nameObject == "SemiCircle" || nameObject == "Rectangle" ||
        nameObject == "Circle")
    {
        if (py < MIN_Y)
        {
            MessageBeep(MB_ICONWARNING);
        }
        else
        {
            object[numberObject - 1]->set(0, CPoint(px, py));
        }
    }
    if (nameObject == "Ellipse" || nameObject == "Rectangle")
    {
        InvalidateRect(hWnd, &object[numberObject - 1]->getRect(), TRUE);
        cy = max(cy, MIN_Y);
        object[numberObject - 1]->set(1, CPoint(cx, cy));
        return;
    }
    if (nameObject == "Circle" || nameObject == "Square" || 
        nameObject == "SemiCircle")
    {
        InvalidateRect(hWnd, &object[numberObject - 1]->getRect(), TRUE);
        double delta_x = (double)cx - px;
        double delta_y = (double)cy - py;
        double min_dis = min(delta_x, delta_y);
        double new_cy = max(cy + min_dis, MIN_Y);
        double new_cx = px + (new_cy - py);
        object[numberObject - 1]->set(1, CPoint(new_cx, new_cy));
        return;
    }
}

void onKeyDown(HWND hWnd, WPARAM wParam, CShape* object[], int& numberObject, 
    int& indexObject, bool& drawSomething, RECT rect)
{
    switch (wParam)
    {
    case VK_TAB:
    {
        if (isDrawing(drawSomething))
        {
            break;
        }
        if (numberObject > 0)
        {
            InvalidateRect(hWnd, &object[indexObject]->getRect(), TRUE);
            indexObject = (indexObject + 1) % numberObject;
            InvalidateRect(hWnd, &object[indexObject]->getRect(), TRUE);
        }
        RECT tempRect = rePaintMenu(rect);
        InvalidateRect(hWnd, &tempRect, TRUE);
        break;
    }
    case VK_RETURN: // If you are drawing a polygon, use ENTER key to finish
    {
        if (numberObject > 0 && !object[numberObject - 1]->isVaild())
        {
            MessageBeep(MB_ICONWARNING);
            MessageBox(hWnd, L"Your polygon is not vaild!", L"Warning!", MB_OK);
            break;
        }
        drawSomething = false;
        RECT tempRect = rePaintMenu(rect);
        InvalidateRect(hWnd, &tempRect, TRUE);
        break;
    }
    case VK_ESCAPE: // If you are drawing something, you can use ESC to cancel
    {
        if (isDrawing(drawSomething))
        {
            delete object[numberObject - 1];
            --numberObject;
        }
        drawSomething = false;
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    }
    // Moving 
    case VK_UP:
    {
        if (numberObject > 0 && !isDrawing(drawSomething))
        {
            InvalidateRect(hWnd, &object[indexObject]->getRect(), TRUE);
            object[indexObject]->move(0, -4);
        }
        break;
    }
    case VK_DOWN:
    {
        if (numberObject > 0 && !isDrawing(drawSomething))
        {
            InvalidateRect(hWnd, &object[indexObject]->getRect(), TRUE);
            object[indexObject]->move(0, 4);
        }
        break;
    }
    case VK_LEFT:
    {
        if (numberObject > 0 && !isDrawing(drawSomething))
        {
            InvalidateRect(hWnd, &object[indexObject]->getRect(), TRUE);
            object[indexObject]->move(-4, 0);
        }
        break;
    }
    case VK_RIGHT:
    {
        if (numberObject > 0 && !isDrawing(drawSomething))
        {
            InvalidateRect(hWnd, &object[indexObject]->getRect(), TRUE);
            object[indexObject]->move(4, 0);
        }
        break;
    }
    default:
        break;
    }
}

void drawDescartes(HDC hdc, RECT rect, HPEN hPenDot0)
{
    double x1 = 0;
    double y1 = 100;
    double x2 = 1920;
    double y2 = 1080;
    CLine horizontalLine(CPoint(x2 / 2, y1), CPoint(x2 / 2, y2));
    CLine verticalLine(CPoint(x1, (y1 + y2) / 2), CPoint(x2, (y1 + y2) / 2));
    SelectObject(hdc, hPenDot0);
    horizontalLine.draw(hdc);
    verticalLine.draw(hdc);
}

RECT rePaintMenu(RECT rect)
{
    RECT tempRect;
    tempRect.left = rect.right - 400; tempRect.top = 0;
    tempRect.right = rect.right; tempRect.bottom = 100;
    return tempRect;
}

void findTheLine(CPoint ourOrigin, CLine& line, RECT rect, double a, double b)
{
    line.set(0, CPoint(ourOrigin.getX() + b, ourOrigin.getY() - a));
    line.set(1, CPoint(ourOrigin.getX() - b, ourOrigin.getY() + a));
    double x1 = (line.getC() - line.getB() * 100) / line.getA();
    double x2 = (line.getC() - line.getB() * rect.bottom) / line.getA();
    line.set(0, CPoint(x1, 100));
    line.set(1, CPoint(x2, rect.bottom));
}

bool checkNumber(wchar_t text[])
{
    int size = wcslen(text);
    bool flags = (text[0] >= '0' && text[0] <= '9');
    if (!flags)
    {
        if (text[0] != '-' && text[0] != '+')
        {
            return false;
        }
    }
    for (int i = 1; i < size; ++i)
    {
        flags = (text[i] >= '0' && text[i] <= '9');
        if (!flags)
        {
            return false;
        }
    }
    return true;
}

void onPaintLine(HWND hWnd, HWND aValue, HWND bValue, RECT rect, CLine& line, 
    CShape* object[], int& numberObject, bool& drawSomething)
{
    if (isDrawing(drawSomething))
    {
        delete object[numberObject - 1];
        --numberObject;
        drawSomething = false;
    }
    bool flags = true;
    wchar_t text[100];
    GetWindowTextW(aValue, text, 100);
    flags &= checkNumber(text);
    double a = _wtoi(text);
    GetWindowTextW(bValue, text, 100);
    flags &= checkNumber(text);
    double b = _wtoi(text);
    if (!flags || a == 0 || b == 0)
    {
        MessageBeep(MB_ICONWARNING);
        MessageBox(hWnd, L"The values must be a number, not equal zero!", 
                L"Error!", MB_OK);
        return;
    }
    CPoint ourOrigin(960, 590);
    findTheLine(ourOrigin, line, rect, a, b);
}

void drawBoderAndMenu(HDC hdc, RECT& rect, CShape* object[], 
    int numberObject, int& height)
{
    CLine(CPoint(0, 100), CPoint(rect.right, 100)).draw(hdc);
    std::string text = "Number of shapes: " + std::to_string(numberObject);
    std::wstring stemp = std::wstring(text.begin(), text.end());
    LPCWSTR sw = stemp.c_str();
    height = DrawTextW(hdc, sw, -1, &rect, DT_SINGLELINE | DT_RIGHT);
    OffsetRect(&rect, 0, height);
}

void drawLeftMenu1(HDC hdc, RECT& rect, CShape* object[],
    int numberObject, int& height)
{
    std::string nameObject = object[numberObject - 1]->getName();
    std::string text1 = "Drawing " + nameObject;
    std::string text2 = "Press ESC key to cancel drawing!";
    std::wstring stemp = std::wstring(text1.begin(), text1.end());
    LPCWSTR sw = stemp.c_str();
    height = DrawTextW(hdc, sw, -1, &rect, DT_SINGLELINE | DT_RIGHT);
    OffsetRect(&rect, 0, height);
    stemp = std::wstring(text2.begin(), text2.end());
    sw = stemp.c_str();
    height = DrawTextW(hdc, sw, -1, &rect, DT_SINGLELINE | DT_RIGHT);
    OffsetRect(&rect, 0, height);
    if (nameObject == "Polygon")
    {
        std::string text3 = "Press Enter to finish!";
        stemp = std::wstring(text3.begin(), text3.end());
        sw = stemp.c_str();
        DrawTextW(hdc, sw, -1, &rect, DT_SINGLELINE | DT_RIGHT);
    }
}

void drawLeftMenu2(HDC hdc, RECT& rect, CShape* object[], 
    int indexObject, int& height)
{
    std::string text1 = "Selecting " + object[indexObject]->getName();
    std::string text2 = "Use TAB to change shape or arrow key to move!";
    std::wstring stemp = std::wstring(text1.begin(), text1.end());
    LPCWSTR sw = stemp.c_str();
    height = DrawTextW(hdc, sw, -1, &rect, DT_SINGLELINE | DT_RIGHT);
    OffsetRect(&rect, 0, height);
    stemp = std::wstring(text2.begin(), text2.end());
    sw = stemp.c_str();
    DrawTextW(hdc, sw, -1, &rect, DT_SINGLELINE | DT_RIGHT);
}
