#define _CRTDBG_MAP_ALLOC

#include "BR5Model.h"
#include <windows.h>
#include <windowsx.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <crtdbg.h>
#include <stdio.h>
#include <time.h>
#include "Resource.h"
#include "OGLUtils.h"

using namespace loaders;
using namespace utils;

HDC			hDC=NULL;
HGLRC		hRC=NULL;
HWND		hWnd=NULL;
HINSTANCE	hInstance;

bool	keys[256];
bool	active=true;
bool	fullscreen=false;

/********************************************************************************************/

#define WINDOW_TITLE "BR5Viewer"

#define SCREEN_WIDTH	1024
#define SCREEN_HEIGHT	768

CBR5Model br5,br5_1,br5_2;

/********************************************************************************************/

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)
{
	if (height==0)
	{
		height=1;
	}

	glViewport(0,0,width,height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,1000.0f);

	glMatrixMode(GL_MODELVIEW);	
	glLoadIdentity();
}

void DetectMemoryLeaks() 
{ 
   _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF|_CRTDBG_ALLOC_MEM_DF); 
   _CrtSetReportMode(_CRT_ASSERT,_CRTDBG_MODE_FILE); 
   _CrtSetReportFile(_CRT_ASSERT,_CRTDBG_FILE_STDERR); 
} 

int InitGL(GLvoid)
{
	COGLUtils::setVSync(false);
	//setVSync(0); in OGLUTILS!

	DetectMemoryLeaks();

	glShadeModel(GL_SMOOTH);
	glClearColor(0.4f, 0.4f, 0.8f, 0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glAlphaFunc(GL_GREATER,0.1f);
	glEnable(GL_ALPHA_TEST);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	GLfloat pos[] = {0.0f,5.0f,0.0f,1.0f};
	glLightfv(GL_LIGHT0,GL_POSITION,pos);

	/*if (!br5_2.loadFromFile("../../data/resources/models/animated/creatures/imp.br5"))
	{
		return FALSE;
	}
	br5_2.setAnimSpeed(0.002f);
	br5_2.setInterpolate(true);
	int claiming = br5_2.registerAction(0,4);
	int running = br5_2.registerAction(5,9);
	br5_2.doAction(claiming);
	br5_2.scaleToMaxY(0.15f);*/

	/*if (!br5.loadFromFile("../../data/resources/models/animated/creatures/fly.br5"))
	{
		return FALSE;
	}
	br5.scaleToMaxY(0.1f);*/

	/*if (!br5.loadFromFile("../../data/resources/models/animated/creatures/bile_demon.br5"))
	{
		return FALSE;
	}
	br5.setAnimSpeed(0.01f);
	br5.setInterpolate(true);
	br5.scaleToMaxY(0.2f);

	if (!br5_1.loadFromFile("../../data/resources/models/animated/creatures/fly.br5"))
	{
		return FALSE;
	}
	br5_1.scaleToMaxY(0.05f);*/

	if (!br5_1.loadFromFile("beast_ms3d\\beast.br5"))
	{
		return FALSE;
	}
	br5_1.setAnimSpeed(0.002f);
	br5_1.setInterpolate(true);
	br5_1.setConnected(true);
	br5_1.scaleToMaxY(0.2f);	

	int Walk = br5_1.registerAction(0, 5);
	int Emerge = br5_1.registerAction(6, 17);
	int Transition = br5_1.registerAction(71-1, 80-1);
	int Jump1 = br5_1.registerAction(82-1, 100-1);
	int Jump2 = br5_1.registerAction(102-1, 120-1);
	int Rear = br5_1.registerAction(122-1, 150-1);
	int Crouch = br5_1.registerAction(152-1, 180-1);
	int Headbutt = br5_1.registerAction(182-1, 195-1);
	int Bite1 = br5_1.registerAction(197-1, 210-1);
	int Bite2 = br5_1.registerAction(212-1, 230-1);
	int Stomp = br5_1.registerAction(232-1, 270-1);
	int Take = br5_1.registerAction(272-1, 288-1);
	int Look = br5_1.registerAction(290-1, 330-1);
	int Idle = br5_1.registerAction(332-1, 380-1);
	int Die = br5_1.registerAction(382-1, 400-1);

	br5_1.doAction(Walk);

	
	/*if (!br5.loadFromFile("../../data/resources/models/animated/creatures/beetle.br5"))
	{
		return FALSE;
	}
	br5.setAnimSpeed(0.001f);
	br5.setInterpolate(true);*/

	/*if (!br5.loadFromFile("../../data/resources/models/static/statue_unlit.br5"))
	{
		return FALSE;
	}*/

	return TRUE;
}

float angle = 0.0f;

int DrawGLScene(GLvoid)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glTranslatef(0.0f,-0.0f,-1.0f);
	br5_1.draw();
	
	/*angle+=0.01f;
	glTranslatef(0.0f,-0.0f,-1.0f);
	glRotatef(angle,0.0f,1.0f,0.0f);
	br5.draw();
	glTranslatef(0.2f,0.0f,0.0f);
	br5_1.draw();
	glTranslatef(-0.4f,0.0f,0.0f);
	br5_2.draw();*/
	
	return TRUE;
}

GLvoid KillGLWindow(GLvoid)
{
	if (fullscreen)
	{
		ChangeDisplaySettings(NULL,0);
		ShowCursor(TRUE);								
	}

	if (hRC)											
	{
		if (!wglMakeCurrent(NULL,NULL))					
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										
	}

	if (hWnd && !DestroyWindow(hWnd))					
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										
	}

	if (!UnregisterClass("OpenGL",hInstance))			
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									
	}
}
 
BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;			
	WNDCLASS	wc;						
	DWORD		dwExStyle;				
	DWORD		dwStyle;				
	RECT		WindowRect;				
	WindowRect.left=(long)0;			
	WindowRect.right=(long)width;		
	WindowRect.top=(long)0;				
	WindowRect.bottom=(long)height;		

	fullscreen=fullscreenflag;			

	hInstance			= GetModuleHandle(NULL);		
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	
	wc.lpfnWndProc		= (WNDPROC) WndProc;					
	wc.cbClsExtra		= 0;									
	wc.cbWndExtra		= 0;									
	wc.hInstance		= hInstance;							
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			
	wc.hbrBackground	= NULL;									
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= "OpenGL";								

	if (!RegisterClass(&wc))									
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											
	}
	
	if (fullscreen)												
	{
		DEVMODE dmScreenSettings;								
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		
		dmScreenSettings.dmPelsWidth	= width;				
		dmScreenSettings.dmPelsHeight	= height;				
		dmScreenSettings.dmBitsPerPel	= bits;					
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;
		
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{			
			if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","NeHe GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				fullscreen=FALSE;
			}
			else
			{
				MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
				return FALSE;
			}
		}
	}

	if (fullscreen)	
	{
		dwExStyle=WS_EX_APPWINDOW;								
		dwStyle=WS_POPUP;										
		ShowCursor(FALSE);										
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			
		dwStyle=WS_OVERLAPPEDWINDOW;							
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		

	if (!(hWnd=CreateWindowEx(	dwExStyle,							
								"OpenGL",							
								title,								
								dwStyle |							
								WS_CLIPSIBLINGS |					
								WS_CLIPCHILDREN,					
								0, 0,								
								WindowRect.right-WindowRect.left,	
								WindowRect.bottom-WindowRect.top,	
								NULL,								
								NULL,								
								hInstance,							
								NULL)))								
	{
		KillGLWindow();								
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	static	PIXELFORMATDESCRIPTOR pfd=				
	{
		sizeof(PIXELFORMATDESCRIPTOR),				
		1,											
		PFD_DRAW_TO_WINDOW |						
		PFD_SUPPORT_OPENGL |						
		PFD_DOUBLEBUFFER,							
		PFD_TYPE_RGBA,								
		bits,										
		0, 0, 0, 0, 0, 0,							
		0,											
		0,											
		0,											
		0, 0, 0, 0,									
		16,											
		0,											
		0,											
		PFD_MAIN_PLANE,								
		0,											
		0, 0, 0										
	};
	
	if (!(hDC=GetDC(hWnd)))							
	{
		KillGLWindow();								
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	
	{
		KillGLWindow();								
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		
	{
		KillGLWindow();								
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	if (!(hRC=wglCreateContext(hDC)))				
	{
		KillGLWindow();								
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	if(!wglMakeCurrent(hDC,hRC))					
	{
		KillGLWindow();								
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	ShowWindow(hWnd,SW_SHOW);						
	SetForegroundWindow(hWnd);						
	SetFocus(hWnd);									
	ReSizeGLScene(width, height);					

	if (!InitGL())									
	{
		KillGLWindow();								
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	return TRUE;									
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)			
{
	switch (uMsg)									
	{
		case WM_ACTIVATE:							
		{
			if ((LOWORD(wParam) != WA_INACTIVE) && !((BOOL)HIWORD(wParam)))
				active=TRUE;						
			else
				active=FALSE;

			return 0;								
		}

		case WM_SYSCOMMAND:							
		{
			switch (wParam)							
			{
				case SC_SCREENSAVE:					
				case SC_MONITORPOWER:				
				return 0;							
			}
			break;									
		}

		case WM_CLOSE:								
		{
			PostQuitMessage(0);						
			return 0;								
		}

		case WM_KEYDOWN:							
		{
			keys[wParam] = TRUE;					
			return 0;								
		}

		case WM_KEYUP:								
		{
			keys[wParam] = FALSE;					
			return 0;								
		}

		case WM_SIZE:								
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  
			return 0;								
		}		

		case WM_LBUTTONDOWN:
		{
			return 0;
		}

		case WM_RBUTTONDOWN:
		{			
			return 0;
		}

		case WM_RBUTTONUP:
		{
			return 0;
		}

		case WM_LBUTTONUP:
		{
			return 0;
		}

		case WM_MOUSEMOVE:
		{
			return 0;
		}
	}

	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

int WINAPI WinMain(	HINSTANCE	hInstance,			
					HINSTANCE	hPrevInstance,		
					LPSTR		lpCmdLine,			
					int			nCmdShow)			
{
	MSG		msg;									
	BOOL	done=FALSE;								
	
	/*if (MessageBox(NULL,"Would You Like To Run In Fullscreen Mode?", "Start FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDNO)
	{	
		fullscreen=FALSE;
	}*/

	fullscreen=FALSE;

	if (!CreateGLWindow(WINDOW_TITLE,SCREEN_WIDTH,SCREEN_HEIGHT,32,fullscreen))
	{
		return 0;									
	}

	while(!done)									
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	
		{
			if (msg.message==WM_QUIT)				
			{
				done=TRUE;							
			}
			else									
			{
				TranslateMessage(&msg);				
				DispatchMessage(&msg);				
			}
		}
		else										
		{
			
			if ((active && !DrawGLScene()) || keys[VK_ESCAPE])	
			{
				done=TRUE;							
			}
			else									
			{
				SwapBuffers(hDC);					
			}

			if (keys[VK_F1])						
			{
				keys[VK_F1]=FALSE;					
				KillGLWindow();						
				fullscreen=!fullscreen;				
				
				if (!CreateGLWindow(WINDOW_TITLE,SCREEN_WIDTH,SCREEN_HEIGHT,32,fullscreen))
				{
					return 0;						
				}
			}
		}
	}

	KillGLWindow();									
	return (msg.wParam);							
}