/**
 * Copyright (c) 2018-2019 YdeaGames
 *
 * This file is released under the MIT License.
 * http://opensource.org/licenses/mit-license.php
 */
#include "Game\GameMain.h"
#include "Resources\Icon\Icon.h"

#if defined(_DEBUG)
#include <io.h>
#include <Fcntl.h>
#endif

// <�E�B���h�E�n���h��>
static HWND hWnd;

// <�J�����g�f�B���N�g���̏C��>
static void UpdateCurrentDir(void)
{
	TCHAR m_Path[MAX_PATH];
	TCHAR m_Path2[MAX_PATH];

	if (GetModuleFileName(NULL, m_Path, MAX_PATH))    //���s�t�@�C���̃t���p�X���擾
	{   //�擾�ɐ���
		TCHAR* ptmp = _tcsrchr(m_Path, _T('\\')); // \�̍Ō�̏o���ʒu���擾
		if (ptmp != NULL)
		{   //�t�@�C�������폜
			*ptmp = _T('\0');

			sprintf_s(m_Path2, MAX_PATH, "%s\\Resources", m_Path);
			if (!PathFileExists(m_Path2) || !PathIsDirectory(m_Path2))
			{
				TCHAR* ptmp = _tcsrchr(m_Path, _T('\\')); // \�̍Ō�̏o���ʒu���擾
				if (ptmp != NULL)
				{   //�t�@�C�������폜
					*ptmp = _T('\0');
				}
			}
			sprintf_s(m_Path2, MAX_PATH, "%s\\Resources", m_Path);
			if (PathFileExists(m_Path2) && PathIsDirectory(m_Path2))
				SetCurrentDirectory(m_Path);
		}
	}
}

#if defined(_DEBUG)
static void CreateConsoleWindow() {
	AllocConsole();
	SetConsoleTitleA("�f�o�b�O�R���\�[��");
	typedef struct { char* _ptr; int _cnt; char* _base; int _flag; int _file; int _charbuf; int _bufsiz; char* _tmpfname; } FILE_COMPLETE;
	*(FILE_COMPLETE*)stdout = *(FILE_COMPLETE*)_fdopen(_open_osfhandle((intptr_t)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT), "w");
	*(FILE_COMPLETE*)stderr = *(FILE_COMPLETE*)_fdopen(_open_osfhandle((intptr_t)GetStdHandle(STD_ERROR_HANDLE), _O_TEXT), "w");
	*(FILE_COMPLETE*)stdin = *(FILE_COMPLETE*)_fdopen(_open_osfhandle((intptr_t)GetStdHandle(STD_INPUT_HANDLE), _O_TEXT), "r");
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
	setvbuf(stdin, NULL, _IONBF, 0);
}
#endif

// �v���O�����̃G���g���[�|�C���g
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// ���g�p�����̌x���΍�
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(nCmdShow);

	UpdateCurrentDir();

#if defined(_DEBUG)
	// �������[���[�N���o�@�\�̃Z�b�g�A�b�v
	SetUpMemoryLeakDetector();
	// �f�o�b�O�p�R���\�[�� (printf�Ƃ��Ńf�o�b�O�������Ƃ��̓R�����g�A�E�g���O��)
	//CreateConsoleWindow();
#endif

	// �f�o�b�O���p���O�t�@�C���̏o�̗͂}��
	SetOutApplicationLogValidFlag(false);

	// �N�����[�h�̐ݒ�
#if defined(_DEBUG)
// �E�C���h�E���[�h�Ŏ��s
	ChangeWindowMode(true);
	// �E�C���h�E�A�C�R���̐ݒ�
	SetWindowIconID(IDI_ICON1);
#else
// �t���X�N���[���Ŏ��s
	ChangeWindowMode(false);
#endif

	// ��ʂ��σT�C�Y�ɂ���ꍇ�̓R�����g�A�E�g���O��

	// �ő剻�{�^�������݂���E�C���h�E���[�h�ɕύX
	//SetWindowStyleMode(8);

	// ��ʃT�C�Y���f�X�N�g�b�v�̃T�C�Y�Ɠ����ɂ���
	//Screen::SetScreenSize(Screen::GetDisplaySize());

	// ��ʃT�C�Y
	Screen::SetSize(Vector2{ SCREEN_WIDTH, SCREEN_HEIGHT });

	// �ŏ��T�C�Y
	//Screen::SetMinSize(Vector2{ 480, 320 });

	// �E�B���h�E�N���X��
	//SetMainWindowClassName("");

	// �^�C�g��
	Screen::SetTitle(GAME_TITLE);

	// �T�C�Y�ύX���\�ɂ���
	//SetWindowSizeChangeEnableFlag(true, false);

	// ��Ɏ��s
	SetAlwaysRunFlag(true);

	// ���d�N���\
	SetDoubleStartValidFlag(true);

	// DX���C�u�����̏���������
	if (DxLib_Init() == -1)
		return -1;

	// �E�B���h�E�擾
	hWnd = GetMainWindowHandle();

	// �`���𗠉�ʂɐݒ�
	SetDrawScreen(DX_SCREEN_BACK);

	// �Q�[���̏���

	// �Q�[���̏���������
	Game * game = new Game;

	while (!ProcessMessage() && !CheckHitKey(EXIT_KEY))
	{
		// �Q�[���̍X�V����
		game->Update();
		// �Q�[���̕`�揈��
		game->Render();

		// ����ʂ̓��e��\��ʂɔ��f
		ScreenFlip();
		// ����ʂ̏���
		ClearDrawScreen();
	}

	// �Q�[���̏I������
	delete game;

	// DX���C�u�����̏I������
	DxLib_End();

	return 0;
}
