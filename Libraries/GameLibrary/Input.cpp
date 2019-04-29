/**
 * Copyright (c) 2018-2019 YdeaGames
 *
 * This file is released under the MIT License.
 * http://opensource.org/licenses/mit-license.php
 */
#include "Input.h"

InputButton::InputButton(std::shared_ptr<IInput> input, int button) :
	input(input),
	button(button)
{
}

void InputButton::Consume() const
{
	input->Consume(button);
}

bool InputButton::GetButton() const
{
	return input->GetButton(button);
}

bool InputButton::GetButtonDown() const
{
	return input->GetButtonDown(button);
}

bool InputButton::GetButtonUp() const
{
	return input->GetButtonUp(button);
}

IInput::IInput()
{
}

IInput::~IInput()
{
}

// ボタンハンドラ
InputButton IInput::GetInputButton(int button)
{
	return{ shared_from_this(), button };
}

ButtonInput::ButtonInput() :
	input_state(0),
	input_state_last(0)
{
}

ButtonInput::~ButtonInput()
{
}

void ButtonInput::Consume(int button)
{
	input_state_last ^= (input_state_last ^ input_state) & button;
}

bool ButtonInput::GetButton(int button)
{
	return (input_state & button) != 0;
}

bool ButtonInput::GetButtonDown(int button)
{
	return !(input_state_last & button) && (input_state & button);
}

bool ButtonInput::GetButtonUp(int button)
{
	return (input_state_last & button) && !(input_state & button);
}

JoypadInput::JoypadInput(int pad_id) :
	pad_id(pad_id)
{
}

JoypadInput::~JoypadInput()
{
}

void JoypadInput::Update()
{
	input_state_last = input_state;
	input_state = GetJoypadInputState(pad_id);
}

MouseInput::MouseInput()
	: wheel_pos(0)
{
}

MouseInput::~MouseInput()
{
}

void MouseInput::Update()
{
	input_state_last = input_state;
	input_state = GetMouseInput();

	int mouse_x, mouse_y;
	GetMousePoint(&mouse_x, &mouse_y);
	position = { static_cast<float>(mouse_x), static_cast<float>(mouse_y) };

	wheel_pos += wheel_delta = GetMouseWheelRotVolF();
}

const Vector2& MouseInput::GetPosition()
{
	return position;
}

void MouseInput::SetPosition(const Vector2& pos)
{
	SetMousePoint(static_cast<int>(pos.x), static_cast<int>(pos.y));
}

float MouseInput::GetWheel()
{
	return wheel_pos;
}

float MouseInput::GetDeltaWheel()
{
	return wheel_delta;
}

KeyInput::KeyInput()
{
	memset(input_state, 0, sizeof(input_state));
	memset(input_state_last, 0, sizeof(input_state_last));
}

KeyInput::~KeyInput()
{
}

void KeyInput::Update()
{
	memcpy(input_state_last, input_state, sizeof(input_state));
	GetHitKeyStateAll(input_state);
}

void KeyInput::Consume(int button)
{
	input_state_last[button] = input_state[button];
}

bool KeyInput::GetButton(int button)
{
	return input_state[button] != 0;
}

bool KeyInput::GetButtonDown(int button)
{
	return !input_state_last[button] && input_state[button];
}

bool KeyInput::GetButtonUp(int button)
{
	return input_state_last[button] && !input_state[button];
}

InputManager::InputManager()
{
	joypad = Register<JoypadInput>("Joypad", std::make_shared<JoypadInput>());
	mouse = Register<MouseInput>("Mouse", std::make_shared<MouseInput>());
	key = Register<KeyInput>("Key", std::make_shared<KeyInput>());
}

void InputManager::Update()
{
	for (auto itr = inputsname.begin(); itr != inputsname.end(); ++itr)
		itr->second->Update();
}

bool Input::GetButton(int button)
{
	return InputManager::GetInstance().joypad->GetButton(button);
}

bool Input::GetButtonDown(int button)
{
	return InputManager::GetInstance().joypad->GetButtonDown(button);
}

bool Input::GetButtonUp(int button)
{
	return InputManager::GetInstance().joypad->GetButtonUp(button);
}

bool Input::GetKey(int button)
{
	return InputManager::GetInstance().key->GetButton(button);
}

bool Input::GetKeyDown(int button)
{
	return InputManager::GetInstance().key->GetButtonDown(button);
}

bool Input::GetKeyUp(int button)
{
	return InputManager::GetInstance().key->GetButtonUp(button);
}

bool Input::GetMouse(int button)
{
	return InputManager::GetInstance().mouse->GetButton(button);
}

bool Input::GetMouseDown(int button)
{
	return InputManager::GetInstance().mouse->GetButtonDown(button);
}

bool Input::GetMouseUp(int button)
{
	return InputManager::GetInstance().mouse->GetButtonUp(button);
}

const Vector2& Input::GetPosition()
{
	return InputManager::GetInstance().mouse->GetPosition();
}

void Input::SetPosition(const Vector2& pos)
{
	InputManager::GetInstance().mouse->SetPosition(pos);
}

float Input::GetWheel()
{
	return InputManager::GetInstance().mouse->GetWheel();
}

float Input::GetDeltaWheel()
{
	return InputManager::GetInstance().mouse->GetDeltaWheel();
}

