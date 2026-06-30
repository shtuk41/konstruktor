find_package(GTest CONFIG REQUIRED)

add_executable(KonstruktorTests "${CMAKE_CURRENT_SOURCE_DIR}/test_main.cpp")

target_link_libraries(KonstruktorTests 
    PRIVATE 
        GTest::gtest_main   # Automatically provides a standard main() entry point
        GTest::gtest        # The core GoogleTest library
        KonstruktorLib
)

target_include_directories(KonstruktorTests PRIVATE ../src)

add_test(NAME KonstruktorTests COMMAND KonstruktorTests)