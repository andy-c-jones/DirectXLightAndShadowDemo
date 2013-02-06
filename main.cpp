#include <Windows.h>
#include <mmsystem.h>
#include <d3dx10.h>
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
	WNDCLASSEX wc = 
		{
			sizeof(WNDCLASSEX),
			CS_CLASSDC,
			MsgProc,
			0L,
			0L,
			GetModuleHandle(NULL),
			NULL,
			NULL,
			NULL,
			NULL,
			"Direct3D Game",
			NULL 
		};
	RegisterClassEx( &wc );

	HWND hWnd = CreateWindow("Direct3D Game",
							 "Direct3D Game",
							 WS_OVERLAPPEDWINDOW,
							 0,
							 0,
							 800,
							 600,
							 GetDesktopWindow(),
							 NULL,
							 wc.hInstance,
							 NULL);

	Environment game;
	if( !(game.Initialise(hWnd, hInst, 800, 600, true)) )
	{
		MessageBoxA(NULL, "Failed to initialise the game.", NULL, MB_OK);
		return 0;
	}

	TimeManager timeManager;

	ShowWindow( hWnd, SW_SHOWDEFAULT );
	UpdateWindow( hWnd );

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
			timeManager.UpdateLastTime();
			game.Render(timeManager.GetTimeDelta());
			timeManager.UpdateCurrentTime();
			timeManager.UpdateTimeDelta();
		}
	}

	UnregisterClass( "Direct3D Game", wc.hInstance );

	return 0;
}