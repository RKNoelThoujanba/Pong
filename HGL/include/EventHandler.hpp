#pragma once

#include "Window.hpp"

namespace HGL
{
    enum class Key : int
    {
        Menu = GLFW_KEY_MENU,
        LeftSuper = GLFW_KEY_LEFT_SUPER,
        KpEqual = GLFW_KEY_KP_EQUAL,
        KpEnter = GLFW_KEY_KP_ENTER,
        KpAdd = GLFW_KEY_KP_ADD,
        KpSubtract = GLFW_KEY_KP_SUBTRACT,
        KpDivide = GLFW_KEY_KP_DIVIDE,
        KpDecimal = GLFW_KEY_KP_DECIMAL,
        Kp9 = GLFW_KEY_KP_9,
        Kp8 = GLFW_KEY_KP_8,
        KpMultiply = GLFW_KEY_KP_MULTIPLY,
        Kp7 = GLFW_KEY_KP_7,
        Kp5 = GLFW_KEY_KP_5,
        Kp2 = GLFW_KEY_KP_2,
        Kp0 = GLFW_KEY_KP_0,
        F25 = GLFW_KEY_F25,
        F19 = GLFW_KEY_F19,
        F16 = GLFW_KEY_F16,
        F15 = GLFW_KEY_F15,
        F14 = GLFW_KEY_F14,
        F13 = GLFW_KEY_F13,
        F12 = GLFW_KEY_F12,
        F11 = GLFW_KEY_F11,
        LeftControl = GLFW_KEY_LEFT_CONTROL,
        F10 = GLFW_KEY_F10,
        F9 = GLFW_KEY_F9,
        F8 = GLFW_KEY_F8,
        F22 = GLFW_KEY_F22,
        F5 = GLFW_KEY_F5,
        F17 = GLFW_KEY_F17,
        F1 = GLFW_KEY_F1,
        RightSuper = GLFW_KEY_RIGHT_SUPER,
        Pause = GLFW_KEY_PAUSE,
        F6 = GLFW_KEY_F6,
        ScrollLock = GLFW_KEY_SCROLL_LOCK,
        PageDown = GLFW_KEY_PAGE_DOWN,
        PageUp = GLFW_KEY_PAGE_UP,
        K = GLFW_KEY_K,
        E = GLFW_KEY_E,
        Kp1 = GLFW_KEY_KP_1,
        J = GLFW_KEY_J,
        Six = GLFW_KEY_6,
        Home = GLFW_KEY_HOME,
        H = GLFW_KEY_H,
        F21 = GLFW_KEY_F21,
        Up = GLFW_KEY_UP,
        I = GLFW_KEY_I,
        F3 = GLFW_KEY_F3,
        G = GLFW_KEY_G,
        F = GLFW_KEY_F,
        D = GLFW_KEY_D,
        Kp6 = GLFW_KEY_KP_6,
        C = GLFW_KEY_C,
        B = GLFW_KEY_B,
        A = GLFW_KEY_A,
        LeftAlt = GLFW_KEY_LEFT_ALT,
        F23 = GLFW_KEY_F23,
        CapsLock = GLFW_KEY_CAPS_LOCK,
        Z = GLFW_KEY_Z,
        Eight = GLFW_KEY_8,
        Backslash = GLFW_KEY_BACKSLASH,
        Space = GLFW_KEY_SPACE,
        Kp3 = GLFW_KEY_KP_3,
        One = GLFW_KEY_1,
        Minus = GLFW_KEY_MINUS,
        F2 = GLFW_KEY_F2,
        Nine = GLFW_KEY_9,
        Tab = GLFW_KEY_TAB,
        Apostrophe = GLFW_KEY_APOSTROPHE,
        Two = GLFW_KEY_2,
        L = GLFW_KEY_L,
        R = GLFW_KEY_R,
        Comma = GLFW_KEY_COMMA,
        Slash = GLFW_KEY_SLASH,
        F24 = GLFW_KEY_F24,
        T = GLFW_KEY_T,
        RightShift = GLFW_KEY_RIGHT_SHIFT,
        Seven = GLFW_KEY_7,
        M = GLFW_KEY_M,
        F4 = GLFW_KEY_F4,
        S = GLFW_KEY_S,
        Zero = GLFW_KEY_0,
        N = GLFW_KEY_N,
        Backspace = GLFW_KEY_BACKSPACE,
        Four = GLFW_KEY_4,
        Semicolon = GLFW_KEY_SEMICOLON,
        O = GLFW_KEY_O,
        F20 = GLFW_KEY_F20,
        Q = GLFW_KEY_Q,
        Period = GLFW_KEY_PERIOD,
        V = GLFW_KEY_V,
        F7 = GLFW_KEY_F7,
        End = GLFW_KEY_END,
        Y = GLFW_KEY_Y,
        Three = GLFW_KEY_3,
        U = GLFW_KEY_U,
        LeftShift = GLFW_KEY_LEFT_SHIFT,
        X = GLFW_KEY_X,
        RightControl = GLFW_KEY_RIGHT_CONTROL,
        LeftBracket = GLFW_KEY_LEFT_BRACKET,
        GraveAccent = GLFW_KEY_GRAVE_ACCENT,
        Equal = GLFW_KEY_EQUAL,
        World1 = GLFW_KEY_WORLD_1,
        F18 = GLFW_KEY_F18,
        World2 = GLFW_KEY_WORLD_2,
        RightAlt = GLFW_KEY_RIGHT_ALT,
        PrintScreen = GLFW_KEY_PRINT_SCREEN,
        Insert = GLFW_KEY_INSERT,
        Escape = GLFW_KEY_ESCAPE,
        Kp4 = GLFW_KEY_KP_4,
        RightBracket = GLFW_KEY_RIGHT_BRACKET,
        W = GLFW_KEY_W,
        Delete = GLFW_KEY_DELETE,
        Five = GLFW_KEY_5,
        Right = GLFW_KEY_RIGHT,
        P = GLFW_KEY_P,
        Left = GLFW_KEY_LEFT,
        NumLock = GLFW_KEY_NUM_LOCK,
        Enter = GLFW_KEY_ENTER,
        Down = GLFW_KEY_DOWN,

    };

    enum class MouseButton : int
    {
        Left , Right , Middle
    };

    enum class MouseScrool : int
    {
        Up , Down
    };
    
    class EventHandler
    {
    public:
        EventHandler(Window& window);
        ~EventHandler() = default;

        bool IsKeyPressed(Key key);
        bool IsKeyReleased(Key key);
        bool IsKeyRepeated(Key key);
    private:
        HGL::Window& m_Window; //this might as well be a time bomb waiting to explode
    };
}