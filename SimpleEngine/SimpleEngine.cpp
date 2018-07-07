// SimpleEngine.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "SimpleEngine.h"

#define MAX_LOADSTRING 100

#define   VK_A  0x41 
#define   VK_D  0x44 
#define   VK_S  0x53 
#define   VK_W  0x57 
#define   VK_R  0x52
#define   VK_1  0x31
#define   VK_2  0x32
#define   VK_3  0x33
#define   VK_4  0x34
#define	  VK_5  0x35
#define   VK_6  0x36

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
//
Scene *g_pScene=new Scene; 
Camera *g_pCamera=new Camera; 
Light *g_pLights=new Light[2];
Object3D *g_pObject3D = NULL;
Texture *g_pTexture=NULL;
int g_x=0;
int g_y=0;
double g_Angle=0.0;


// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HWND hWnd;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_SIMPLEENGINE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	hInst = hInstance; // Store instance handle in our global variable

	//hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	//     CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW| WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		100, 100, 800, 800, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}
	//Initialize
	g_pCamera->setWCToVCTransform(Vec3(0,0,20),Vec3(0,0,0),Vec3(0,1,0));
	g_pCamera->setVCToNCTransform(90.0,1.0,50.0,1000.0);
	g_pCamera->setNCToVPTransform(0,0,600,600);

	g_pScene->SetWnd(hWnd);
	g_pScene->SetWinWidthAndHeight(0,0,600,600);
	g_pScene->SetShadeMode(SHADE_MODE_NONE);

	g_pObject3D=g_pScene->LoadObject("cube.3d");
	g_pTexture=g_pScene->LoadTexture("checker.bmp");
	g_pObject3D->InitTexture(g_pTexture);
	//Initialize lights info
	g_pLights[0].setLightPosition(Vec4(-10,10.0,10.0,1.0));
	g_pLights[0].bActive = true;
	g_pLights[1].setLightPosition(Vec4(-10,10.0,10.0,1.0));
	g_pLights[1].bActive = true;

	//show window
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SIMPLEENGINE));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}	

	//destroy the scene
	g_pScene->Destroy_Camera(g_pCamera);
	g_pScene->Destroy_Texture(g_pTexture);
	g_pScene->Destroy_Object3D(g_pObject3D);
	if(g_pScene!=NULL)
	{
		delete g_pScene;
		g_pScene=NULL;
	}
	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC; 
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SIMPLEENGINE));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+3);
	wcex.lpszMenuName	= NULL/*MAKEINTRESOURCE(IDC_SIMPLEENGINE)*/;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

void SetDCPixelFormat(HDC hDC) 
{
	int nPixelFormat; 
	static PIXELFORMATDESCRIPTOR pfd = { 
		sizeof(PIXELFORMATDESCRIPTOR), 
		1,
		PFD_DRAW_TO_WINDOW | 
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER, 
		PFD_TYPE_RGBA, 
		32, 
		0, 0, 0, 0, 0, 0, 
		0, 
		0, 
		0,
		0, 0, 0, 0, 
		24, 
		0, 
		0, 
		PFD_MAIN_PLANE, 
		0,
		0, 0, 0 }; 
	nPixelFormat = ChoosePixelFormat(hDC, &pfd);
	SetPixelFormat(hDC, nPixelFormat, &pfd);
}
int InitGL(GLvoid) 
{
	glShadeModel(GL_SMOOTH); 
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); 
	return TRUE;                         
}

void ReShape(int width, int height)
{
	glViewport(0, 0, width, height); 
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluOrtho2D ((GLdouble) (-width)/2, (GLdouble) width/2,(GLdouble) (-height)/2, (GLdouble) height/2);
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	static HDC hdc;
	static HGLRC hrc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_ERASEBKGND:
		return true;// not redraw the background
		break;
	case WM_CREATE: 
		hdc = GetDC(hWnd);
		SetDCPixelFormat(hdc);
		hrc = wglCreateContext(hdc);
		wglMakeCurrent(hdc, hrc); 
		InitGL();
		break;
	case WM_SIZE:
		ReShape(LOWORD(lParam),HIWORD(lParam));
		break;
	case WM_PAINT:
		//RECT rectClient;
		//hdc = BeginPaint(hWnd, &ps);
			//GetClientRect(hWnd,&rectClient);
			//SetMapMode (hdc, MM_ISOTROPIC);
			//SetWindowExtEx (hdc, rectClient.right, rectClient.bottom, NULL);
			//SetViewportExtEx (hdc, rectClient.right, -rectClient.bottom, NULL);
			//SetViewportOrgEx (hdc, rectClient.right/2,rectClient.bottom/2, NULL); 
		
			//X axis,red
			//DDA_DrawLine(hdc,Vec2f(-rectClient.right/3,0),Vec2f(rectClient.right/3,0),RGB(255,0,0),RGB(0,0,255));
			//Y axis,blue
			//DDA_DrawLine(hdc,Vec2f(0,-rectClient.bottom/3),Vec2f(0,rectClient.bottom/3),RGB(0,0,255),RGB(255,0,0));
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f); 
			glClear(GL_COLOR_BUFFER_BIT);
			//glBegin(GL_POINTS);
			g_pScene->DrawObject3D(hdc,g_pObject3D,g_pCamera,g_pLights,2,g_Angle);
			//glEnd();
			//glFlush();

		SwapBuffers(hdc);
		ValidateRect(hWnd, NULL);

		//EndPaint(hWnd, &ps);
		break;
	case WM_KEYDOWN:
            switch (wParam)             
            {               
			case   VK_RETURN:
			case VK_A: //left
				g_pCamera->eye_pos[0]-=1;
				//g_pCamera->setWCToVCTransform(g_pCamera->eye_pos,Vec3(g_x-=1,g_y,0),Vec3(0,1,0));
				g_pCamera->setWCToVCTransform(g_pCamera->eye_pos,Vec3(0,0,0),Vec3(0,1,0));
				InvalidateRect(hWnd,NULL,false);
				UpdateWindow(hWnd);
				break;
			case VK_S: //down
				g_pCamera->eye_pos[1]-=1;
				//g_pCamera->setWCToVCTransform(g_pCamera->eye_pos,Vec3(g_x,g_y-=1,0),Vec3(0,1,0));
				g_pCamera->setWCToVCTransform(g_pCamera->eye_pos,Vec3(0,0,0),Vec3(0,1,0));
				InvalidateRect(hWnd,NULL,false);
				UpdateWindow(hWnd);
				break;
			case VK_D: //right
				g_pCamera->eye_pos[0]+=1;
				//g_pCamera->setWCToVCTransform(g_pCamera->eye_pos,Vec3(g_x+=1,g_y,0),Vec3(0,1,0));
				g_pCamera->setWCToVCTransform(g_pCamera->eye_pos,Vec3(0,0,0),Vec3(0,1,0));
				InvalidateRect(hWnd,NULL,false);
				UpdateWindow(hWnd);	
				break;
			case VK_W: //up
				g_pCamera->eye_pos[1]+=1;
				//g_pCamera->setWCToVCTransform(g_pCamera->eye_pos,Vec3(g_x,g_y+=1,0),Vec3(0,1,0));
				g_pCamera->setWCToVCTransform(g_pCamera->eye_pos,Vec3(0,0,0),Vec3(0,1,0));
				InvalidateRect(hWnd,NULL,false);
				UpdateWindow(hWnd);
				break;
			case VK_1://shade mode: none(wire frame)
				g_pScene->SetShadeMode(SHADE_MODE_NONE);
				InvalidateRect(hWnd,NULL,false);
				UpdateWindow(hWnd);
				break;
			case VK_2://shade mode: constant(the original triangle color)
				g_pScene->SetShadeMode(SHADE_MODE_CONSTANT);
				InvalidateRect(hWnd,NULL,false);
				UpdateWindow(hWnd);
				break;
			case VK_3://shade mode: flat with lights mode
				g_pScene->SetShadeMode(SHADE_MODE_FLAT);
				InvalidateRect(hWnd,NULL,false);
				UpdateWindow(hWnd);
				break;
			case VK_4://shade mode: gouraud
				g_pScene->SetShadeMode(SHADE_MODE_GOURAUD);
				InvalidateRect(hWnd,NULL,false);
				UpdateWindow(hWnd);
				break;
			case VK_5://shade mode: texture
				g_pScene->SetShadeMode(SHADE_MODE_TEXTURE);
				InvalidateRect(hWnd,NULL,false);
				UpdateWindow(hWnd);
				break;
			case VK_6:
				g_pScene->SetShadeMode(SHADE_MODE_NORMAL);
				InvalidateRect(hWnd,NULL,false);
				UpdateWindow(hWnd);
				break;
			case VK_R:
				{
					g_Angle+=5;
					if (g_Angle>=360.0)
					{
						g_Angle=0.0;
					}
					//g_pScene->Rotate3D(g_pObject3D,g_Angle,0.0,1.0,0.0);
					InvalidateRect(hWnd,NULL,false);
					UpdateWindow(hWnd);
				}
				break;
			case VK_ESCAPE: 
				SendMessage(hWnd, WM_CLOSE, 0, 0); 
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
            }        
		break;
	case WM_DESTROY:
		{
			wglMakeCurrent(hdc, NULL);
			wglDeleteContext(hrc);
			DeleteObject(hdc);
			PostQuitMessage(0);
		}
		break;
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
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}


