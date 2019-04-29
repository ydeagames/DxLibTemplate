/**
 * Copyright (c) 2018-2019 YdeaGames
 *
 * This file is released under the MIT License.
 * http://opensource.org/licenses/mit-license.php
 */
#pragma once

namespace Screen
{
	void SetTitle(const std::string& newtitle);

	Vector2 GetSize();
	void SetSize(const Vector2& newsize);
	void SetMaxSize(const Vector2 & newsize);
	void SetMinSize(const Vector2 & newsize);
	Vector2 GetDisplaySize();
	void SetScreenSize(const Vector2& newsize);

	void SetLocation(const Vector2& newsize);
	void SetLocationRelativeTo();
}