#pragma once

// #define USE_RUNTIME_CONSTANTS    // This will be added by CMake for the runtime executable

#ifdef USE_RUNTIME_CONSTANTS
#include "constants_gui_mode.hpp"
#else
#include "constants_oj_mode.hpp"
#endif