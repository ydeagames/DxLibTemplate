/**
 * Copyright (c) 2018-2019 YdeaGames
 *
 * This file is released under the MIT License.
 * http://opensource.org/licenses/mit-license.php
 */
#pragma once

// <�V�X�e��>
const char* const GAME_TITLE = "Sample Game";		// �Q�[���^�C�g��

// <���>
const int SCREEN_WIDTH = 640;						// ��ʂ̕�[pixel]
const int SCREEN_HEIGHT = 480;						// ��ʂ̍���[pixel]

const int SCREEN_TOP = 0;							// ��ʂ̏�[
const int SCREEN_BOTTOM = SCREEN_HEIGHT;			// ��ʂ̉��[
const int SCREEN_LEFT = 0;							// ��ʂ̍��[
const int SCREEN_RIGHT = SCREEN_WIDTH;				// ��ʂ̉E�[

const int SCREEN_CENTER_X = SCREEN_WIDTH / 2;		// ��ʂ̒���(X���W)
const int SCREEN_CENTER_Y = SCREEN_HEIGHT / 2;		// ��ʂ̒���(Y���W)

// <�Q�[��>
class Game
{
public:
	Game();
	~Game();

	void Update(void);
	void Render(void);
};
