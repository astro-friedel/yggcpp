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
  message(STATUS "Search directories ${${output}}")
endfunction()

function(find_library_suffix output libname libtype)
  set(options NO_CACHE REQUIRED)
  cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
  if(${output} AND NOT ARGS_NO_CACHE)
    return()
  endif()
  if (NOT libtype STREQUAL "SHARED")
    find_library(${output} ${libname} ${ARGS_UNPARSED_ARGUMENTS} NO_CACHE)
    message(STATUS "Trying find_library ${libname}")
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
    message(STATUS "Trying find_file ${prefix}${libname}${suffix}")
    find_file(${output} "${prefix}${libname}${suffix}" ${ARGS_UNPARSED_ARGUMENTS} NO_CACHE)
  endif()
  if (${${output}} STREQUAL "${output}-NOTFOUND")
    set(${output})
    message(STATUS "Trying find_file_regex ${prefix}${libname}*${suffix}")
    find_file_regex(${output} "${prefix}${libname}*${suffix}"
                    FILETYPE ${libtype} ${ARGS_UNPARSED_ARGUMENTS}
		    NO_CACHE)
  endif()
  if (${output} STREQUAL "${output}-NOTFOUND")
    if (ARGS_REQUIRED)
      message(FATAL_ERROR "Could not locate a library file for ${libname}")
    endif()
  else()
    if (NOT ARGS_NO_CACHE)
      set(${output} "${${output}}" CACHE FILEPATH "The location of the library file for ${libname}")
    endif()
  endif()
  set(${output} "${${output}}" PARENT_SCOPE)
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
    if (NOT ARGS_NO_CACHE)
      set(${output} "${${output}}" CACHE FILEPATH "The location of the file matching the regex ${pattern}")
    endif()
  endif()
  set(${output} "${${output}}" PARENT_SCOPE)
endfunction()