#pragma once

#include <string>


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
        virtual void Shutdown() = 0;


    public:
        // inline functions
        
        inline std::string GetTitle() const { return m_Title; }
        inline int GetWidth() const { return m_Width; }
        inline int GetHeight() const { return m_Height; }


    private:
        std::string m_Title = "";
        int m_Width = 0;
        int m_Height = 0;
};
