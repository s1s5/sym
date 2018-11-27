add_executable(example_000 test_000)
add_executable(example_001 test_001)
add_executable(example_002 test_002)

if (EIGEN_FOUND)
  add_executable(example_eigen_000 test_eigen_000)
  target_compile_options(example_eigen_000
    PRIVATE -I${EIGEN_INCLUDE_DIR})
endif()
