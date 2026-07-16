#pragma once

#include <string>
#include "Keyboard.h"

class Game
{
    public:
        // constructor
        
        Game(
            std::string title,
            int width,
            int height
        ) :
            m_Title(title),
            m_Width(width),
            m_Height(height)
        {}

        // virtual destructor
        virtual ~Game() = default;


    public:
        // virtual functions

        virtual void Init() = 0;
        virtual void Update(float dt) = 0;
        virtual void Render() = 0;
        virtual void Shutdown() = 0;

        virtual void OnActivated() = 0;
        virtual void OnDeactivated() = 0;
        virtual void OnSuspending() = 0;
        virtual void OnResuming() = 0;
    
    public:
        // inline functions
        
        // getters
        inline std::string GetTitle() const { return m_Title; }
        inline int GetWidth() const { return m_Width; }
        inline int GetHeight() const { return m_Height; }
        inline DirectX::Keyboard* GetKeyboard() const { return m_Keyboard; }

        // setters
        inline void SetKeyboard(DirectX::Keyboard* keyboard) { m_Keyboard = keyboard; }

    private:
        std::string m_Title = "";
        int m_Width = 0;
        int m_Height = 0;

        DirectX::Keyboard* m_Keyboard = nullptr;
};
