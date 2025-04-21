//
//  Input.hpp
//  game_engine
//
//  Created by Ben Huston on 2/14/25.
//

#ifndef Input_hpp
#define Input_hpp

#include <stdio.h>
#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>
#include "SDL.h"
#include "glm/glm.hpp"


enum INPUT_STATE{
    INPUT_STATE_UP,
    INPUT_STATE_JUST_BECAME_DOWN,
    INPUT_STATE_DOWN,
    INPUT_STATE_JUST_BECAME_UP
};


class Input {
public:
    static void Init();
    static void processEvent(const SDL_Event& e);
    static void lateUpdate();
    
    static bool getKey(std::string code);
    static bool getKeyDown(std::string code);
    static bool getKeyUp(std::string code);
    
    static glm::vec2 getMousePosition();
    
    static bool getMouseButton(int button);
    static bool getMouseButtonDown(int button);
    static bool getMouseButtonUp(int button);
    static float getMouseScrollDelta();
    
    static void hideCursor();
    static void showCursor();
    
    static bool getControllerButton(std::string code, int id);
    static bool getControllerButtonDown(std::string code, int id);
    static bool getControllerButtonUp(std::string code, int id);
    
    static bool getLeftJoystickDirection(std::string direction, int id);
    static bool getRightJoystickDirection(std::string direction, int id);
    
    
private:
    static inline std::unordered_map<SDL_Scancode, INPUT_STATE> keyStates;
    static inline std::vector<SDL_Scancode> justBecameDown;
    static inline std::vector<SDL_Scancode> justBecameUp;
    
    static inline glm::vec2 mousePosition;
    static inline std::unordered_map<int, INPUT_STATE> mouseStates;
    static inline std::vector<int> justBecameDownButtons;
    static inline std::vector<int> justBecameUpButtons;
    
    static inline float scrollThisFrame;
    
    static inline std::unordered_map<SDL_JoystickID, std::unordered_map<SDL_GameControllerButton, INPUT_STATE>> controllerStates;
    static inline std::vector<std::pair<SDL_GameControllerButton, SDL_JoystickID>> justBecameDownControllerButtons;
    static inline std::vector<std::pair<SDL_GameControllerButton, SDL_JoystickID>> justBecameUpControllerButtons;
    
    static inline std::unordered_map<SDL_JoystickID, int> LeftJoystickXVal;
    static inline std::unordered_map<SDL_JoystickID, int> LeftJoystickYVal;
    static inline std::unordered_map<SDL_JoystickID, int> RightJoystickXVal;
    static inline std::unordered_map<SDL_JoystickID, int> RightJoystickYVal;
    static inline std::unordered_map<SDL_JoystickID, int> L2Val;
    static inline std::unordered_map<SDL_JoystickID, int> R2Val;
    
    static inline std::vector<SDL_GameController*> controllers;
    
    
    static inline std::unordered_map<std::string, SDL_Scancode> __keycode_to_scancode = {
        // Directional (arrow) Keys
        {"up", SDL_SCANCODE_UP},
        {"down", SDL_SCANCODE_DOWN},
        {"right", SDL_SCANCODE_RIGHT},
        {"left", SDL_SCANCODE_LEFT},

        // Misc Keys
        {"escape", SDL_SCANCODE_ESCAPE},

        // Modifier Keys
        {"lshift", SDL_SCANCODE_LSHIFT},
        {"rshift", SDL_SCANCODE_RSHIFT},
        {"lctrl", SDL_SCANCODE_LCTRL},
        {"rctrl", SDL_SCANCODE_RCTRL},
        {"lalt", SDL_SCANCODE_LALT},
        {"ralt", SDL_SCANCODE_RALT},

        // Editing Keys
        {"tab", SDL_SCANCODE_TAB},
        {"return", SDL_SCANCODE_RETURN},
        {"enter", SDL_SCANCODE_RETURN},
        {"backspace", SDL_SCANCODE_BACKSPACE},
        {"delete", SDL_SCANCODE_DELETE},
        {"insert", SDL_SCANCODE_INSERT},

        // Character Keys
        {"space", SDL_SCANCODE_SPACE},
        {"a", SDL_SCANCODE_A},
        {"b", SDL_SCANCODE_B},
        {"c", SDL_SCANCODE_C},
        {"d", SDL_SCANCODE_D},
        {"e", SDL_SCANCODE_E},
        {"f", SDL_SCANCODE_F},
        {"g", SDL_SCANCODE_G},
        {"h", SDL_SCANCODE_H},
        {"i", SDL_SCANCODE_I},
        {"j", SDL_SCANCODE_J},
        {"k", SDL_SCANCODE_K},
        {"l", SDL_SCANCODE_L},
        {"m", SDL_SCANCODE_M},
        {"n", SDL_SCANCODE_N},
        {"o", SDL_SCANCODE_O},
        {"p", SDL_SCANCODE_P},
        {"q", SDL_SCANCODE_Q},
        {"r", SDL_SCANCODE_R},
        {"s", SDL_SCANCODE_S},
        {"t", SDL_SCANCODE_T},
        {"u", SDL_SCANCODE_U},
        {"v", SDL_SCANCODE_V},
        {"w", SDL_SCANCODE_W},
        {"x", SDL_SCANCODE_X},
        {"y", SDL_SCANCODE_Y},
        {"z", SDL_SCANCODE_Z},
        {"0", SDL_SCANCODE_0},
        {"1", SDL_SCANCODE_1},
        {"2", SDL_SCANCODE_2},
        {"3", SDL_SCANCODE_3},
        {"4", SDL_SCANCODE_4},
        {"5", SDL_SCANCODE_5},
        {"6", SDL_SCANCODE_6},
        {"7", SDL_SCANCODE_7},
        {"8", SDL_SCANCODE_8},
        {"9", SDL_SCANCODE_9},
        {"/", SDL_SCANCODE_SLASH},
        {";", SDL_SCANCODE_SEMICOLON},
        {"=", SDL_SCANCODE_EQUALS},
        {"-", SDL_SCANCODE_MINUS},
        {".", SDL_SCANCODE_PERIOD},
        {",", SDL_SCANCODE_COMMA},
        {"[", SDL_SCANCODE_LEFTBRACKET},
        {"]", SDL_SCANCODE_RIGHTBRACKET},
        {"\\", SDL_SCANCODE_BACKSLASH},
        {"'", SDL_SCANCODE_APOSTROPHE}
    };
    
    
    
    static inline std::unordered_map<std::string, SDL_GameControllerButton> controllerToKeycode = {
        {"A", SDL_CONTROLLER_BUTTON_A},            // Typically Cross (✕) on PS5
        {"B", SDL_CONTROLLER_BUTTON_B},            // Circle (◯)
        {"X", SDL_CONTROLLER_BUTTON_X},            // Square (□)
        {"Y", SDL_CONTROLLER_BUTTON_Y},            // Triangle (△)
        {"Share", SDL_CONTROLLER_BUTTON_BACK},         // Share button
        {"PS", SDL_CONTROLLER_BUTTON_GUIDE},        // PS button
        {"Options", SDL_CONTROLLER_BUTTON_START},        // Options button
        {"Left_Stick", SDL_CONTROLLER_BUTTON_LEFTSTICK},    // Pressing left stick
        {"Right_Stick", SDL_CONTROLLER_BUTTON_RIGHTSTICK},   // Pressing right stick
        {"L1", SDL_CONTROLLER_BUTTON_LEFTSHOULDER}, // L1
        {"R1", SDL_CONTROLLER_BUTTON_RIGHTSHOULDER},// R1
        {"DPAD_UP", SDL_CONTROLLER_BUTTON_DPAD_UP},
        {"DPAD_DOWN", SDL_CONTROLLER_BUTTON_DPAD_DOWN},
        {"DPAD_LEFT", SDL_CONTROLLER_BUTTON_DPAD_LEFT},
        {"DPAD_RIGHT", SDL_CONTROLLER_BUTTON_DPAD_RIGHT},
        {"Touchpad", SDL_CONTROLLER_BUTTON_MISC1}
    };
    
};

#endif /* Input_hpp */
