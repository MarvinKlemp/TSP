set(WARNINGS "-Wall -Wextra -pedantic -Weffc++ -Werror")
set(CMAKE_CXX_FLAGS "${WARNINGS} -O3")

add_executable(tsp main.cpp)
