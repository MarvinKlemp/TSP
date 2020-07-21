set(WARNINGS "-Wall -Wextra -pedantic -Weffc++")
set(CMAKE_CXX_FLAGS "${WARNINGS}")

set(CMAKE_CXX_FLAGS_DEBUG  "-O0 -g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Werror")

add_executable(tsp main.cpp)
