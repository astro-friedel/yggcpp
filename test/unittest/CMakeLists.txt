include(CheckCXXCompilerFlag)

set(UNITTEST_SOURCES
        testutils.cpp
        unittest.cpp)

find_program(CCACH_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Qunused-arguments -fcolor-diagnostics")
    endif()
endif(CCACHE_FOUND)

set_property(DIRECTORY PROPERTY COMPILE_OPTIONS ${EXTRA_CXX_FLAGS})
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    # If the user is running a newer version of Clang that includes the
    # -Wdouble-promotion, we will ignore that warning.
    if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 3.7)
        CHECK_CXX_COMPILER_FLAG("-Wno-double-promotion" HAS_NO_DOUBLE_PROMOTION)
        if (HAS_NO_DOUBLE_PROMOTION)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-double-promotion")
        endif()
    endif()
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    # Force to always compile with /W4
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()

    # Force to always compile with /WX
    if(CMAKE_CXX_FLAGS MATCHES "/WX-")
        string(REGEX REPLACE "/WX-" "/WX" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")
    endif()
endif()

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DRAPIDJSON_HAS_STDSTRING=1")

#add_library(namespacetest STATIC namespacetest.cpp)

add_executable(unittest ${UNITTEST_SOURCES})
target_link_libraries(unittest ${TEST_LIBRARIES} YggInterface gomp)

add_dependencies(tests unittest)
add_definitions(-DYGG_TEST)
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|ARMClang")
    add_custom_command(TARGET unittest POST_BUILD
            COMMAND dsymutil unittest
            WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
            COMMENT "Creating debug symbols...")
endif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|ARMClang")

message(STATUS "YGG_PYTHON_ENV = ${YGG_PYTHON_ENV}")
add_test(NAME unittest
        COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/unittest
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set_tests_properties(unittest PROPERTIES
        ENVIRONMENT
        "${YGG_PYTHON_ENV}")


option(YGG_SKIP_VALGRIND_TESTS "Disable valgrind tests" OFF)
if(NOT MSVC)
    find_program(VALGRIND_FOUND valgrind)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DYGG_DONT_IMPORT_NUMPY")
    if((NOT YGG_SKIP_VALGRIND_TESTS) AND VALGRIND_FOUND)
        # Not running SIMD.* unit test cases for Valgrind
        add_test(NAME valgrind_unittest
                COMMAND valgrind --suppressions=${CMAKE_SOURCE_DIR}/test/valgrind.supp --suppressions=${CMAKE_SOURCE_DIR}/test/valgrind-python.supp --leak-check=full --error-exitcode=1 --track-origins=yes --dsymutil=no --keep-debuginfo=yes --read-var-info=yes ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/unittest --gtest_filter=-SIMD.*
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
        set_tests_properties(valgrind_unittest PROPERTIES
                ENVIRONMENT
                "${YGG_PYTHON_ENV}")
    endif((NOT YGG_SKIP_VALGRIND_TESTS) AND VALGRIND_FOUND)

    #if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    #    add_test(NAME symbol_check
    #            COMMAND sh -c "objdump -t -C libnamespacetest.a | grep rapidjson ; test $? -ne 0"
    #            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    #endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

endif(NOT MSVC)