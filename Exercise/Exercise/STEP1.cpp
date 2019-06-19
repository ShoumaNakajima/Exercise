
#pragma comment (lib,"winmm.lib")


#include<windows.h>
#include<mmsystem.h>

LRESULT CALLBACK WndProc
(
	HWND,
	UINT,
	WPARAM,
	LPARAM
);//�E�B���h�E�v���V�[�W���[�֐��v���g�^�C�v�錾
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
	);//�E�B���h�E�n���h���̒l���i�[�E�Œ�1�̃E�B���h�E���쐬��OS���̓n���h���ɂ���ăE�B���h�E�̊Ǘ�������
	
	MSG msg;//OS����󂯎�郁�b�Z�[�W���i�[

	void message();
		
	//	�V�X�e�����Ԃ��擾
	DWORD SyncPrev = timeGetTime();
	DWORD SyncCurr;

	ZeroMemory
	(
		&msg,
		sizeof(msg)
	);//msg�\���̂�������
	timeBeginPeriod(1);
	while (msg.message != WM_QUIT)//msg�\���̂�message��WM_QUIT�łȂ����胋�[�v
	{
		Sleep(1);
		if (PeekMessage
		(
			&msg,
			NULL,
			0U,
			0U,
			PM_REMOVE
		))//OS����̃��b�Z�[�W�𒲂ׂ�//���b�Z�[�W�L���[�Ƀ��b�Z�[�W�����邩���ׁA������MSG�\���̎����b�Z�[�W���i�[
		{//����ꍇ
		//�E�B���h�E�v���V�[�W���֐��ɓn���E�]��
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
		{//�Ȃ��ꍇ����ʓI�ɃA�v���P�[�V�����̏���������

		}
	}
	timeEndPeriod(1);
	return (INT)msg.wParam;//msg�̍\���̂Ɋi�[�Emsg�@�����b�Z�[�W
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
	HWND hWnd = NULL;//�E�B���h�E�n���h���̒l���i�[�E�Œ�1�̃E�B���h�E���쐬��OS���̓n���h���ɂ���ăE�B���h�E�̊Ǘ�������
	MSG msg;//OS����󂯎�郁�b�Z�[�W���i�[
	//�E�B���h�E�̏�����
	static char szAppName[] = "Chapter2";
	WNDCLASSEX wndclass;

	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;//classstale_�����ɍĕ`��|classstale_�����ɍĕ`��
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