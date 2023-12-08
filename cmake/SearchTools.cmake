function(find_package_zmq)
    if (NOT CONDA_PREFIX)
        cmake_path(SET CONDA_PREFIX "$ENV{CONDA_PREFIX}")
    endif()
    find_package(ZeroMQ CONFIG)
    if (NOT ZeroMQ_FOUND)
        message(STATUS "ZeroMQ could not be found using default search tree. Trying via pkg-config...")
        # Only conda version has CMake config
        ## load in pkg-config support
        find_package(PkgConfig)
        ## use pkg-config to get hints for 0mq locations
        pkg_check_modules(PC_ZeroMQ QUIET zmq libzmq libzmq-static)

        if (CONDA_PREFIX)
	    if (WIN32)
                set(PC_ZeroMQ_INCLUDE_DIRS "${CONDA_PREFIX}/Library/include ${PC_ZeroMQ_INCLUDE_DIRS}")
                set(PC_ZeroMQ_LIBRARY_DIRS "${CONDA_PREFIX}/Library/lib ${CONDA_PREFIX}/Library/bin ${PC_ZeroMQ_LIBRARY_DIRS}")
            else()
                set(PC_ZeroMQ_INCLUDE_DIRS "${CONDA_PREFIX}/include ${PC_ZeroMQ_INCLUDE_DIRS}")
                set(PC_ZeroMQ_LIBRARY_DIRS "${CONDA_PREFIX}/lib ${PC_ZeroMQ_LIBRARY_DIRS}")
            endif()
        endif()

        ## use the hint from above to find where 'zmq.h' is located
        find_path(ZeroMQ_INCLUDE_DIR
            NAMES zmq.h
            PATHS ${PC_ZeroMQ_INCLUDE_DIRS})

        ## use the hint from above to find the location of libzmq
        find_library(ZeroMQ_LIBRARY_NEW
            NAMES zmq libzmq libzmq-static
            PATHS ${PC_ZeroMQ_LIBRARY_DIRS})

        set(ZeroMQ_LIBRARY ${ZeroMQ_LIBRARY_NEW})
	if ((NOT ZeroMQ_INCLUDE_DIR STREQUAL ZeroMQ_INCLUDE_DIR-NOTFOUND) AND (NOT ZeroMQ_LIBRARY STREQUAL ZeroMQ_LIBRARY-NOTFOUND))
            set(ZeroMQ_FOUND 1)
	endif()
    endif()
    # Force error
    if ((NOT ZeroMQ_FOUND) AND (WIN32))
        find_package(ZeroMQ CONFIG REQUIRED)
    endif()

    if (ZeroMQ_FOUND)
        message(STATUS "ZeroMQ found")
        if (WIN32 AND ZeroMQ_LIBRARY)
            string(FIND ${ZeroMQ_LIBRARY} ".dll" ZeroMQ_DLL_POS)
            if (NOT "${ZeroMQ_DLL_POS}" STREQUAL "-1")
                get_filename_component(ZeroMQ_BIN_DIR ${ZeroMQ_LIBRARY} DIRECTORY)
                get_filename_component(ZeroMQ_PREFIX_DIR ${ZeroMQ_BIN_DIR} DIRECTORY)
                get_filename_component(ZeroMQ_BASE_NAME ${ZeroMQ_LIBRARY} NAME_WLE)
                find_library(ZeroMQ_LIBRARY_LIB
                    NAMES ${ZeroMQ_BASE_NAME}
                    PATHS ${ZeroMQ_PREFIX_DIR}/lib)
                set(ZeroMQ_LIBRARY ${ZeroMQ_LIBRARY_LIB})
 	        message(STATUS "Updated ZeroMQ_LIBRARY = ${ZeroMQ_LIBRARY}")
            endif()
        endif()
        if (ZeroMQ_STATIC_LIBRARY AND NOT ZeroMQ_LIBRARY)
            add_library(libzmq INTERFACE IMPORTED)
	    set_target_properties(libzmq PROPERTIES
	                          INTERFACE_LINK_LIBRARIES libzmq-static)
	    set(ZeroMQ_LIBRARY libzmq-static)
        elseif (NOT ZeroMQ_LIBRARY)
	    set(ZeroMQ_LIBRARY libzmq)
        endif()
    endif()
    set(ZeroMQ_FOUND ${ZeroMQ_FOUND} PARENT_SCOPE)
    set(ZeroMQ_LIBRARY ${ZeroMQ_LIBRARY} PARENT_SCOPE)
    set(ZeroMQ_STATIC_LIBRARY ${ZeroMQ_STATIC_LIBRARY} PARENT_SCOPE)
    set(ZeroMQ_INCLUDE_DIR ${ZeroMQ_INCLUDE_DIR} PARENT_SCOPE)
endfunction()


function(check_suffixes filename output)
  set(multiValueArgs INCLUDE IGNORE)
  cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
  set(out true)
  if (ARGS_INCLUDE)
    set(out false)
    foreach(suffix IN LISTS ARGS_INCLUDE)
      if (filename MATCHES ".*${suffix}$")
        set(out true)
	break()
      endif()
    endforeach()
  endif()
  if (ARGS_IGNORE)
    foreach(suffix IN LISTS ARGS_IGNORE)
      if (filename MATCHES ".*${suffix}$")
        set(out false)
	break()
      endif()
    endforeach()
  endif()
  set(${output} ${out} PARENT_SCOPE)
endfunction()

function(list_search_directories output)
  set(options NO_DEFAULT_PATH NO_CMAKE_PATH NO_CMAKE_ENVIRONMENT_PATH NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_INSTALL_PREFIX NO_CMAKE_SYSTEM_PATH NO_PACKAGE_ROOT_PATH)
  set(oneValueArgs FILETYPE)
  set(multiValueArgs PATHS HINTS PATH_SUFFIXES)
  cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
  set(${output})
  if (CMAKE_FIND_USE_CMAKE_PATH STREQUAL "FALSE")
    set(ARGS_NO_CMAKE_PATH true)
  endif()
  if (CMAKE_FIND_USE_CMAKE_ENVIRONMENT_PATH STREQUAL "FALSE")
    set(ARGS_NO_CMAKE_ENVIRONMENT_PATH true)
  endif()
  if (CMAKE_FIND_USE_SYSTEM_ENVIRONMENT_PATH STREQUAL "FALSE")
    set(ARGS_NO_SYSTEM_ENVIRONMENT_PATH true)
  endif()
  if (CMAKE_FIND_USE_INSTALL_PREFIX STREQUAL "FALSE")
    set(ARGS_NO_CMAKE_INSTALL_PREFIX true)
  endif()
  if (CMAKE_FIND_USE_CMAKE_SYSTEM_PATH STREQUAL "FALSE")
    set(ARGS_NO_CMAKE_SYSTEM_PATH true)
  endif()
  if (CMAKE_FIND_USE_PACKAGE_ROOT_PATH STREQUAL "FALSE")
    set(ARGS_NO_PACKAGE_ROOT_PATH true)
  endif()
  if (ARGS_FILETYPE STREQUAL "SHARED")
    if (WIN32)
      set(suffix "bin")
    else()
      set(suffix "lib")
    endif()
    set(pathvar "CMAKE_LIBRARY_PATH")
    set(syspathvar "CMAKE_SYSTEM_LIBRARY_PATH")
    set(envvar "LIB")
  elseif (ARGS_FILETYPE STREQUAL "STATIC" OR
          ARGS_FILETYPE STREQUAL "IMPORT")
    set(suffix "lib")
    set(pathvar "CMAKE_LIBRARY_PATH")
    set(syspathvar "CMAKE_SYSTEM_LIBRARY_PATH")
    set(envvar "LIB")
  else()
    set(suffix "include")
    set(pathvar "CMAKE_INCLUDE_PATH")
    set(syspathvar "CMAKE_SYSTEM_INCLUDE_PATH")
    set(envvar "INCLUDE")
  endif()
  # 1. Package root
  # if (NOT (ARGS_NO_DEFAULT_PATH OR ARGS_NO_PACKAGE_ROOT_PATH))
  # endif()
  # 2. & 3. cmake variables
  if (NOT (ARGS_NO_DEFAULT_PATH OR (ARGS_NO_CMAKE_PATH AND ARGS_NO_CMAKE_ENVIRONMENT_PATH)))
    if (DEFINED ENV{CONDA_PREFIX})
      set(mingw64 "$ENV{CONDA_PREFIX}")
      set(mingw32 "$ENV{CONDA_PREFIX}")
      cmake_path(APPEND mingw64
                 "$ENV{CONDA_PREFIX}" "Library" "mingw-w64")
      cmake_path(APPEND mingw32 ${mingw64} "x86_64-w64-mingw32")
      list(APPEND conda_prefixes ${mingw32} ${mingw64} $ENV{CONDA_PREFIX})
      foreach(prefix IN LISTS conda_prefixes)
        if ((EXISTS ${prefix}) AND (NOT ${prefix} IN_LIST CMAKE_PREFIX_PATH))
	  list(INSERT CMAKE_PREFIX_PATH 0 ${prefix})
	endif()
      endforeach()
      if (NOT $ENV{CONDA_PREFIX} IN_LIST CMAKE_PREFIX_PATH)
        list(INSERT CMAKE_PREFIX_PATH 0 $ENV{CONDA_PREFIX})
      endif()
    endif()
    foreach(prefix IN LISTS CMAKE_PREFIX_PATH)
      if (NOT (${prefix} IN_LIST CMAKE_IGNORE_PREFIX_PATH) OR
              (${prefix} IN_LIST CMAKE_SYSTEM_IGNORE_PREFIX_PATH))
        if (CMAKE_LIBRARY_ARCHITECTURE)
          cmake_path(APPEND prefix ${suffix}
  	             ${CMAKE_LIBRARY_ARCHITECTURE} OUTPUT_VARIABLE tmp)
  	  if (NOT (${tmp} IN_LIST CMAKE_IGNORE_PATH) OR
	          (${tmp} IN_LIST CMAKE_SYSTEM_IGNORE_PATH))
            list(APPEND ${output} ${tmp})
	  endif()
        endif()
        cmake_path(APPEND prefix ${suffix} OUTPUT_VARIABLE tmp)
	if (NOT (${tmp} IN_LIST CMAKE_IGNORE_PATH) OR
	        (${tmp} IN_LIST CMAKE_SYSTEM_IGNORE_PATH))
          list(APPEND ${output} ${tmp})
	endif()
      endif()
    endforeach()
    foreach(tmp IN LISTS ${pathvar} CMAKE_FRAMEWORK_PATH)
      if (NOT (${tmp} IN_LIST CMAKE_IGNORE_PATH) OR
              (${tmp} IN_LIST CMAKE_SYSTEM_IGNORE_PATH))
        list(APPEND ${output} ${tmp})
      endif()
    endforeach()
  endif()
  # 4. User specified hints
  list(APPEND ${output} ${ARGS_HINTS})
  # 5. Environment variables
  if (NOT (ARGS_NO_DEFAULT_PATH OR ARGS_NO_SYSTEM_ENVIRONMENT_PATH))
    if (DEFINED ENV{${envvar}})
      string(REPLACE ":" ";" ENVVAR_LIST $ENV{${envvar}})
    endif()
    if (DEFINED ENV{PATH})
      string(REPLACE ":" ";" PATH_LIST $ENV{PATH})
    endif()
    foreach(tmp IN LISTS ENVVAR_LIST PATH_LIST)
      if (NOT (${tmp} IN_LIST CMAKE_IGNORE_PATH) OR
              (${tmp} IN_LIST CMAKE_SYSTEM_IGNORE_PATH))
	list(APPEND ${output} ${tmp})
      endif()
    endforeach()
  endif()
  # Behavior removed in CMake 3.28
  # Search <prefix>/${suffix} and <prefix>/${suffix}/<arch> for each <prefix>/[s]bin in PATH
  # 6. Variables in platform files
  if (NOT (ARGS_NO_DEFAULT_PATH OR ARGS_NO_CMAKE_SYSTEM_PATH))
    foreach(prefix IN LISTS CMAKE_SYSTEM_PREFIX_PATH)
      if (NOT (${prefix} IN_LIST CMAKE_IGNORE_PREFIX_PATH) OR
              (${prefix} IN_LIST CMAKE_SYSTEM_IGNORE_PREFIX_PATH))
        if (CMAKE_LIBRARY_ARCHITECTURE)
          cmake_path(APPEND prefix ${suffix}
	             ${CMAKE_LIBRARY_ARCHITECTURE} OUTPUT_VARIABLE tmp)
          if (NOT (${tmp} IN_LIST CMAKE_IGNORE_PATH) OR
                  (${tmp} IN_LIST CMAKE_SYSTEM_IGNORE_PATH))
            list(APPEND ${output} ${tmp})
	  endif()
        endif()
        cmake_path(APPEND prefix ${suffix} OUTPUT_VARIABLE tmp)
        if (NOT (${tmp} IN_LIST CMAKE_IGNORE_PATH) OR
                (${tmp} IN_LIST CMAKE_SYSTEM_IGNORE_PATH))
          list(APPEND ${output} ${tmp})
	endif()
      endif()
    endforeach()
    foreach(tmp IN LISTS ${syspathvar} CMAKE_SYSTEM_FRAMEWORK_PATH)
      if (NOT (${tmp} IN_LIST CMAKE_IGNORE_PATH) OR
              (${tmp} IN_LIST CMAKE_SYSTEM_IGNORE_PATH))
        list(APPEND ${output} ${tmp})
      endif()
    endforeach()
    if (NOT ARGS_NO_CMAKE_INSTALL_PREFIX)
      foreach(prefix IN LISTS CMAKE_INSTALL_PREFIX CMAKE_STAGING_PREFIX)
        if (NOT (${prefix} IN_LIST CMAKE_IGNORE_PREFIX_PATH) OR
                (${prefix} IN_LIST CMAKE_SYSTEM_IGNORE_PREFIX_PATH))
          list(APPEND ${output} ${prefix})
	endif()
      endforeach()
    endif()
  endif()
  # 7. User specified paths
  list(APPEND ${output} ${ARGS_PATHS})
  # PATH_SUFFIXES
  if (ARGS_PATH_SUFFIXES)
    foreach(dir IN LISTS ${output})
      list(APPEND new_output ${dir})
      foreach(suffix IN LISTS ${ARGS_PATH_SUFFIXES})
        cmake_path(APPEND dir ${suffix} OUTPUT_VARIABLE tmp)
        list(APPEND new_output ${tmp})
      endforeach()
    endforeach()
    set(${output} ${new_output})
  endif()
  list(REMOVE_DUPLICATES ${output})
  set(${output} ${${output}} PARENT_SCOPE)
endfunction()

function(find_library_suffix output libname libtype)
  set(options NO_CACHE REQUIRED)
  cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
  if(${output} AND NOT ARGS_NO_CACHE)
    return()
  endif()
  if (NOT libtype STREQUAL "SHARED")
    find_library(${output} ${libname} ${ARGS_UNPARSED_ARGUMENTS} NO_CACHE)
    message(STATUS "find_library_suffix: Trying find_library ${libname}")
  else()
    set(${output} "${output}-NOTFOUND")
  endif()
  if (${${output}} STREQUAL "${output}-NOTFOUND")
    set(${output})
    set(prefix)
    set(suffix)
    if (libtype STREQUAL "SHARED" OR
        libtype STREQUAL "STATIC" OR
        libtype STREQUAL "IMPORT")
      set(prefix ${CMAKE_${libtype}_LIBRARY_PREFIX})
      set(suffix ${CMAKE_${libtype}_LIBRARY_SUFFIX})
    elseif (libtype STREQUAL "OBJECT")
      set(prefix)
      set(suffix ${CMAKE_C_OUTPUT_EXTENSION})
    else()
      message(FATAL_ERROR "Invalid libtype ${libtype}")
    endif()
    message(STATUS "find_library_suffix: Trying find_file ${prefix}${libname}${suffix}")
    find_file(${output} "${prefix}${libname}${suffix}" ${ARGS_UNPARSED_ARGUMENTS} NO_CACHE)
  endif()
  if (${${output}} STREQUAL "${output}-NOTFOUND")
    set(${output})
    message(STATUS "find_library_suffix: Trying find_file_regex ${prefix}${libname}*${suffix}")
    find_file_regex(${output} "${prefix}${libname}*${suffix}"
                    FILETYPE ${libtype} ${ARGS_UNPARSED_ARGUMENTS}
		    NO_CACHE)
  endif()
  if (${output} STREQUAL "${output}-NOTFOUND")
    if (ARGS_REQUIRED)
      message(FATAL_ERROR "Could not locate a library file for ${libname}")
    endif()
  else()
    message(STATUS "find_library_suffix: Found library file for ${libname}: ${${output}}")
  endif()
  if (NOT ARGS_NO_CACHE)
    set(${output} "${${output}}" CACHE FILEPATH "The location of the library file for ${libname}")
  else()
    set(${output} "${${output}}" PARENT_SCOPE)
  endif()
endfunction()

function(find_file_regex output pattern)
  set(options NO_CACHE REQUIRED)
  cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
  if(${output} AND NOT ARGS_NO_CACHE)
    return()
  endif()
  list_search_directories(search_dirs ${ARGS_UNPARSED_ARGUMENTS})
  foreach(dir IN LISTS search_dirs)
    cmake_path(APPEND dir ${pattern} OUTPUT_VARIABLE iregex)
    file(GLOB matches LIST_DIRECTORIES false ${iregex})
    foreach(match IN LISTS matches)
      set(${output} ${match})
      break()
    endforeach()
    if (${output})
      break()
    endif()
  endforeach()
  if (NOT ${output})
    set(${output} "${output}-NOTFOUND")
    if (ARGS_REQUIRED)
      message(FATAL_ERROR "Could not locate a file matching the pattern ${pattern}")
    endif()
  else()
  endif()
  if (NOT ARGS_NO_CACHE)
    set(${output} "${${output}}" CACHE FILEPATH "The location of the file matching the regex ${pattern}")
  else()
    set(${output} "${${output}}" PARENT_SCOPE)
  endif()
endfunction()

function(find_libraries)
  set(multiValueArgs LIBRARIES DIRECTORIES
      INCLUDE_SUFFIXES IGNORE_SUFFIXES)
  set(oneValueArgs OUTPUT MISSING MISMATCHED_SUFFIX LIBTYPE)
  cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
  if (NOT ARGS_LIBTYPE)
    if (WIN32)
      set(ARGS_LIBTYPE IMPORT)
    else()
      set(ARGS_LIBTYPE SHARED)
    endif()
  endif()
  list(APPEND ARGS_LIBRARIES ${ARGS_UNPARSED_ARGUMENTS})
  set(LIBS_FULL)
  set(LIBS_MISS)
  set(LIBS_UNMT)
  foreach(x IN LISTS ARGS_LIBRARIES)
    find_library_suffix(${x}_FOUND ${x} ${ARGS_LIBTYPE}
                        PATHS ${ARGS_DIRECTORIES})
    if(${x}_FOUND STREQUAL "${x}_FOUND-NOTFOUND")
      list(APPEND LIBS_MISS ${${x}_FOUND})
    else()
      message(STATUS "Looking for ${x}: ${x}_FOUND = ${${x}_FOUND}")
      check_suffixes(${${x}_FOUND} add_x
                     INCLUDE ${ARGS_INCLUDE_SUFFIXES}
		     IGNORE ${ARGS_IGNORE_SUFFIXES})
      if (add_x)
        list(APPEND LIBS_FULL ${${x}_FOUND})
      else()
        message(STATUS "Suffix does not match (include=${ARGS_INCLUDE_SUFFIXES}, ignore=${ARGS_IGNORE_SUFFIXES})")
        list(APPEND LIBS_UNMT ${x})
      endif()
    endif()
  endforeach()
  if (ARGS_OUTPUT)
    set(${ARGS_OUTPUT} ${LIBS_FULL} PARENT_SCOPE)
  endif()
  if (ARGS_MISSING)
    set(${ARGS_MISSING} ${LIBS_MISS} PARENT_SCOPE)
  endif()
  if (ARGS_MISMATCHED_SUFFIX)
    set(${ARGS_MISMATCHED_SUFFIX} ${LIBS_UNMT} PARENT_SCOPE)
  endif()
endfunction()
