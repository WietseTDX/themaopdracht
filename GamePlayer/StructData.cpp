#ifndef _STRUCT_DATA
#define _STRUCT_DATA

#include "hwlib.hpp"
#include "rtos.hpp"

struct StructData {
  StructData() {}
  StructData(int to_change, uint16_t data) : to_change(to_change), data(data) {}
  int to_change;
  uint16_t data;
};


#endif // _STRUCT_DATA