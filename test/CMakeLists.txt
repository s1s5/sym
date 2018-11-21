set(CPPUT_MAIN ${CMAKE_CURRENT_SOURCE_DIR}/cpput_main.cpp)
set(CPPUT_LIBS dl)

macro(py_add_test module_name)
  set(list_var "${ARGN}")
  foreach(test_filename IN LISTS list_var)
    add_executable(test_${module_name}_${test_filename} test_${test_filename}.cpp ${CPPUT_MAIN})
    target_link_libraries(test_${module_name}_${test_filename} ${CPPUT_LIBS})
    add_test(
      NAME test_${module_name}_${test_filename}
      COMMAND test_${module_name}_${test_filename})
  endforeach()
endmacro()
