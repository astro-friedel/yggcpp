enable_testing()

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${PROJECT_SOURCE_DIR}/cmake")
find_package(GTestSrc)

find_package(cmocka)
if(cmocka_FOUND)
    message("CMOCKA")
else()
    include(FetchContent)
    FetchContent_Declare(
            cmocka_ygg
            GIT_REPOSITORY https://gitlab.com/cmocka/cmocka.git
            GIT_TAG f5e2cd77c88d9f792562888d2b70c5a396bfbf7a
    )
    FetchContent_MakeAvailable(cmocka_ygg)
endif()

if(GTESTSRC_FOUND)

    if (WIN32 AND (NOT CYGWIN) AND (NOT MINGW))
        set(gtest_disable_pthreads ON)
        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    endif()

    IF(GTEST_SOURCE_DIR)
        add_subdirectory(${GTEST_SOURCE_DIR} ${CMAKE_BINARY_DIR}/googletest)
    endif()
    include_directories(SYSTEM ${GTEST_INCLUDE_DIR})

    set(TEST_LIBRARIES gtest gtest_main ${Python3_LIBRARIES})

endif()

if(TEST_LIBRARIES)
    option(YGG_ENABLE_COVERAGE "Enable coverage reporting" ON)
    if(YGG_ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang|ARMClang")
        message("COVERAGE ON")
        set(CMAKE_CXX_FLAGS "-g -O0 --coverage -fprofile-arcs -ftest-coverage -fprofile-filter-files=\"${CMAKE_SOURCE_DIR}/communication/*\" -fpermissive")
        set(CMAKE_C_FLAGS "-g -O0 --coverage -fprofile-arcs -ftest-coverage -fprofile-filter-files=\"${CMAKE_SOURCE_DIR}/communication/*\" -fpermissive")
        if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|ARMClang")
            set(CTEST_COVERAGE_COMMAND "llvm-cov")
            set(CTEST_COVERAGE_EXTRA_FLAGS "gcov")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-instr-generate -fcoverage-mapping")
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-instr-generate -fcoverage-mapping")
        else()
            set(CTEST_COVERAGE_COMMAND "gcov")
            set(CTEST_COVERAGE_EXTRA_FLAGS "")
        endif()
    endif(YGG_ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang|ARMClang")


    include(unittest/CMakeLists.txt)
    #add_custom_target(tests ALL)
    #add_subdirectory(perftest)
    #add_subdirectory(unittest)
    add_definitions(-DYGG_TEST)

endif()
