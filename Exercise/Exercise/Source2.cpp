#include<windows.h>
#include<d3dx9.h>
#include <d3d9.h>
#include<mmsystem.h>
#include<tchar.h>

LPDIRECT3D9 pD3d;//�_�C���N�g3D�I�u�W�F�N�g�E�|�C���^�錾
LPDIRECT3DDEVICE9 pDevice;//�_�C���N�g3D�f�o�C�X�I�u�W�F�N�g�E�|�C���^�錾//�`��S��
LPDIRECT3DTEXTURE9 pTexture;//�_�C���N�g3D�e�N�X�`���I�u�W�F�N�g�E�|�C���^�錾//�\��t���S��
D3DPRESENT_PARAMETERS D3dPresentParameters;//�E�B���h�E�ݒ�

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);//�E�B���h�E�v���V�[�W���[�֐��v���g�^�C�v�錾
HWND createWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow);
HRESULT BuildDxDevice(HWND, const TCHAR*);
HRESULT InitD3d(HWND hWnd, const TCHAR*);
void InitPresentParameter(HWND hWnd);
VOID Draw();

//�J�X�^���o�[�e�b�N�X�̍\����
struct CUSTOMVERTEX
{
	FLOAT	x, y, z, rhw;
	DWORD	color;
	FLOAT	tu, tv;
};

//�J�X�^���o�[�e�b�N�X
CUSTOMVERTEX CustomVertex[4]
{
	{10,10,1,1,0xffffffff,0,0},
	{200,10,1,1,0xffffffff,1,0},
	{200,200,1,1,0xffffffff,1,1},
	{10,200,1,1,0xffffffff,0,1},
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd_a = NULL;//�E�B���h�E�n���h���̒l���i�[�E�Œ�1�̃E�B���h�E���쐬��OS���̓n���h���ɂ���ăE�B���h�E�̊Ǘ�������
	MSG msg;//OS����󂯎�郁�b�Z�[�W���i�[

	hWnd_a = createWindow(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

	BuildDxDevice(hWnd_a, _T("���b�V�[.png"));

	D3DXCreateTextureFromFile(
		pDevice,
		"���b�V�[.png",
		&pTexture);

	//	�V�X�e�����Ԃ��擾
	DWORD previous = timeGetTime();

	ZeroMemory(&msg, sizeof(msg));//msg�\���̂�������

	timeBeginPeriod(1);

	while (msg.message != WM_QUIT)//msg�\���̂�message��WM_QUIT�łȂ����胋�[�v
	{
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))//OS����̃��b�Z�[�W�𒲂ׂ�//���b�Z�[�W�L���[�Ƀ��b�Z�[�W�����邩���ׁA������MSG�\���̎����b�Z�[�W���i�[
		{//����ꍇ
		//�E�B���h�E�v���V�[�W���֐��ɓn���E�]��
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{//�Ȃ��ꍇ����ʓI�ɃA�v���P�[�V�����̏���������
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
	return (INT)msg.wParam;//msg�̍\���̂Ɋi�[�Emsg�@�����b�Z�[�W
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN://�L�[���������Ƃ�
		switch ((CHAR)wParam)
		{
		case VK_ESCAPE://�G�X�P�[�v
			PostQuitMessage(0);//�A�v���P�[�V�����̏I��
			break;
		}
		break;
	}
	//�A�v���P�[�V�������������Ȃ��������b�Z�[�W���V�X�e���ɏ������Ă��炤
	//�A�v���P�[�V�����ɕs�v�ȃ��b�Z�[�W�͂��ׂăV�X�e���C��
	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

HWND createWindow(HINSTANCE hInst, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd_b = NULL;//�E�B���h�E�n���h���̒l���i�[�E�Œ�1�̃E�B���h�E���쐬��OS���̓n���h���ɂ���ăE�B���h�E�̊Ǘ�������
	MSG msg;//OS����󂯎�郁�b�Z�[�W���i�[
	//�E�B���h�E�̏�����
	static char szAppName[] = _T("Chapter3");
	WNDCLASSEX wndclass;



	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;//classstale_�����ɍĕ`��|classstale_�����ɍĕ`��
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
		MessageBox(0, "Direct3D�̍쐬�Ɏ��s���܂���", NULL, MB_OK);
		return E_FAIL;
	}
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	return S_OK;
}

HRESULT InitD3d(HWND hWnd, const TCHAR * filepath)
{
	//Direct3D�̍쐬
	//Direct3Dcreate9��DirectXSDK�ŗp�ӂ���Ă���O���[�o���֐�
	//Direct3Dcreate9��Direct3D�I�u�W�F�N�g���쐬���A�|�C���^�𓾂�
	//Direct3DCreate9��Direct3D�I�u�W�F�N�g�𐶐�
	if (NULL == (pD3d = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		MessageBox(0, "Direct3D�̍쐬�Ɏ��s���܂���", "", MB_OK);
		return E_FAIL;
	}

	//�uDirect3D�f�o�C�X�v�I�u�W�F�N�g�̍쐬
	//�uDirect3D�f�o�C�X�v�I�u�W�F�N�g�̓e�N�X�`���I�u�W�F�N�g�ƃX�v���C�g�I�u�W�F�N�g�ɕK�v

	InitPresentParameter(hWnd);

	if (FAILED(pD3d->CreateDevice
	(
		D3DADAPTER_DEFAULT,//�p�\�R���Ƀr�f�I�J�[�h���������ڂ���Ă��Ȃ�����q���������g��
		D3DDEVTYPE_HAL,//�\���̓n�[�h�E�F�A���\�t�g�E�F�A�̂ǂ��炩�Ŏ������܂����A�n�[�h�E�F�A�Ŏ�������ق������Ȃ荂��
		hWnd,//�A�v���P�[�V�����̃E�B���h�E�n���h��
		D3DCREATE_MIXED_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,//����
		&D3dPresentParameters,//D3DPRESENT_PARAMETERS�ϐ��̃A�h���X
		&pDevice))//CreateDevice���R�[������O�͓��e������̃f�o�C�X�I�u�W�F�N�g�ւ̃|�C���^�ϐ��̃A�h���X
		)
		//CreateDevice���R�[�����Ȃ��Ǝg���Ȃ�
	{
		MessageBox(0, "HAL���[�h��DIRECT3D�f�o�C�X���쐬�ł��܂���\nREF���[�h�ōĎ��s���܂�", NULL, MB_OK);
		if (FAILED(pD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd,
			D3DCREATE_MIXED_VERTEXPROCESSING,
			&D3dPresentParameters, &pDevice)))
		{
			MessageBox(0, "DIRECT3D�f�o�C�X�̍쐬�Ɏ��s���܂���", NULL, MB_OK);
			return E_FAIL;
		}
	}

	//�e�N�X�`���I�u�W�F�N�g�̍쐬
	if (FAILED(D3DXCreateTextureFromFileEx(
		pDevice,			//�f�o�C�X�I�u�W�F�N�g�̃|�C���^
		filepath,			//�r�b�g�}�b�v�t�@�C����
		100,				//�r�b�g�}�b�v�t�@�C���̃T�C�Y�c
		100,				//�r�b�g�}�b�v�t�@�C���̃T�C�Y��
		0,					//�r�b�g77�}�b�v�t�@�C���̃T�C�Y0
		0,					//�r�b�g�}�b�v�t�@�C���̃T�C�Y0
		D3DFMT_UNKNOWN,		//���� 
		D3DPOOL_DEFAULT,	//����
		D3DX_FILTER_NONE,	//����
		D3DX_DEFAULT,		// ����
		0xff000000,			//�����x,�ԁA�΁A�EA�AR�AG�AB
		NULL,				//����
		NULL,				//����
		&pTexture)))		//�e�N�X�`���I�u�W�F�N�g�̃|�C���^�ϐ��̃A�h���X
	{
		MessageBox(0, "�e�N�X�`���̍쐬�Ɏ��s���܂���", "", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

//�`��֐�
VOID Draw()
{
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0x00, 0x00, 0x00), 1.0f, 0);//��ʂ����ꂢ�ɂ���
	//�e�N�X�`���\��t���J�n
	pDevice->BeginScene();
	// �e�N�X�`���̃Z�b�g
	pDevice->SetTexture(0, pTexture);
	// ���_���X�g�̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, CustomVertex, sizeof(CUSTOMVERTEX));
	//�e�N�X�`���\��t���I��
	pDevice->EndScene();//�`��̍Ō�ɃR�[��
	pDevice->Present(NULL, NULL, NULL, NULL);//��������ʂ������Ă���
}

void InitPresentParameter(HWND hWnd) {

	ZeroMemory(&D3dPresentParameters, sizeof(D3dPresentParameters));

	//�\���̂̃����o��������
	//��ʕ\���̓t�����g�o�b�t�@�ƃo�b�N�o�b�t�@��؂�ւ���E�t�����g�o�b�t�@�ƃo�b�N�o�b�t�@�̐؂�ւ����t���b�v
	D3dPresentParameters.BackBufferFormat = D3DFMT_UNKNOWN;//�o�b�N�o�b�t�@�̕\���`���ED3DFMT_UNKNOWN�Ńf�X�N�g�b�v�̕\�����[�h�������ݒ�
	D3dPresentParameters.BackBufferCount = 1;//�o�b�N�o�b�t�@�̐�

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

	D3dPresentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;//�t���b�v�̌`��
	D3dPresentParameters.Windowed = TRUE;//�E�B���h�E���[�h���t���X�N���[�����[�h���Etrue���E�B���h�E���[�h�Efalse���t���X�N���[�����[�h
}