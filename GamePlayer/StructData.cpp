#include "hwlib.hpp"
#include "rtos.hpp"

struct StructData {
  StructData(int to_change, uint16_t data) : to_change(to_change), data(data) {};
  int to_change;
  uint16_t data;
};