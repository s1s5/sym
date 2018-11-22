set(CPPUT_MAIN ${CMAKE_CURRENT_SOURCE_DIR}/cpput_main.cpp)
set(CPPUT_LIBS dl)

macro(sym_add_test test_filename)
  add_executable(test_${test_filename} test_${test_filename}.cpp ${CPPUT_MAIN})
  target_link_libraries(test_${test_filename} ${CPPUT_LIBS})
  add_test(
    NAME test_${test_filename}
    COMMAND test_${test_filename})
endmacro()

sym_add_test(four_arithmetic_operations)
sym_add_test(unary)
sym_add_test(trigonometric)
