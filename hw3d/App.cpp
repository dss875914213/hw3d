#include "App.h"

App::App()
	:m_wnd(800, 600, "The Donkey ")
{

}

int App::Go()
{
	while (true)
	{
		if (const auto ecode = Window::ProcessMessages())
		{
			return *ecode;
		}
		DoFrame();
	}
}

void App::DoFrame()
{
	const float c = sin(m_timer.Peek()) / 2.0f + 0.5f;
	m_wnd.Gfx().ClearBuffer(c, c, 1.0f);
	m_wnd.Gfx().DrawTestTriangle();
	m_wnd.Gfx().EndFrame();
}
