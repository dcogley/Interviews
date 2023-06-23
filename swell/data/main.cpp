#include <cstdint>
#include <iostream>

/*
Register Number Register Name Scale Factor Unit
40000 State of Charge 0.1 %
40001 Grid Frequency 10 Hz
40010 Grid Voltage 100 V
40333 PV in use 1 None */

struct schema_t {
  uint32_t reg;
  const char *name;
  union {
    float f;
    uint32_t u;
  } scale;
  const char *unit;
};

static schema_t table[] = {
    {40000, "State of Charge", 0.1, "%"},
    {40001, "Grid Frequency", 10, "Hz"},
    {40010, "Grid Voltage", 100, "V"},
    {40333, "PV in use", 1, ""},
};

struct data_t {
  uint32_t reg;
  union {
    float f;
    uint32_t u;
  } value;
};

static data_t data[] = {
    {40000, 500},
    {40001, 6},
    {40010, 2.4},
    {40333, 1},
};

int main() {

  for (auto d : data) {
    for (auto t : table) {
      if (d.reg == t.reg)
        std::cout << t.name << " is " << (t.scale.f * d.value.f) << t.unit
                  << std::endl;
    }
  }
}