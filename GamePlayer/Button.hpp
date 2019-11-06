#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "StructData.hpp"
#include "hwlib.hpp"
#include "rtos.hpp"

/// @file

///\brief
/// Interface ButtonListener
///\details
/// This class is a interface for a buttons.
class ButtonListener {
 public:
  virtual void buttonPressed(int button_id) = 0;
};

///\brief
/// Button
///\details
/// This class implements a button
class Button {
 private:
  hwlib::target::pin_in &button;
  unsigned int index_listener = 0;
  unsigned int id;
  ButtonListener *button_listeners[];

 public:
  Button(hwlib::target::pin_in &button, unsigned int id) : button(button), id(id) {}

  ///\brief
  /// addButtonListener
  ///\details
  /// Add ButtonListener class to the ButtonListener array.
  void addButtonListener(ButtonListener *object);

  ///\brief
  /// update
  ///\details
  /// call the buttonPressed functions on all the ButtonListeners classes in the array
  void update();
};

#endif  // BUTTON_HPP