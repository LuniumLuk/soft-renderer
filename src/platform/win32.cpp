#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include "platform.hpp"

// reference : http://www.winprog.org/tutorial/simple_window.html

const char g_szClassName[] = "viewer_window_class";

struct Lurdr::APPWINDOW
{
    HWND        *handle;
    byte_t      *surface;
    bool        keys[KEY_NUM];
    bool        buttons[BUTTON_NUM];
    bool        should_close;
    void        (*keyboardCallback)(AppWindow *window, KEY_CODE key, bool pressed);
    void        (*mouseButtonCallback)(AppWindow *window, MOUSE_BUTTON button, bool pressed);
    void        (*mouseScrollCallback)(AppWindow *window, float offset);
    void        (*mouseDragCallback)(AppWindow *window, float x, float y);
};

Lurdr::APPWINDOW * g_window;

HINSTANCE   g_hInstance;
POINTS      g_mouse_pts;
BITMAPINFO  g_bitmapinfo;
int         g_viewer_width;
int         g_viewer_height;
bool        g_update_paint = false;

static void handleKeyPress(WPARAM wParam, bool pressed)
{
    static Lurdr::KEY_CODE key;
    // reference : https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
    switch(wParam)
    {
        case 0x41: key = Lurdr::KEY_A;      break;
        case 0x44: key = Lurdr::KEY_D;      break;
        case 0x53: key = Lurdr::KEY_S;      break;
        case 0x57: key = Lurdr::KEY_W;      break;
        case 0x20: key = Lurdr::KEY_SPACE;  break;
        case 0x1B: key = Lurdr::KEY_ESCAPE; break;
        default:   key = Lurdr::KEY_NUM;    break;
    }

    if (key < Lurdr::KEY_NUM)
    {
        g_window->keys[key] = pressed;
        if (g_window->keyboardCallback)
        {
            g_window->keyboardCallback(g_window, key, pressed);
        }
    }
}

static void handleMouseDrag(float x, float y)
{
    if (g_window->mouseDragCallback)
    {
        g_window->mouseDragCallback(g_window, x, y);
    }
}

static void handleMouseButton(Lurdr::MOUSE_BUTTON button, bool pressed)
{
    g_window->buttons[button] = pressed;
    if (g_window->mouseButtonCallback)
    {
        g_window->mouseButtonCallback(g_window, button, pressed);
    }
}

static void handleMouseScroll(float delta)
{
    if (g_window->mouseScrollCallback)
    {
        g_window->mouseScrollCallback(g_window, delta);
    }
}

// handling windows procedures
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    // reference : https://docs.microsoft.com/en-us/windows/win32/inputdev

    switch(msg)
    {
        // handle keyboard input
        case WM_KEYDOWN:
            handleKeyPress(wParam, true);
            break;
        case WM_KEYUP:
            handleKeyPress(wParam, false);
            break;
        // handle mouse input
        case WM_LBUTTONDOWN:
            handleMouseButton(Lurdr::BUTTON_L, true);
            break;
        case WM_LBUTTONUP:
            handleMouseButton(Lurdr::BUTTON_L, false);
            break;
        case WM_RBUTTONDOWN:
            handleMouseButton(Lurdr::BUTTON_R, true);
            break;
        case WM_RBUTTONUP:
            handleMouseButton(Lurdr::BUTTON_R, false);
            break;
        case WM_MOUSEMOVE:
            if (wParam & MK_LBUTTON) // left mouse drag
            {
                g_mouse_pts = MAKEPOINTS(lParam); 
                handleMouseDrag(g_mouse_pts.x, g_mouse_pts.y);
            }
            break;
        case WM_MOUSEWHEEL:
            handleMouseScroll(GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? 1.0f : -1.0f);
            break;
        case WM_PAINT:
            if (g_update_paint)
            {
                HDC hdc = GetDC(hwnd);

                SetDIBitsToDevice(
                    hdc,
                    0, 0,
                    g_viewer_width,
                    g_viewer_height,
                    0, 0, 0, g_viewer_height,
                    (void*)(g_window->surface),
                    &g_bitmapinfo,
                    DIB_RGB_COLORS
                );

                ReleaseDC(hwnd, hdc);
                g_update_paint = false;
            }
            break;
        case WM_CLOSE:
            g_window->should_close = true;
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

void Lurdr::initializeApplication()
{
    WNDCLASSEX wc;
    g_hInstance = GetModuleHandle(NULL);

    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = g_hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
    }
}

// need no implementation
void Lurdr::runApplication() {};

// need no implementation
void Lurdr::terminateApplication() {};

Lurdr::AppWindow* Lurdr::createWindow(const char *title, int width, int height, byte_t *surface_buffer)
{
    HWND hwnd = CreateWindowEx(
        0,
        g_szClassName,
        title,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        NULL, NULL, g_hInstance, NULL);

    if (hwnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    g_bitmapinfo.bmiHeader.biSize           = sizeof(BITMAPINFOHEADER);
    g_bitmapinfo.bmiHeader.biBitCount       = 24;
    g_bitmapinfo.bmiHeader.biWidth          = width;
    g_bitmapinfo.bmiHeader.biHeight         = -height;
    g_bitmapinfo.bmiHeader.biCompression    = BI_RGB;
    g_bitmapinfo.bmiHeader.biClrUsed        = 0;
    g_bitmapinfo.bmiHeader.biClrImportant   = 0;
    g_bitmapinfo.bmiHeader.biPlanes         = 1;
    g_bitmapinfo.bmiHeader.biSizeImage      = 0;

    g_window = new Lurdr::AppWindow();
    g_window->handle = &hwnd;
    g_window->surface = surface_buffer;

    g_viewer_width = width;
    g_viewer_height = height;

    ShowWindow(hwnd, SW_SHOWNORMAL);
    UpdateWindow(hwnd);

    return g_window;
}

void Lurdr::destroyWindow(AppWindow *window)
{
    window->should_close = true;
}

void Lurdr::swapBuffer(AppWindow *window)
{
    __unused_variable(window);
    g_update_paint = true;
}

bool Lurdr::windowShouldClose(AppWindow *window)
{
    return window->should_close;
}

void Lurdr::pollEvent()
{
    static MSG msg;
    if (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

/**
 * input & callback registrations
 */
void Lurdr::setKeyboardCallback(AppWindow *window, void(*callback)(AppWindow*, KEY_CODE, bool))
{
    window->keyboardCallback = callback;
}

void Lurdr::setMouseButtonCallback(AppWindow *window, void(*callback)(AppWindow*, MOUSE_BUTTON, bool))
{
    window->mouseButtonCallback = callback;
}

void Lurdr::setMouseScrollCallback(AppWindow *window, void(*callback)(AppWindow*, float))
{
    window->mouseScrollCallback = callback;
}

void Lurdr::setMouseDragCallback(AppWindow *window, void(*callback)(AppWindow*, float, float))
{
    window->mouseDragCallback = callback;
}

bool Lurdr::isKeyDown(AppWindow *window, KEY_CODE key)
{
    return window->keys[key];
}

bool Lurdr::isMouseButtonDown(AppWindow *window, MOUSE_BUTTON button)
{
    return window->buttons[button];
}

void Lurdr::getMousePosition(AppWindow *window, float *x, float *y)
{
    __unused_variable(window);

    *x = g_mouse_pts.x;
    *y = g_mouse_pts.y;
}

Lurdr::Time Lurdr::getSystemTime()
{
    SYSTEMTIME st;
    GetSystemTime(&st);

    Lurdr::Time time;
    time.year = st.wYear;
    time.month = st.wMonth;
    time.day_of_week = st.wDayOfWeek;
    time.day = st.wDay;
    time.hour = st.wHour;
    time.minute = st.wMinute;
    time.second = st.wSecond;
    time.millisecond = st.wMilliseconds;

    return time;
}