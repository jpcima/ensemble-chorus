#
# Warnings for GCC and Clang
#
#
# Usage example:
#   enable_gnu_warning("all")
#   disable_gnu_warning("unused-parameter")
#

include(CheckCCompilerFlag)
include(CheckCXXCompilerFlag)

macro(enable_gnu_warning ID)
  if (CMAKE_C_COMPILER_ID STREQUAL "GNU" OR CMAKE_C_COMPILER_ID STREQUAL "Clang")
    check_c_compiler_flag("-Werror -W${ID}" _test)
    if(_test)
      add_compile_options($<$<COMPILE_LANGUAGE:C>:-W${ID}>)
    endif()
  endif()
  if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    check_cxx_compiler_flag("-Werror -W${ID}" _test)
    if(_test)
      add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-W${ID}>)
    endif()
  endif()
  unset(_test)
endmacro()

macro(disable_gnu_warning ID)
  enable_gnu_warning("no-${ID}")
endmacro()
