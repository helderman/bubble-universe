// bu.cpp - Ruud Helderman, May 2026 - MIT License
// Based on "Bubble Universe", written in Processing by yuruyurau
// https://x.com/yuruyurau/status/1226846058728177665

#define WINVER 0x0700
//#define _WIN32_WINNT 0x0700   // https://github.com/microsoft/Windows-classic-samples/issues/317
#define UNICODE
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <d2d1.h>
#include <math.h>
#include <chrono>

template<class Interface> inline void SafeRelease(Interface **ppInterfaceToRelease)
{
    if (*ppInterfaceToRelease != NULL)
    {
        (*ppInterfaceToRelease)->Release();
        (*ppInterfaceToRelease) = NULL;
    }
}

EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)

class BubbleUniverse
{
public:
    BubbleUniverse();
    ~BubbleUniverse();
    HRESULT Initialize();
    void RunMessageLoop();

private:
    HRESULT CreateDeviceIndependentResources();
    HRESULT CreateDeviceResources();
    void DiscardDeviceResources();

    HRESULT OnRender();

    void OnResize(UINT width, UINT height);

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    static long long GetTime(void);

private:
    HWND m_hwnd;
    ID2D1Factory *m_pD2DFactory;
    ID2D1HwndRenderTarget *m_pRenderTarget;
    ID2D1SolidColorBrush *m_pBrush;
    long long m_start;
};

BubbleUniverse::BubbleUniverse() :
    m_hwnd(NULL),
    m_pD2DFactory(NULL),
    m_pRenderTarget(NULL),
    m_pBrush(NULL)
{
}

BubbleUniverse::~BubbleUniverse()
{
    SafeRelease(&m_pD2DFactory);
    SafeRelease(&m_pRenderTarget);
    SafeRelease(&m_pBrush);
}

HRESULT BubbleUniverse::Initialize()
{
    HRESULT hr = CreateDeviceIndependentResources();
    if (SUCCEEDED(hr))
    {
        WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
        wcex.style         = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc   = BubbleUniverse::WndProc;
        wcex.cbClsExtra    = 0;
        wcex.cbWndExtra    = sizeof(LONG_PTR);
        wcex.hInstance     = HINST_THISCOMPONENT;
        wcex.hbrBackground = NULL;
        wcex.lpszMenuName  = NULL;
        wcex.hCursor       = LoadCursor(NULL, IDC_ARROW);
        wcex.lpszClassName = L"D2DBubbleUniverse";

        RegisterClassEx(&wcex);

        FLOAT dpiX, dpiY;
        m_pD2DFactory->GetDesktopDpi(&dpiX, &dpiY);

        m_hwnd = CreateWindow(
            L"D2DBubbleUniverse",
            L"Direct2D Bubble Universe",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            static_cast<UINT>(ceil(640.f * dpiX / 96.f)),
            static_cast<UINT>(ceil(480.f * dpiY / 96.f)),
            NULL,
            NULL,
            HINST_THISCOMPONENT,
            this
        );
        hr = m_hwnd ? S_OK : E_FAIL;
        if (SUCCEEDED(hr))
        {
            ShowWindow(m_hwnd, SW_SHOWNORMAL);
            UpdateWindow(m_hwnd);
        }
    }
    m_start = 0; //GetTime();
    return hr;
}

HRESULT BubbleUniverse::CreateDeviceIndependentResources()
{
    return D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);
}

HRESULT BubbleUniverse::CreateDeviceResources()
{
    HRESULT hr = S_OK;

    if (!m_pRenderTarget)
    {
        RECT rc;
        GetClientRect(m_hwnd, &rc);

        D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

        hr = m_pD2DFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(m_hwnd, size),
            &m_pRenderTarget
        );
        if (SUCCEEDED(hr))
        {
            hr = m_pRenderTarget->CreateSolidColorBrush(
                D2D1::ColorF(D2D1::ColorF::Black),
                &m_pBrush
            );
            // D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES props;
            // props.center.x = 0;
            // props.center.y = 0;
            // props.gradientOriginOffset.x = 0;
            // props.gradientOriginOffset.y = 0;
            // props.radiusX = 3;
            // props.radiusY = 3;
            // hr = m_pRenderTarget->CreateRadialGradientBrush(props, stops, &m_pBrush);
        }
    }
    return hr;
}

void BubbleUniverse::DiscardDeviceResources()
{
    SafeRelease(&m_pRenderTarget);
    SafeRelease(&m_pBrush);
}

void BubbleUniverse::RunMessageLoop()
{
    MSG msg;

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

HRESULT BubbleUniverse::OnRender()
{
    HRESULT hr = CreateDeviceResources();

    if (SUCCEEDED(hr) && !(m_pRenderTarget->CheckWindowState() & D2D1_WINDOW_STATE_OCCLUDED))
    {
        D2D1_SIZE_F renderTargetSize = m_pRenderTarget->GetSize();
        FLOAT width = renderTargetSize.width;
        FLOAT height = renderTargetSize.height;
        FLOAT size = min(width, height) * 0.24f;
        //FLOAT time = (FLOAT)(GetTime() - m_start) * 2e-13f;
        //FLOAT time = (FLOAT)((GetTime() - m_start) % 4);
        FLOAT time = (FLOAT)(m_start++) * 0.002f;

        m_pRenderTarget->BeginDraw();
        m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
        m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

        for (int i = 0; i < 256; i += 2)
        {
            FLOAT x = 0, y = 0;
            for (int j = 0; j < 256; j += 2)
            {
                FLOAT a = x + i;
                FLOAT b = y + i * 0.031f - time;
                x = cosf(a) + cosf(b);
                y = sinf(a) + sinf(b);
                m_pBrush->SetColor(D2D1::ColorF((float)i / 255.0f, (float)j / 255.0f, 0.4f, 1));
                m_pRenderTarget->FillEllipse(
                    D2D1::Ellipse(
                        D2D1::Point2F(width / 2 - size * x, height / 2 - size * y),
                        size * 0.008f,
                        size * 0.008f
                    ),
                    m_pBrush
                );
            }
        }

        hr = m_pRenderTarget->EndDraw();

        if (hr == D2DERR_RECREATE_TARGET)
        {
            hr = S_OK;
            DiscardDeviceResources();
        }
    }
    return hr;
}

void BubbleUniverse::OnResize(UINT width, UINT height)
{
    if (m_pRenderTarget)
    {
        D2D1_SIZE_U size;
        size.width = width;
        size.height = height;
        m_pRenderTarget->Resize(size);
    }
}

LRESULT CALLBACK BubbleUniverse::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;

    if (message == WM_CREATE)
    {
        LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
        BubbleUniverse *pApp = (BubbleUniverse *)pcs->lpCreateParams;

        ::SetWindowLongPtrW(
            hwnd,
            GWLP_USERDATA,
            reinterpret_cast<LONG_PTR>(pApp)
            );

        result = 1;
    }
    else
    {
        BubbleUniverse *pApp = reinterpret_cast<BubbleUniverse *>(
            ::GetWindowLongPtrW(hwnd, GWLP_USERDATA)
        );

        bool wasHandled = false;

        if (pApp)
        {
            switch (message)
            {
            case WM_SIZE:
                {
                    UINT width = LOWORD(lParam);
                    UINT height = HIWORD(lParam);
                    pApp->OnResize(width, height);
                }
                wasHandled = true;
                result = 0;
                break;

            case WM_PAINT:
            case WM_DISPLAYCHANGE:
                {
                    PAINTSTRUCT ps;
                    BeginPaint(hwnd, &ps);
                    pApp->OnRender();
                    EndPaint(hwnd, &ps);
                }
                InvalidateRect(hwnd, NULL, false);
                wasHandled = true;
                result = 0;
                break;

            case WM_DESTROY:
                {
                    PostQuitMessage(0);
                }
                wasHandled = true;
                result = 1;
                break;
            }
        }

        if (!wasHandled)
        {
            result = DefWindowProc(hwnd, message, wParam, lParam);
        }
    }
    return result;
}

long long BubbleUniverse::GetTime(void)     // TODO
{
    return std::chrono::high_resolution_clock::now().time_since_epoch().count();
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

    if (SUCCEEDED(CoInitialize(NULL)))
    {
        {
            BubbleUniverse app;
            if (SUCCEEDED(app.Initialize()))
            {
                app.RunMessageLoop();
            }
        }
        CoUninitialize();
    }
    return 0;
}
