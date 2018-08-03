#ifndef OBJECT_H
#define OBJECT_H

#include <string>
#include <vector>

namespace __CORE
{

    /* Basic object. Stores simple rendering details */
    class Object {
    public:
        Object(std::string _name) : Object(_name, 0, 0, 0, 0) {} // Object o;
        Object(std::string _name, double x, double y, double w, double h) : name(_name), x(x), y(y), w(w), h(h) {} //Object o(1.0, 1.2, 1.3, 1.4);
        ~Object() {}

        void Move(double dx, double dy);
        void SetPosition(double x, double y);
        virtual void Tick();
        virtual void Render(); //

    private:
        /* Absolute position on screen */
        double x, y; //SetPosition
        double w, h; //Set Size
        /* Velocity */
        double dx, dy; //Move

        /* Tile from tilesheet */
        int sx, sy; // X -> Different sprite, Y -> Animation
        int sw, sh; //64

        std::string tilesheet;

        //Nice name for debugging to recognize
        std::string name;
    };

    // HandleUserKeyboard
    // HandleUserMouse
    // HandleUserScroll
    class PlayerControlledObject : public Object {
    public:
        void HandleUserKeyboard(SDL_Keycore k, bool isDown, uint32_t modifiers);
        void HandleUserMouse(int32_t x, int32_t y, int32_t button, bool isDown);
        void HandleUserScroll(int32_t dx, int32_t dy);
    };

}

#endif //OBJECT_H