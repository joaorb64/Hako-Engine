#ifdef HAKO_BUILD_WIN32


#include "gfx_opengl.h"
#include <hako/common/engine.h>


#ifdef HAKO_BUILD_GFXOPENGL
	#include <hako/common_gl/render.h>
#endif



void Hako::Win32::GfxOpenGL::init(Hako::Engine* engine)
{
	m_user_quit = false;

	//
	// Create a window descriptor.
	//
	HINSTANCE module_handle         = GetModuleHandle(NULL);

	WNDCLASSEX window_descriptor;
	window_descriptor.cbSize        = sizeof(WNDCLASSEX);
	window_descriptor.style         = CS_OWNDC;
	window_descriptor.lpfnWndProc   = window_callback;
	window_descriptor.cbClsExtra    = 0;
	window_descriptor.cbWndExtra    = 0;
	window_descriptor.hInstance     = module_handle;
	window_descriptor.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	window_descriptor.hCursor       = LoadCursor(NULL, IDC_ARROW);
	window_descriptor.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	window_descriptor.lpszMenuName  = NULL;
	window_descriptor.lpszClassName = "Hako Game Engine";
	window_descriptor.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&window_descriptor))
		HAKO_ERROR("unable to register Win32 window");


	//
	// Instantiate and show window to user.
	//
	m_hwnd = CreateWindowEx(0,
		"Hako Game Engine", "Hako Game Engine",
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		960, 544, NULL, NULL, module_handle, NULL);

	ShowWindow(m_hwnd, true);

	opengl_enable(m_hwnd, &m_hdc, &m_hrc);


	//
	// Register render task.
	//
	Hako::Callback<void, Hako::Engine*> render_callback;
	render_callback.init(this, render);

	Hako::Task render_task;
	render_task.init(render_callback);

	engine->task_add_framesync(render_task);
}



void Hako::Win32::GfxOpenGL::shutdown()
{
	opengl_disable(m_hwnd, m_hdc, m_hrc);
}



void Hako::Win32::GfxOpenGL::process_events()
{
	//
	// Process all queued Win32 event messages.
	//
	MSG window_message;
	while (PeekMessage(&window_message, NULL, 0, 0, PM_REMOVE))
	{
		if (window_message.message == WM_QUIT)
		{
			m_user_quit = true;
		}
		else
		{
			TranslateMessage(&window_message);
			DispatchMessage(&window_message);
		}
	}
}



bool Hako::Win32::GfxOpenGL::did_user_quit()
{
	return m_user_quit;
}



LRESULT CALLBACK Hako::Win32::GfxOpenGL::window_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//
	// Switch on the message kind.
	//
	switch (uMsg)
	{
		case WM_CLOSE:
			PostQuitMessage(0);
			break;

		case WM_DESTROY:
			return 0;

		case WM_KEYDOWN:
		{
			switch (wParam)
			{
			case VK_ESCAPE:
				PostQuitMessage(0);
				break;
			}
		}
			break;

		default:
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return 0;
}



void Hako::Win32::GfxOpenGL::opengl_enable(HWND hwnd, HDC* hdc, HGLRC* hrc)
{
	PIXELFORMATDESCRIPTOR pfd;
	int iFormat;

	*hdc = GetDC(hwnd);
	ZeroMemory(&pfd, sizeof(pfd));
	pfd.nSize      = sizeof(pfd);
	pfd.nVersion   = 1;
	pfd.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;

	iFormat = ChoosePixelFormat(*hdc, &pfd);
	SetPixelFormat(*hdc, iFormat, &pfd);

	*hrc = wglCreateContext(*hdc);
	wglMakeCurrent(*hdc, *hrc);
}



void Hako::Win32::GfxOpenGL::opengl_disable(HWND hwnd, HDC hdc, HGLRC hrc)
{
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hrc);
	ReleaseDC(hwnd, hdc);
}



void Hako::Win32::GfxOpenGL::render(void* userdata, Hako::Engine* engine)
{
	HAKO_UNUSED(engine);
    Hako::Win32::GfxOpenGL* gfx = (Hako::Win32::GfxOpenGL*)userdata;

    Hako::common_opengl_render();
    SwapBuffers(gfx->m_hdc);
}



#endif