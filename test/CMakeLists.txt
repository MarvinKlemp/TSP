add_executable(unit_tests main.cpp mock/RandomMock.h genetic_tsp/OrderedCrossoverTest.cpp genetic_tsp/ReverseSequenceMutationTest.cpp genetic_tsp/RouletteWheelSelectionTest.cpp)

target_link_libraries(unit_tests gtest_main gmock)

add_test(NAME unit-tests COMMAND ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/unit_tests)
