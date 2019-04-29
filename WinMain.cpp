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

// <ウィンドウハンドル>
static HWND hWnd;

// <カレントディレクトリの修正>
static void UpdateCurrentDir(void)
{
	TCHAR m_Path[MAX_PATH];
	TCHAR m_Path2[MAX_PATH];

	if (GetModuleFileName(NULL, m_Path, MAX_PATH))    //実行ファイルのフルパスを取得
	{   //取得に成功
		TCHAR* ptmp = _tcsrchr(m_Path, _T('\\')); // \の最後の出現位置を取得
		if (ptmp != NULL)
		{   //ファイル名を削除
			*ptmp = _T('\0');

			sprintf_s(m_Path2, MAX_PATH, "%s\\Resources", m_Path);
			if (!PathFileExists(m_Path2) || !PathIsDirectory(m_Path2))
			{
				TCHAR* ptmp = _tcsrchr(m_Path, _T('\\')); // \の最後の出現位置を取得
				if (ptmp != NULL)
				{   //ファイル名を削除
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
	SetConsoleTitleA("デバッグコンソール");
	typedef struct { char* _ptr; int _cnt; char* _base; int _flag; int _file; int _charbuf; int _bufsiz; char* _tmpfname; } FILE_COMPLETE;
	*(FILE_COMPLETE*)stdout = *(FILE_COMPLETE*)_fdopen(_open_osfhandle((intptr_t)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT), "w");
	*(FILE_COMPLETE*)stderr = *(FILE_COMPLETE*)_fdopen(_open_osfhandle((intptr_t)GetStdHandle(STD_ERROR_HANDLE), _O_TEXT), "w");
	*(FILE_COMPLETE*)stdin = *(FILE_COMPLETE*)_fdopen(_open_osfhandle((intptr_t)GetStdHandle(STD_INPUT_HANDLE), _O_TEXT), "r");
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
	setvbuf(stdin, NULL, _IONBF, 0);
}
#endif

// プログラムのエントリーポイント
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// 未使用引数の警告対策
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(nCmdShow);

	UpdateCurrentDir();

#if defined(_DEBUG)
	// メモリーリーク検出機構のセットアップ
	SetUpMemoryLeakDetector();
	// デバッグ用コンソール (printfとかでデバッグしたいときはコメントアウトを外す)
	//CreateConsoleWindow();
#endif

	// デバッグ情報用ログファイルの出力の抑制
	SetOutApplicationLogValidFlag(false);

	// 起動モードの設定
#if defined(_DEBUG)
// ウインドウモードで実行
	ChangeWindowMode(true);
	// ウインドウアイコンの設定
	SetWindowIconID(IDI_ICON1);
#else
// フルスクリーンで実行
	ChangeWindowMode(false);
#endif

	// 画面を可変サイズにする場合はコメントアウトを外す

	// 最大化ボタンが存在するウインドウモードに変更
	//SetWindowStyleMode(8);

	// 画面サイズをデスクトップのサイズと同じにする
	//Screen::SetScreenSize(Screen::GetDisplaySize());

	// 画面サイズ
	Screen::SetSize(Vector2{ SCREEN_WIDTH, SCREEN_HEIGHT });

	// 最小サイズ
	//Screen::SetMinSize(Vector2{ 480, 320 });

	// ウィンドウクラス名
	//SetMainWindowClassName("");

	// タイトル
	Screen::SetTitle(GAME_TITLE);

	// サイズ変更を可能にする
	//SetWindowSizeChangeEnableFlag(true, false);

	// 常に実行
	SetAlwaysRunFlag(true);

	// 多重起動可能
	SetDoubleStartValidFlag(true);

	// DXライブラリの初期化処理
	if (DxLib_Init() == -1)
		return -1;

	// ウィンドウ取得
	hWnd = GetMainWindowHandle();

	// 描画先を裏画面に設定
	SetDrawScreen(DX_SCREEN_BACK);

	// ゲームの処理

	// ゲームの初期化処理
	Game * game = new Game;

	while (!ProcessMessage() && !CheckHitKey(EXIT_KEY))
	{
		// ゲームの更新処理
		game->Update();
		// ゲームの描画処理
		game->Render();

		// 裏画面の内容を表画面に反映
		ScreenFlip();
		// 裏画面の消去
		ClearDrawScreen();
	}

	// ゲームの終了処理
	delete game;

	// DXライブラリの終了処理
	DxLib_End();

	return 0;
}
