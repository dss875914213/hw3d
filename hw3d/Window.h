#pragma once
#include "ChiliWin.h"
#include "ChiliException.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include <optional>
#include <memory>

class Window
{
public:
	class Exception :public ChiliException
	{
		using ChiliException::ChiliException;
	public:
		static std::string TranslateErrorCode(HRESULT hr) noexcept;
	};
	class HrException: public Exception
	{
	public:
		HrException(int line, const char* file, HRESULT hr) noexcept;
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorDescription() const noexcept;
	private:
		HRESULT m_hr;
	};

	class NoGfxException :public Exception
	{
	public:
		using Exception::Exception;
		const char* GetType() const noexcept override;
	};

private:
	class WindowClass
	{
	public:
		static const char* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass& ) = delete;
	private:
		static constexpr const char* m_wndClassName = "Chili Direct3D";
		static WindowClass m_wndClass;
		HINSTANCE m_hInst;
	};
public:
	Window(int width, int height, const char* name);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	void SetTitle(const std::string& title);
	static std::optional<int> ProcessMessages() noexcept;
	Graphics& Gfx();
private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
public:
	Keyboard m_kbd;
	Mouse m_mouse;
private:
	int m_width;
	int m_height;
	HWND m_hWnd;
	std::unique_ptr<Graphics> m_pGfx;
};


#define CHWND_EXCEPT(hr) Window::HrException(__LINE__, __FILE__, hr);
#define CHWND_LAST_EXCEPT() Window::HrException(__LINE__, __FILE__, GetLastError())
#define CHWND_NOGFX_EXCEPT() Window::HrException(__LINE__, __FILE__)

