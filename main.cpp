#include <Windows.h>
#include <mmsystem.h>
#include <d3dx9.h>
#include ".\Environment.h"
#include ".\timeManager.h"

LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{

	case WM_DESTROY:

		PostQuitMessage( 0 );
		return 0;

	}

	return DefWindowProc( hWnd, msg, wParam, lParam );
}

INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{
	// Register the window class
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		"Cubic Shadow Mapping", NULL };
	RegisterClassEx( &wc );

	// Create the application's window
	HWND hWnd = CreateWindow( "Cubic Shadow Mapping", "Cubic Shadow Mapping",
		WS_OVERLAPPEDWINDOW, 0, 0, 800, 600,
		GetDesktopWindow(), NULL, wc.hInstance, NULL );

	//////////////////////////////////////////////////////////////////////
	//initialization	
	//////////////////////////////////////////////////////////////////////
	Environment shadowDemo;
	if( !(shadowDemo.Initialise(hWnd, hInst, 800, 600, true)) )
	{
		MessageBoxA(NULL, "Initializing the demo failed.", NULL, MB_OK);
		return 0;
	}

	TimeManager timeManager;
	//////////////////////////////////////////////////////////////////////
	//....................................................................
	//////////////////////////////////////////////////////////////////////

	ShowWindow( hWnd, SW_SHOWDEFAULT );
	UpdateWindow( hWnd );

	// Enter the message loop
	MSG msg;
	ZeroMemory( &msg, sizeof(msg) );

	while( msg.message != WM_QUIT )
	{
		if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			//////////////////////////////////////////////////////////////////////
			//run	
			//////////////////////////////////////////////////////////////////////
			timeManager.UpdateLastTime();
			shadowDemo.Render(timeManager.GetTimeDelta());
			timeManager.UpdateCurrentTime();
			timeManager.UpdateTimeDelta();
			//////////////////////////////////////////////////////////////////////
			//....................................................................
			//////////////////////////////////////////////////////////////////////
		}
	}

	UnregisterClass( "Cubic Shadow Mapping", wc.hInstance );

	return 0;
}