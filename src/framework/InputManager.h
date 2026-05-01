#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "Engine.h"
#include "../events/Event.h"
#include "./CVariableSystem.h"
#include "String.h"

const int MAX_EVENT_COUNT = 64;

namespace Arboria {

#if SDL_VERSION_ATLEAST(3,0,0)
#define IS_SDL_BUTTON_DOWN(EV) EV.down
#else
#define IS_SDL_BUTTON_DOWN(EV) EV.state == SDL_PRESSED;
#endif

	class Lexer;

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
		ArboriaKey_GamepadLeftTrigger_1,
		ArboriaKey_GamepadRightTrigger_1,
		ArboriaKey_GamepadLeftTrigger_2,
		ArboriaKey_GamepadRightTrigger_2,
		ArboriaKey_GamepadLeftTrigger_3,
		ArboriaKey_GamepadRightTrigger_3,
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

	enum ArboriaJoystickAxis {
		AXIS_LEFT_X,
		AXIS_LEFT_Y,
		AXIS_RIGHT_X,
		AXIS_RIGHT_Y,
		AXIS_LEFT_TRIGGER,
		AXIS_RIGHT_TRIGGER,
		MAX_TRIGGER_AXIS,
	};

	enum InputConfigFlags {
		InputConfigFlags_None = 0,
		InputConfigFlags_EnableKeyboard = 1,
		InputConfigFlags_EnableGamepad = 1 << 1,
		InputConfigFlags_EnableMouse = 1 << 2,
	};

	enum ActionType : int {
		ACTION_NONE = -1,
		ACTION_CONFIRM = 0,
		ACTION_CANCEL = 1,
		ACTION_TOGGLE_DISPLAY = 2,
		ACTION_HELP = 3,
		ACTION_LEFTARROW = 4,
		ACTION_RIGHTARROW = 5,
		ACTION_UPARROW = 6,
		ACTION_DOWNARROW = 7,
		ACTION_L_TRIGGER_1 = 8,
		ACTION_R_TRIGGER_1 = 9,
		ACTION_L_TRIGGER_2 = 10,
		ACTION_R_TRIGGER_2 = 11,
		ACTION_L_TRIGGER_3 = 12,
		ACTION_R_TRIGGER_3 = 13,
		ACTION_ENTER = 14,
		ACTION_SELECT = 15,
	};

	struct KeyState {
		bool down; //whether key/button is pressed down
		int binding; //the action enum that this key is bound to, default value is -1
		float downDuration; //duration the key/button has been pressed down (if < 0.0: not pressed, if == 0.0: just pressed, if > 0.0f: held)
		float lastDownDuration; //the duration the last time the button was pressed down.
		float analog; //range from 0.0f to 1.0f for analog sticks/gamepad values.

		KeyState() : down(false), binding(ACTION_NONE), downDuration(-1.0f), lastDownDuration(-1.0f), analog(0.0f) {}
		KeyState(int _binding) : KeyState() { binding = _binding; }
	};

	enum InputDeviceType : int {
		KEYBOARD,
		CONTROLLER,
		MOUSE
	};

	extern CVariableFloat joyAxis_deadzone;
	static float joyAxis[MAX_TRIGGER_AXIS];

	struct ActBinding {
		int key;
		int button;
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
			int translateGamepadCode(SDL_GameControllerButton button);
			int translateGamepadAxis(SDL_GameControllerAxis axis); //includes L2/R2 for some reason
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
			int getBinding(int key) const { return keyStates[key].binding; }
			void unbindKeyAction(int keyNum, int action);
			void unbindKeyAction(int action);
			void unbindGamepadAction(int buttonNum, int action);
			void unbindGamepadAction(int action);
			void bindAction(int keynum, int action);
			int getKeyFromAction(int action);
			int getGamepadButtonFromAction(int action);
			KeyState* getKeyInputFromAction(int action);
			KeyState* getButtonInputFromAction(int action);
			void pushAxisButton(int key, bool value); //translate L2/R2 and controller axis into controller input

			float getDeltaTime() { return deltaTime; }
			void setDeltaTime(float v) { deltaTime = v; }

			void restoreDefaults();
			void saveUserBindings();
			bool loadUserBindings();
			bool parseActionBindings(Lexer& src, int& act, int& key, int& button);
			String& convertToString(const int& act, const int& key, const int& button);
			//void updateKeyInputState(AEvent* _keyEv);
	};
}

#endif