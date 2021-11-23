#ifndef PCH_H
#define PCH_H

// Console libraries
#include <iostream>
#include <iomanip>

// String libraries
#ifndef KROSS_DLL
#include <string>
#endif //KROSS_DLL

#include <cstring>
#include <cctype>
#include <sstream>

// Utilities libraries
#include <algorithm>
#include <assert.h>
#include <functional>

// Math libraries
#include <math.h>

// Time Manip
#include <chrono>

// Data Manip libraries
#include <memory>
#include <array>

#ifndef KROSS_DLL
#include <vector>
#endif //KROSS_DLL

#include <unordered_map>
#include <unordered_set>

// Files Manip libraries
#include <fstream>

// Logging libraries
#include "spdlog/include/spdlog/spdlog.h"
#include "spdlog/include/spdlog/fmt/ostr.h"
#include "spdlog/include/spdlog/sinks/stdout_color_sinks.h"

#include "Kross/Core/Log.h"
#include "Kross/Core/Timestep.h"

#endif //PCH_H
