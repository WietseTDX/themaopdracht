#include "Button.hpp"

void Button::addButtonListener(ButtonListener *object){
    button_listeners[index_listener] = object;
    index_listener++;
}

void Button::update(){
    for(unsigned int i = 0; i < index_listener; i++){
        if(!button.read()){
            button_listeners[i]->buttonPressed(id);
            hwlib::cout << "Button.update()" << hwlib::endl;
        }
    }
}