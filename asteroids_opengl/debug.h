#ifndef DEBUG_H
#define DEBUG_H

#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL 0
#endif

#include <iostream>

#define debug(level, message) { if (level <= DEBUG_LEVEL) std::cout <<  message << "\t\t(" << __FILE__ << ", line: " << __LINE__ << ") " << std::endl; }
#define error(message) debug(1, (message));
#define warning(message) debug(1, (message));

#endif 
