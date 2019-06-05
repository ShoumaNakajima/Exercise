#include<windows.h>
#include<d3dx9.h>
#include <d3d9.h>
#include<mmsystem.h>
#include<tchar.h>
#include<dinput.h>
#include<d3dx9core.h>

#include<d3d9types.h>

#pragma comment (lib,"dinput8.lib")

LPDIRECT3D9 pD3d;//ダイレクト3Dオブジェクト・ポインタ宣言
LPDIRECT3DDEVICE9 pDevice;//ダイレクト3Dデバイスオブジェクト・ポインタ宣言//描画担当
LPDIRECT3DTEXTURE9 pTexture;//ダイレクト3Dテクスチャオブジェクト・ポインタ宣言//貼り付け担当
D3DPRESENT_PARAMETERS D3dPresentParameters;//ウィンドウ設定

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);//ウィンドウプロシージャー関数プロトタイプ宣言
HWND createWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow);
HRESULT BuildDxDevice(HWND, const TCHAR*);
HRESULT InitD3d(HWND hWnd, const TCHAR*);
void InitPresentParameter(HWND hWnd);
VOID Draw();

LPDIRECTINPUTDEVICE8 p_inputdevice;
LPDIRECTINPUT8 p_input;
BYTE key[256];

//左上(a,b)右下(x,y)
struct TopLeftPosision
{
	FLOAT x, y;
	FLOAT Heght,Width;
};

TopLeftPosision topleftposision = {10,10,200,200};

//カスタムバーテックスの構造体
struct CUSTOMVERTEX
{
	FLOAT	x, y, z, rhw;
	DWORD	color;
	FLOAT	tu, tv;
};

CUSTOMVERTEX CustomVertex[4]
{
	{topleftposision.x,topleftposision.y,0,1,0xffffffff,0,0},
	{topleftposision.x + topleftposision.Width,topleftposision.y,0,1,0xffffffff,1,0},
	{topleftposision.x + topleftposision.Width,topleftposision.y + topleftposision.Heght,0,1,0xffffffff,1,1},
	{topleftposision.x,topleftposision.y + topleftposision.Heght,0,1,0xffffffff,0,1},
};

struct WindowSize
{
	int height;
	int width;
};

WindowSize windowsize ={480,680};


//DirectInput初期化関数

HRESULT InitDinput(HWND hWnd) {
	HRESULT hr;
	// 「DirectInput」オブジェクトの作成
	if (FAILED(hr = DirectInput8Create(
		GetModuleHandle(NULL),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(VOID * *)& p_input,
		NULL)))//ポインタ
	{
		return hr;
	}
	// 「DirectInputデバイス」オブジェクトの作成
	if (FAILED(hr = p_input->CreateDevice(
		GUID_SysKeyboard,//キーボードはGUID_SysKeyboard・マウスはGUID_SysMouse
		&p_inputdevice,//ポインタ
		NULL)))
	{
		return hr;
	}
	// デバイスをキーボードに設定
	if (FAILED(hr = p_inputdevice->SetDataFormat(&c_dfDIKeyboard)))//謎
	{
		return hr;
	}
	// 協調レベルの設定
	//排他的or非排他的・DISCL_EXCLUSIVE or DISCL_NONEXCLUSIVE 
	//フォアグラウンドorバックグラウンド・DISCL_FOREGROUND or DISCL_BACKGROUND
	//一応「DISCL_NOWINKEY」はキー入力のウィンドウメッセージを無効にする
	if (FAILED(hr = p_inputdevice->SetCooperativeLevel(
		hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
	{
		return hr;
	}
	// デバイスを「取得」する
	p_inputdevice->Acquire();
	return S_OK;
}

void UpdateKeyStatus() {
	//キーボードの押下状態を保存
	HRESULT hr = p_inputdevice->Acquire();//BYTE diks[256]
	if ((hr == DI_OK) || (hr == S_FALSE)) {
		p_inputdevice->GetDeviceState(sizeof(key), &key);
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd_a = NULL;//ウィンドウハンドルの値を格納・最低1つのウィンドウを作成しOS側はハンドルによってウィンドウの管理をする
	MSG msg;//OSから受け取るメッセージを格納

	hWnd_a = createWindow(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

	BuildDxDevice(hWnd_a, _T("ヨッシー.png"));

	D3DXCreateTextureFromFile(
		pDevice,
		"ヨッシー.png",//ファイル名
		&pTexture);


	//	システム時間を取得
	DWORD previous = timeGetTime();

	ZeroMemory(&msg, sizeof(msg));//msg構造体を初期化

	timeBeginPeriod(1);

	while (msg.message != WM_QUIT)//msg構造体のmessageがWM_QUITでない限りループ
	{
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))//OSからのメッセージを調べる//メッセージキューにメッセージがあるか調べ、引数のMSG構造体似メッセージを格納
		{//ある場合
		//ウィンドウプロシージャ関数に渡す・転送
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{//ない場合→一般的にアプリケーションの処理を書く
			DWORD current = timeGetTime();
			
			if (current - previous >= 1000 / 60) {

				//キーボードの押下状態を保存
				UpdateKeyStatus();

				//ダイレクトインプット
				if (key[DIK_UP] & 0x80)
				{
					if (topleftposision.y >= 0) 
					{
						topleftposision.y -= 10.f;
					}
				}
				if (key[DIK_RIGHT] & 0x80)
				{
					if (topleftposision.x + topleftposision.Width <= windowsize.width)
					{
						topleftposision.x += 10.f;
					}
				}
				if (key[DIK_LEFT] & 0x80)
				{
					if (topleftposision.x >= 0)
					{
						topleftposision.x -= 10.f;
					}
				}
				if (key[DIK_DOWN] & 0x80)
				{
					if (topleftposision.y + topleftposision.Heght <= windowsize.height)
					{
						topleftposision.y += 10.f;
					}
				}
				/*
				// カスタムバーテックス
				CUSTOMVERTEX CustomVertex[4]
				{
					{topleftposision.x,topleftposision.y,0,1,0xffffffff,0,0},
					{topleftposision.x + topleftposision.Width,topleftposision.y,0,1,0xffffffff,1,0},
					{topleftposision.x + topleftposision.Width,topleftposision.y + topleftposision.Heght,0,1,0xffffffff,1,1},
					{topleftposision.x,topleftposision.y + topleftposision.Heght,0,1,0xffffffff,0,1},
				};
				*/
				Draw();
								
				previous = current;
			}
			
		}
		Sleep(1);
	}
	timeEndPeriod(1);
	pDevice->Release();
	pDevice = nullptr;
	pD3d->Release();
	pD3d = nullptr;
	return (INT)msg.wParam;//msgの構造体に格納・msg　＝メッセージ
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
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
	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

HWND createWindow(HINSTANCE hInst, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd_b = NULL;//ウィンドウハンドルの値を格納・最低1つのウィンドウを作成しOS側はハンドルによってウィンドウの管理をする
	MSG msg;//OSから受け取るメッセージを格納
	//ウィンドウの初期化
	static char szAppName[] = _T("Chapter3");
	WNDCLASSEX wndclass;

	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;//classstale_水平に再描画|classstale_垂直に再描画
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInst;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&wndclass);

	hWnd_b = CreateWindow(szAppName, szAppName, WS_OVERLAPPEDWINDOW,
		0, 0, windowsize.width,windowsize.height , NULL, NULL, hInst, NULL);
	ShowWindow(hWnd_b, SW_SHOW);
	UpdateWindow(hWnd_b);

	return hWnd_b;
}

HRESULT BuildDxDevice(HWND hWnd, const TCHAR* filepath) {
	if (FAILED(InitD3d(hWnd, filepath))) {
		return E_FAIL;
	}
	if (FAILED(InitDinput(hWnd))) {
		return E_FAIL;
	}
	pD3d = Direct3DCreate9(D3D_SDK_VERSION);

	if (pD3d == NULL) {
		MessageBox(0, "Direct3Dの作成に失敗しました", NULL, MB_OK);
		return E_FAIL;
	}
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	//頂点フォーマットの伝達・頂点座標、頂点の色、
	pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	return S_OK;
}

HRESULT InitD3d(HWND hWnd, const TCHAR * filepath)
{
	//Direct3Dの作成
	//Direct3Dcreate9はDirectXSDKで用意されているグローバル関数
	//Direct3Dcreate9でDirect3Dオブジェクトを作成し、ポインタを得る
	//Direct3DCreate9がDirect3Dオブジェクトを生成
	if (NULL == (pD3d = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		MessageBox(0, "Direct3Dの作成に失敗しました", "", MB_OK);
		return E_FAIL;
	}

	//「Direct3Dデバイス」オブジェクトの作成
	//「Direct3Dデバイス」オブジェクトはテクスチャオブジェクトとスプライトオブジェクトに必要

	InitPresentParameter(hWnd);

	if (FAILED(pD3d->CreateDevice
	(
		D3DADAPTER_DEFAULT,//パソコンにビデオカードが複数搭載されていない限り子も序数を使う
		D3DDEVTYPE_HAL,//表示はハードウェアかソフトウェアのどちらかで実現しますが、ハードウェアで実現するほうがかなり高速
		hWnd,//アプリケーションのウィンドウハンドル
		D3DCREATE_MIXED_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,//割愛
		&D3dPresentParameters,//D3DPRESENT_PARAMETERS変数のアドレス
		&pDevice))//CreateDeviceをコールする前は内容がからのデバイスオブジェクトへのポインタ変数のアドレス
		)
		//CreateDeviceをコールしないと使えない
	{
		MessageBox(0, "HALモードでDIRECT3Dデバイスを作成できません\nREFモードで再試行します", NULL, MB_OK);
		if (FAILED(pD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd,
			D3DCREATE_MIXED_VERTEXPROCESSING,
			&D3dPresentParameters, &pDevice)))
		{
			MessageBox(0, "DIRECT3Dデバイスの作成に失敗しました", NULL, MB_OK);
			return E_FAIL;
		}
	}

	//テクスチャオブジェクトの作成
	if (FAILED(D3DXCreateTextureFromFileEx(
		pDevice,			//デバイスオブジェクトのポインタ
		filepath,			//ファイル名
		100,				//ファイルのサイズ縦
		100,				//ファイルのサイズ横
		0,					//ファイルのサイズ0
		0,					//ファイルのサイズ0
		D3DFMT_UNKNOWN,		//割愛 
		D3DPOOL_DEFAULT,	//割愛
		D3DX_FILTER_NONE,	//割愛
		D3DX_DEFAULT,		// 割愛
		0xff000000,			//透明度,赤、緑、青・A、R、G、B
		NULL,				//割愛
		NULL,				//割愛
		&pTexture)))		//テクスチャオブジェクトのポインタ変数のアドレス
	{
		MessageBox(0, "テクスチャの作成に失敗しました", "", MB_OK);
		return E_FAIL;
	}
	return S_OK;
}

//描画関数
VOID Draw()
{

	// カスタムバーテックス
	CUSTOMVERTEX CustomVertex[4]
	{
		{topleftposision.x,topleftposision.y,0,1,0xffffffff,0,0},
		{topleftposision.x + topleftposision.Width,topleftposision.y,0,1,0xffffffff,1,0},
		{topleftposision.x + topleftposision.Width,topleftposision.y + topleftposision.Heght,0,1,0xffffffff,1,1},
		{topleftposision.x,topleftposision.y + topleftposision.Heght,0,1,0xffffffff,0,1},
	};
	
	// デプスバッファ、ステンシルバッファもある
	
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET,//クリアフラグ・バックバッファ|STANCEL|DEPS
		D3DCOLOR_XRGB(0x00, 0x00, 0x00),//クリアする色
		1.0f, 0);//画面をきれいにする
	//テクスチャ貼り付け開始
	pDevice->BeginScene();
	// 頂点リストの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, CustomVertex, sizeof CUSTOMVERTEX);
	// テクスチャのセット
	pDevice->SetTexture(0, pTexture);
	//テクスチャ貼り付け終了
	pDevice->EndScene();//描画の最後にコール
	pDevice->Present(NULL, NULL, NULL, NULL);//書いた画面を持ってくる・描画内容をguiへ送信
}

void InitPresentParameter(HWND hWnd) {

	ZeroMemory(&D3dPresentParameters, sizeof(D3dPresentParameters));

	//構造体のメンバを初期化
	//画面表示はフロントバッファとバックバッファを切り替える・フロントバッファとバックバッファの切り替えをフリップ
	
	//バックバッファの表示形式・D3DFMT_UNKNOWNでデスクトップの表示モードが自動設定,色
	D3dPresentParameters.BackBufferFormat = D3DFMT_UNKNOWN;
	;//バックバッファの数
	D3dPresentParameters.BackBufferCount = 1;
	//バックバッファの高さ
	D3dPresentParameters.BackBufferHeight = 480;
	//バックバッファの幅
	D3dPresentParameters.BackBufferWidth = 640;
	D3dPresentParameters.MultiSampleType = D3DMULTISAMPLE_NONE;
	D3dPresentParameters.MultiSampleQuality = 0;
	D3dPresentParameters.hDeviceWindow = hWnd;
	D3dPresentParameters.EnableAutoDepthStencil = FALSE;
	D3dPresentParameters.AutoDepthStencilFormat = D3DFMT_A1R5G5B5;
	D3dPresentParameters.Flags = 0;
	D3dPresentParameters.FullScreen_RefreshRateInHz = 0;
	D3dPresentParameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	D3dPresentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;//フリップの形態
	D3dPresentParameters.Windowed = TRUE;//ウィンドウモードかフルスクリーンモードか・true＝ウィンドウモード・false＝フルスクリーンモード
}

