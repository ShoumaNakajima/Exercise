#include<windows.h>
#include<d3dx9.h>
#include <d3d9.h>
#include<mmsystem.h>
#include<tchar.h>

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

//カスタムバーテックスの構造体
struct CUSTOMVERTEX
{
	FLOAT	x, y, z, rhw;
	DWORD	color;
	FLOAT	tu, tv;
};

//カスタムバーテックス
CUSTOMVERTEX CustomVertex[4]
{
	{10,10,1,1,0xffffffff,0,0},
	{200,10,1,1,0xffffffff,1,0},
	{200,200,1,1,0xffffffff,1,1},
	{10,200,1,1,0xffffffff,0,1},
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd_a = NULL;//ウィンドウハンドルの値を格納・最低1つのウィンドウを作成しOS側はハンドルによってウィンドウの管理をする
	MSG msg;//OSから受け取るメッセージを格納

	hWnd_a = createWindow(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

	BuildDxDevice(hWnd_a, _T("ヨッシー.png"));

	D3DXCreateTextureFromFile(
		pDevice,
		"ヨッシー.png",
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
		0, 0, 640, 480, NULL, NULL, hInst, NULL);
	ShowWindow(hWnd_b, SW_SHOW);
	UpdateWindow(hWnd_b);

	return hWnd_b;
}

HRESULT BuildDxDevice(HWND hWnd, const TCHAR* filepath) {
	if (FAILED(InitD3d(hWnd, filepath))) {
		return E_FAIL;
	}
	pD3d = Direct3DCreate9(D3D_SDK_VERSION);

	if (pD3d == NULL) {
		MessageBox(0, "Direct3Dの作成に失敗しました", NULL, MB_OK);
		return E_FAIL;
	}
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
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
		filepath,			//ビットマップファイル名
		100,				//ビットマップファイルのサイズ縦
		100,				//ビットマップファイルのサイズ横
		0,					//ビット77マップファイルのサイズ0
		0,					//ビットマップファイルのサイズ0
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
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0x00, 0x00, 0x00), 1.0f, 0);//画面をきれいにする
	//テクスチャ貼り付け開始
	pDevice->BeginScene();
	// テクスチャのセット
	pDevice->SetTexture(0, pTexture);
	// 頂点リストの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, CustomVertex, sizeof(CUSTOMVERTEX));
	//テクスチャ貼り付け終了
	pDevice->EndScene();//描画の最後にコール
	pDevice->Present(NULL, NULL, NULL, NULL);//書いた画面を持ってくる
}

void InitPresentParameter(HWND hWnd) {

	ZeroMemory(&D3dPresentParameters, sizeof(D3dPresentParameters));

	//構造体のメンバを初期化
	//画面表示はフロントバッファとバックバッファを切り替える・フロントバッファとバックバッファの切り替えをフリップ
	D3dPresentParameters.BackBufferFormat = D3DFMT_UNKNOWN;//バックバッファの表示形式・D3DFMT_UNKNOWNでデスクトップの表示モードが自動設定
	D3dPresentParameters.BackBufferCount = 1;//バックバッファの数

	D3dPresentParameters.BackBufferHeight = 480;
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