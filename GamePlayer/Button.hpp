#ifndef _BUTTON
#define _BUTTON

#include "hwlib.hpp"
#include "rtos.hpp"

#include "StructData.cpp"

class ButtonListener{
public:
    virtual void buttonPressed(int button_id) = 0;
};

class Button{
private:
    hwlib::target::pin_in & button;
    unsigned int index_listener = 0;
    int id;
    ButtonListener *button_listeners[];
public:
    Button(hwlib::target::pin_in & button, int id):
    button(button),
    id(id)
    {}

    void addButtonListener(ButtonListener *object);

    void update();
};

#endif // _BUTTON