#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "Engine.h"
#include "../events/Event.h"

const int MAX_EVENT_COUNT = 64;

namespace Arboria {

	enum ArboriaKey : int {
		ArboriaKey_Unknown = 0,
		ArboriaKey_A,
		ArboriaKey_B,
		ArboriaKey_C,
		ArboriaKey_D,
		ArboriaKey_E,
		ArboriaKey_F,
		ArboriaKey_G,
		ArboriaKey_H,
		ArboriaKey_I,
		ArboriaKey_J,
		ArboriaKey_K,
		ArboriaKey_L,
		ArboriaKey_M,
		ArboriaKey_N,
		ArboriaKey_O,
		ArboriaKey_P,
		ArboriaKey_Q,
		ArboriaKey_R,
		ArboriaKey_S,
		ArboriaKey_T,
		ArboriaKey_U,
		ArboriaKey_V,
		ArboriaKey_W,
		ArboriaKey_X,
		ArboriaKey_Y,
		ArboriaKey_Z,
		ArboriaKey_0,
		ArboriaKey_1,
		ArboriaKey_2,
		ArboriaKey_3,
		ArboriaKey_4,
		ArboriaKey_5,
		ArboriaKey_6,
		ArboriaKey_7,
		ArboriaKey_8,
		ArboriaKey_9,
		ArboriaKey_Return,
		ArboriaKey_Escape,
		ArboriaKey_Backspace,
		ArboriaKey_Tab,
		ArboriaKey_Space,
		ArboriaKey_Minus,
		ArboriaKey_Equals,
		ArboriaKey_LeftBracket,
		ArboriaKey_RightBracket,
		ArboriaKey_Backslash,
		ArboriaKey_Semicolon,
		ArboriaKey_Apostrophe,
		ArboriaKey_Grave,
		ArboriaKey_Comma,
		ArboriaKey_Period,
		ArboriaKey_Slash,
		ArboriaKey_CapsLock,
		ArboriaKey_F1,
		ArboriaKey_F2,
		ArboriaKey_F3,
		ArboriaKey_F4,
		ArboriaKey_F5,
		ArboriaKey_F6,
		ArboriaKey_F7,
		ArboriaKey_F8,
		ArboriaKey_F9,
		ArboriaKey_F10,
		ArboriaKey_F11,
		ArboriaKey_F12,
		ArboriaKey_F13,
		ArboriaKey_F14,
		ArboriaKey_F15,
		ArboriaKey_F16,
		ArboriaKey_F17,
		ArboriaKey_F18,
		ArboriaKey_F19,
		ArboriaKey_F20,
		ArboriaKey_F21,
		ArboriaKey_F22,
		ArboriaKey_F23,
		ArboriaKey_F24,
		ArboriaKey_PrintScreen,
		ArboriaKey_ScrollLock,
		ArboriaKey_Pause,
		ArboriaKey_Insert,
		ArboriaKey_Home,
		ArboriaKey_PageUp,
		ArboriaKey_PageDown,
		ArboriaKey_Delete,
		ArboriaKey_End,
		ArboriaKey_LeftArrow,
		ArboriaKey_RightArrow,
		ArboriaKey_UpArrow,
		ArboriaKey_DownArrow,
		ArboriaKey_NumLock,
		
		ArboriaKey_KP_Divide,
		ArboriaKey_KP_Multiply,
		ArboriaKey_KP_Minus,
		ArboriaKey_KP_Plus,
		ArboriaKey_KP_Enter,
		ArboriaKey_KP_0,
		ArboriaKey_KP_1,
		ArboriaKey_KP_2,
		ArboriaKey_KP_3,
		ArboriaKey_KP_4,
		ArboriaKey_KP_5,
		ArboriaKey_KP_6,
		ArboriaKey_KP_7,
		ArboriaKey_KP_8,
		ArboriaKey_KP_9,
		ArboriaKey_KP_Period,
		ArboriaKey_KP_Equal,
		ArboriaKey_Menu,
		ArboriaKey_LCtrl,
		ArboriaKey_LShift,
		ArboriaKey_LAlt,
		ArboriaKey_LGui,
		ArboriaKey_RCtrl,
		ArboriaKey_RShift,
		ArboriaKey_RAlt,
		ArboriaKey_RGui,

		//Gamepads
		ArboriaKey_GamepadStart,
		ArboriaKey_GamepadBack,
		ArboriaKey_GamepadFaceLeft,
		ArboriaKey_GamepadFaceRight,
		ArboriaKey_GamepadFaceUp,
		ArboriaKey_GamepadFaceDown,
		ArboriaKey_GamepadDpadLeft,
		ArboriaKey_GamepadDpadRight,
		ArboriaKey_GamepadDpadUp,
		ArboriaKey_GamepadDpadDown,
		ArboriaKey_GamepadDpadL1,
		ArboriaKey_GamepadDpadR1,
		ArboriaKey_GamepadDpadL2,
		ArboriaKey_GamepadDpadR2,
		ArboriaKey_GamepadDpadL3,
		ArboriaKey_GamepadDpadR3,
		ArboriaKey_GamepadLStickLeft,
		ArboriaKey_GamepadLStickRight,
		ArboriaKey_GamepadLStickUp,
		ArboriaKey_GamepadLStickDown,
		ArboriaKey_GamepadRStickLeft,
		ArboriaKey_GamepadRStickRight,
		ArboriaKey_GamepadRStickUp,
		ArboriaKey_GamepadRStickDown,

		//Mouse Inputs
		ArboriaKey_MouseLeft,
		ArboriaKey_MouseRight,
		ArboriaKey_MouseMiddle,
		ArboriaKey_MouseX1,
		ArboriaKey_MouseX2,
		ArboriaKey_MouseWheelX,
		ArboriaKey_MouseWheelY,

		ArboriaKey_ModCtrl,
		ArboriaKey_ModShift,
		ArboriaKey_ModAlt,
		ArboriaKey_ModGui,

		ArboriaKey_Count,

		ArboriaMod_None = 0,
		ArboriaMod_Shortcut = 1 << 11, //alias for Ctrl (non-MacOS) or Super (macOS)
		ArboriaMod_Ctrl = 1 << 12,
		ArboriaMod_Shift = 1 << 13,
		ArboriaMod_Alt = 1 << 14,
		ArboriaMod_Gui = 1 << 15,
		ArboriaMod_Mask = 0xF800
	};

	enum InputConfigFlags {
		InputConfigFlags_None = 0,
		InputConfigFlags_EnableKeyboard = 1,
		InputConfigFlags_EnableGamepad = 1 << 1,
		InputConfigFlags_EnableMouse = 1 << 2,
	};

	struct KeyState {
		bool down; //whether key/button is pressed down
		float downDuration; //duration the key/button has been pressed down (if < 0.0: not pressed, if == 0.0: just pressed, if > 0.0f: held)
		float lastDownDuration; //the duration the last time the button was pressed down.
		float analog; //range from 0.0f to 1.0f for analog sticks/gamepad values.

		KeyState() : down(false), downDuration(-1.0f), lastDownDuration(-1.0f), analog(0.0f) {}
	};

	class InputManager {
		private:
			//InputConfiguration* inputConfiguration;
			AEvent queue[64];
			unsigned int head;
			unsigned int tail;
		protected:
			unsigned int configFlags;
			KeyState keyStates[ArboriaKey_Count];
			float deltaTime;
			int translateKeyCode(SDL_Scancode keycode); //use with keyboard only?
		public:
			explicit InputManager();
			~InputManager();
			void translateSdlEvents();
			void submitEvent(const AEvent* ev);
			const AEvent& getFront() const;
			AEvent& getFront();
			void popFront();
			bool isQueueEmpty() const { return head == tail; }
			void clearQueue();
			bool isGamepadKey(int scancode);
			int getRepeatCount(int key, float repeat_delay, float repeat_rate);
			KeyState* getKeyState(int key) { return &keyStates[key]; }
			void updateKeyState(int keyCode, bool isDown);

			float getDeltaTime() { return deltaTime; }
			void setDeltaTime(float v) { deltaTime = v; }
			//void updateKeyInputState(AEvent* _keyEv);
	};
}

#endif