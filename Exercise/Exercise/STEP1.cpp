
#pragma comment (lib,"winmm.lib")


#include<windows.h>
#include<mmsystem.h>

LRESULT CALLBACK WndProc
(
	HWND,
	UINT,
	WPARAM,
	LPARAM
);//ウィンドウプロシージャー関数プロトタイプ宣言
HWND createWindow
(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	PSTR lpCmdLine,
	int nCmdShow
);

int WINAPI WinMain
(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	PSTR lpCmdLine,
	int nCmdShow
)
{
	HWND hWnd = createWindow
	(
		hInstance,
		hPrevInstance,
		lpCmdLine,
		nCmdShow
	);//ウィンドウハンドルの値を格納・最低1つのウィンドウを作成しOS側はハンドルによってウィンドウの管理をする
	
	MSG msg;//OSから受け取るメッセージを格納

	void message();
		
	//	システム時間を取得
	DWORD SyncPrev = timeGetTime();
	DWORD SyncCurr;

	ZeroMemory
	(
		&msg,
		sizeof(msg)
	);//msg構造体を初期化
	timeBeginPeriod(1);
	while (msg.message != WM_QUIT)//msg構造体のmessageがWM_QUITでない限りループ
	{
		Sleep(1);
		if (PeekMessage
		(
			&msg,
			NULL,
			0U,
			0U,
			PM_REMOVE
		))//OSからのメッセージを調べる//メッセージキューにメッセージがあるか調べ、引数のMSG構造体似メッセージを格納
		{//ある場合
		//ウィンドウプロシージャ関数に渡す・転送
			TranslateMessage
			(
				&msg
			);
			DispatchMessage
			(
				&msg
			);
		}
		else
		{//ない場合→一般的にアプリケーションの処理を書く

		}
	}
	timeEndPeriod(1);
	return (INT)msg.wParam;//msgの構造体に格納・msg　＝メッセージ
}


LRESULT CALLBACK WndProc
(
	HWND hWnd,
	UINT iMsg,
	WPARAM wParam,
	LPARAM lParam
)
{
	switch (iMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN://キーが押したとき
		switch ((CHAR)wParam)
		{
		case VK_ESCAPE://エスケープ
			PostQuitMessage(0);//アプリケーションの終了
			break;
		}
		break;
	}
	//アプリケーションが処理しなかったメッセージをシステムに処理してもらう
	//アプリケーションに不要なメッセージはすべてシステム任せ
	return DefWindowProc
	(
		hWnd,
		iMsg,
		wParam,
		lParam
	);
}

HWND createWindow
(
	HINSTANCE hInst,//
	HINSTANCE hPrevInstance,//
	PSTR lpCmdLine,//
	int nCmdShow
)//
{
	HWND hWnd = NULL;//ウィンドウハンドルの値を格納・最低1つのウィンドウを作成しOS側はハンドルによってウィンドウの管理をする
	MSG msg;//OSから受け取るメッセージを格納
	//ウィンドウの初期化
	static char szAppName[] = "Chapter2";
	WNDCLASSEX wndclass;

	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;//classstale_水平に再描画|classstale_垂直に再描画
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInst;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&wndclass);

	hWnd = CreateWindow(
		szAppName,
		szAppName,
		WS_OVERLAPPEDWINDOW,
		0,
		0,
		640,
		480,
		NULL,
		NULL,
		hInst,
		NULL);
	ShowWindow(
		hWnd,
		SW_SHOW);
	UpdateWindow(hWnd);
	return hWnd;
}