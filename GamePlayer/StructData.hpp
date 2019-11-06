#ifndef STRUCTDATA_HPP
#define STRUCTDATA_HPP

#include "hwlib.hpp"
#include "rtos.hpp"

///@file

/// \brief
/// StructData
/// \details
/// This class is used to communicate between keyboard & main, and IRReceive & main
struct StructData {
  StructData() {}
  StructData(int to_change, uint16_t data) : to_change(to_change), data(data) {}
  int to_change;
  uint16_t data;
};


#endif // STRUCTDATA_HPP