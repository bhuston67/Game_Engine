//
//  Input.cpp
//  game_engine
//
//  Created by Ben Huston on 2/14/25.
//

#include "Input.hpp"


void Input::Init() {
    int numJoysticks = SDL_NumJoysticks();
    for (int i = 0; i < numJoysticks; ++i) {
        if (SDL_IsGameController(i)) {
            SDL_GameController* controller = SDL_GameControllerOpen(i);
            if (controller) {
                controllers.push_back(controller);
                printf("Opened controller %d: %s\n", i, SDL_GameControllerName(controller));
            } else {
                printf("Could not open gamecontroller %d: %s\n", i, SDL_GetError());
            }
        }
    }
    for (int code = SDL_SCANCODE_UNKNOWN; code < SDL_NUM_SCANCODES;code++) {
        keyStates[static_cast<SDL_Scancode>(code)] = INPUT_STATE_UP;
    }
    for (int code = 1; code <=3; code++) {
        mouseStates[code] = INPUT_STATE_UP;
    }
    for (int i = 0; i < numJoysticks; i++) {
        for (int code = SDL_CONTROLLER_BUTTON_A; code <= SDL_CONTROLLER_BUTTON_MISC1; code++) {
            controllerStates[i][static_cast<SDL_GameControllerButton>(code)] = INPUT_STATE_UP;
        }
        LeftJoystickXVal[i] = 0;
        RightJoystickXVal[i] = 0;
        LeftJoystickYVal[i] = 0;
        RightJoystickYVal[i] = 0;
        R2Val[i] = 0;
        L2Val[i] = 0;
    }
}

void Input::processEvent(const SDL_Event &e) {
    if (e.type == SDL_KEYDOWN) {
        keyStates[e.key.keysym.scancode] = INPUT_STATE_JUST_BECAME_DOWN;
        justBecameDown.push_back(e.key.keysym.scancode);
        return;
    }
    if (e.type == SDL_KEYUP) {
        keyStates[e.key.keysym.scancode] = INPUT_STATE_JUST_BECAME_UP;
        justBecameUp.push_back(e.key.keysym.scancode);
        return;
    }
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        mouseStates[e.button.button] = INPUT_STATE_JUST_BECAME_DOWN;
        justBecameDownButtons.push_back(e.button.button);
        return;
    }
    if (e.type == SDL_MOUSEBUTTONUP) {
        mouseStates[e.button.button] = INPUT_STATE_JUST_BECAME_UP;
        justBecameUpButtons.push_back(e.button.button);
        return;
    }
    if (e.type == SDL_MOUSEWHEEL) {
        scrollThisFrame = e.wheel.preciseY;
        return;
    }
    if (e.type == SDL_MOUSEMOTION) {
        mousePosition[0] = e.motion.x;
        mousePosition[1] = e.motion.y;
        return;
    }
    if (e.type == SDL_CONTROLLERBUTTONDOWN) {
        controllerStates[e.cbutton.which][static_cast<SDL_GameControllerButton>(e.cbutton.button)] = INPUT_STATE_JUST_BECAME_DOWN;
        justBecameDownControllerButtons.push_back(std::pair(static_cast<SDL_GameControllerButton>(e.cbutton.button),e.cbutton.which));
        return;
    }
    if (e.type == SDL_CONTROLLERBUTTONUP) {
        controllerStates[e.cbutton.which][static_cast<SDL_GameControllerButton>(e.cbutton.button)] = INPUT_STATE_JUST_BECAME_UP;
        justBecameUpControllerButtons.push_back(std::pair(static_cast<SDL_GameControllerButton>(e.cbutton.button), e.cbutton.which));
        return;
    }
    if (e.type == SDL_CONTROLLERAXISMOTION) {
        if (e.caxis.axis == SDL_CONTROLLER_AXIS_LEFTX) {
            LeftJoystickXVal[e.cbutton.which] = e.caxis.value;
        }
        if (e.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY) {
            LeftJoystickYVal[e.cbutton.which] = e.caxis.value;
        }
        if (e.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTX) {
            RightJoystickXVal[e.cbutton.which] = e.caxis.value;
        }
        if (e.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTY) {
            RightJoystickYVal[e.cbutton.which] = e.caxis.value;
        }
        if (e.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERLEFT) {
            L2Val[e.cbutton.which] = e.caxis.value;
        }
        if (e.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERRIGHT) {
            R2Val[e.cbutton.which] = e.caxis.value;
        }
    }
}

void Input::lateUpdate() {
    for (const SDL_Scancode code : justBecameDown) {
        keyStates[code] = INPUT_STATE_DOWN;
    }
    justBecameDown.clear();
    for (const SDL_Scancode code : justBecameUp) {
        keyStates[code] = INPUT_STATE_UP;
    }
    justBecameUp.clear();
    for (int code : justBecameUpButtons) {
        mouseStates[code] = INPUT_STATE_UP;
    }
    justBecameUpButtons.clear();
    for (int code : justBecameDownButtons) {
        mouseStates[code] = INPUT_STATE_DOWN;
    }
    justBecameDownButtons.clear();
    scrollThisFrame = 0.0f;
    for (auto code : justBecameDownControllerButtons) {
        controllerStates[code.second][code.first] = INPUT_STATE_DOWN;
    }
    justBecameDownControllerButtons.clear();
    for (auto code : justBecameUpControllerButtons) {
        controllerStates[code.second][code.first] = INPUT_STATE_UP;
    }
    justBecameUpControllerButtons.clear();
}

bool Input::getKey(std::string code) {
    if (__keycode_to_scancode.find(code) == __keycode_to_scancode.end()) {
        return false;
    }
    SDL_Scancode keycode = __keycode_to_scancode[code];
    return (keyStates[keycode] == INPUT_STATE_DOWN || keyStates[keycode] == INPUT_STATE_JUST_BECAME_DOWN);
}

bool Input::getKeyUp(std::string code) {
    if (__keycode_to_scancode.find(code) == __keycode_to_scancode.end()) {
        return false;
    }
    SDL_Scancode keycode = __keycode_to_scancode[code];
    return keyStates[keycode] == INPUT_STATE_JUST_BECAME_UP;
}

bool Input::getKeyDown(std::string code) {
    if (__keycode_to_scancode.find(code) == __keycode_to_scancode.end()) {
        return false;
    }
    SDL_Scancode keycode = __keycode_to_scancode[code];
    return keyStates[keycode] == INPUT_STATE_JUST_BECAME_DOWN;
}

bool Input::getMouseButton(int button) {
    if (mouseStates.find(button) == mouseStates.end()) {
        return false;
    }
    return (mouseStates[button] == INPUT_STATE_DOWN || mouseStates[button] == INPUT_STATE_JUST_BECAME_DOWN);
}

bool Input::getMouseButtonUp(int button) {
    if (mouseStates.find(button) == mouseStates.end()) {
        return false;
    }
    return mouseStates[button] == INPUT_STATE_JUST_BECAME_UP;
}

bool Input::getMouseButtonDown(int button) {
    if (mouseStates.find(button) == mouseStates.end()) {
        return false;
    }
    return mouseStates[button] == INPUT_STATE_JUST_BECAME_DOWN;
}

glm::vec2 Input::getMousePosition() {
    return mousePosition;
}

float Input::getMouseScrollDelta() {
    return scrollThisFrame;
}

void Input::hideCursor() {
    SDL_ShowCursor(SDL_DISABLE);
}

void Input::showCursor() {
    SDL_ShowCursor(SDL_ENABLE);
}

bool Input::getControllerButton(std::string code, int id) {
    if (code == "L2") {
        if (L2Val[id] > 0) {
            return true;
        }
        else {
            return false;
        }
    }
    if (code == "R2") {
        if (R2Val[id] > 0) {
            return true;
        }
        else {
            return false;
        }
    }
    if (controllerToKeycode.find(code) == controllerToKeycode.end()) {
        return false;
    }
    SDL_GameControllerButton keycode = controllerToKeycode[code];
    return (controllerStates[id][keycode] == INPUT_STATE_DOWN || controllerStates[id][keycode] == INPUT_STATE_JUST_BECAME_DOWN);
}

bool Input::getControllerButtonUp(std::string code, int id) {
    if (controllerToKeycode.find(code) == controllerToKeycode.end()) {
        return false;
    }
    SDL_GameControllerButton keycode = controllerToKeycode[code];
    return controllerStates[id][keycode] == INPUT_STATE_JUST_BECAME_UP;
}

bool Input::getControllerButtonDown(std::string code, int id) {
    if (controllerToKeycode.find(code) == controllerToKeycode.end()) {
        return false;
    }
    SDL_GameControllerButton keycode = controllerToKeycode[code];
    return controllerStates[id][keycode] == INPUT_STATE_JUST_BECAME_DOWN;
}

bool Input::getLeftJoystickDirection(std::string direction, int id) {
    if (direction == "left") {
        if (LeftJoystickXVal[id] < -8000) {
            return true;
        }
        else {
            return false;
        }
    }
    if (direction == "right") {
        if (LeftJoystickXVal[id] > 8000) {
            return true;
        }
        else {
            return false;
        }
    }
    if (direction == "down") {
        if (LeftJoystickYVal[id] > 8000) {
            return true;
        }
        else {
            return false;
        }
    }
    if (direction == "up") {
        if (LeftJoystickYVal[id] < -8000) {
            return true;
        }
        else {
            return false;
        }
    }
    return false;
}

bool Input::getRightJoystickDirection(std::string direction, int id) {
    if (direction == "left") {
        if (RightJoystickXVal[id] < -8000) {
            return true;
        }
        else {
            return false;
        }
    }
    if (direction == "right") {
        if (RightJoystickXVal[id] > 8000) {
            return true;
        }
        else {
            return false;
        }
    }
    if (direction == "down") {
        if (RightJoystickYVal[id] > 8000) {
            return true;
        }
        else {
            return false;
        }
    }
    if (direction == "up") {
        if (RightJoystickYVal[id] < -8000) {
            return true;
        }
        else {
            return false;
        }
    }
    return false;
}
