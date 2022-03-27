#include "Window.h"
#include <sstream>
#include "resource.h"

Window::WindowClass Window::WindowClass::m_wndClass;

Window::WindowClass::WindowClass() noexcept
	:m_hInst(GetModuleHandle(nullptr))
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hIcon = static_cast<HICON>(LoadImage(m_hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 48, 48, 0));
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	wc.hIconSm = static_cast<HICON>(LoadImage(m_hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0));;
	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass(m_wndClassName, GetInstance());
}

const char* Window::WindowClass::GetName() noexcept
{
	return m_wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return m_wndClass.m_hInst;
}

Window::Window(int width, int height, const char* name)
	:m_width(width),
	m_height(height)
{
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

	//throw CHWND_EXCEPT(ERROR_ARENA_TRASHED);
	//throw std::runtime_error("butts butts buuuuuuuuuuuuuuuuttsssss");
	//throw 6969699999999;

	// create window 
	m_hWnd = CreateWindow(WindowClass::GetName(), name, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, width, height, nullptr, nullptr, WindowClass::GetInstance(), this);
	
	// check for error
	if (m_hWnd == nullptr)
	{
		throw CHWND_LAST_EXCEPT();
	}
	
	// show window
	ShowWindow(m_hWnd, SW_SHOWDEFAULT);
	// create graphics object
	m_pGfx = std::make_unique<Graphics>(m_hWnd);
}

void Window::SetTitle(const std::string& title)
{
	if (SetWindowText(m_hWnd, title.c_str()) == 0)
	{
		throw CHWND_LAST_EXCEPT();
	}
}

std::optional<int> Window::ProcessMessage()
{
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return (int)msg.wParam;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return {};
}

Graphics& Window::Gfx()
{
	return *m_pGfx;
}

Window::~Window()
{
	DestroyWindow(m_hWnd);
}

LRESULT CALLBACK Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	if (msg == WM_NCCREATE)
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	case WM_KILLFOCUS:
		m_kbd.ClearState();
		break;

		/********************  KEYBOARD MESSAGES  ***************************/
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		if (!(lParam & 0x40000000) || m_kbd.AutorepeatIsEnabled())
		{
			m_kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
		}
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		m_kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
		break;
	case WM_CHAR:
		m_kbd.OnChar(static_cast<unsigned char>(wParam));
		break;
		/********************* MOUSE MESSAGES *******************************/
	case WM_MOUSEMOVE:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		if (pt.x >= 0 && pt.x < m_width && pt.y >= 0 && pt.y < m_height)
		{
			m_mouse.OnMouseMove(pt.x, pt.y);
			if (!m_mouse.IsInwindow())
			{
				SetCapture(m_hWnd);
				m_mouse.OnMouseEnter();
			}
		}
		else
		{
			if (wParam & (MK_LBUTTON | MK_RBUTTON))
			{
				m_mouse.OnMouseMove(pt.x, pt.y);
			}
			else
			{
				ReleaseCapture();
				m_mouse.OnMouseLeave();
			}
		}
		break;
	}
	case WM_LBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		m_mouse.OnLeftPressed(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		m_mouse.OnRightPressed(pt.x, pt.y);
		break;
	}
	case WM_LBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		m_mouse.OnLeftReleased(pt.x, pt.y);
		if (pt.x < 0 || pt.x >= m_width || pt.y < 0 || pt.y >= m_height)
		{
			ReleaseCapture();
			m_mouse.OnMouseLeave();
		}
		break;
	}
	case WM_RBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		m_mouse.OnRightReleased(pt.x, pt.y);
		if (pt.x < 0 || pt.x >= m_width || pt.y < 0 || pt.y >= m_height)
		{
			ReleaseCapture();
			m_mouse.OnMouseLeave();
		}
		break;
	}
	case WM_MOUSEWHEEL:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		m_mouse.OnWheelDelta(pt.x, pt.y, delta);
		break;
	}

	default:
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}


Window::Exception::Exception(int line, const char* file, HRESULT hr) noexcept
	:ChiliException(line, file),
	m_hr(hr)
{

}

const char* Window::Exception::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] " << GetErrorCode() << std::endl
		<< "[Description] " << GetErrorString() << std::endl
		<< GetOriginString();
	m_whatBuffer = oss.str();
	return m_whatBuffer.c_str();
}

const char* Window::Exception::GetType() const noexcept
{
	return "Chili Window Exception";
}

std::string Window::Exception::TranslateErrorCode(HRESULT hr) noexcept
{
	char* pMsgBuf = nullptr;
	DWORD nMsgLen = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr);

	if (nMsgLen == 0)
	{
		return "Unidentified error code";
	}

	std::string errorString = pMsgBuf;
	LocalFree(pMsgBuf);
	return errorString;
}

HRESULT Window::Exception::GetErrorCode() const noexcept
{
	return m_hr;
}

std::string Window::Exception::GetErrorString() const noexcept
{
	return TranslateErrorCode(m_hr);
}
