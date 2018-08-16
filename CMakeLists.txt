cmake_minimum_required(VERSION 3.5)

project(EnsembleChorus VERSION "0.0.1" LANGUAGES C CXX)
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_VISIBILITY_PRESET "hidden")
set(CMAKE_CXX_VISIBILITY_PRESET "hidden")
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

option(BUILD_PLUGIN "Build plugin" ON)
option(ENABLE_PROFILING "Enable profiling" OFF)

include(GNUWarnings)
enable_gnu_warning(all)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")

add_library(ensemble_chorus STATIC
  "sources/core/bbd.cc"
  "sources/core/chorus.cc"
  "sources/core/lfos.cc"
  "sources/core/core.cc"
  "sources/core/core_preset.cc")
target_include_directories(ensemble_chorus PUBLIC "sources/core/include")
target_compile_definitions(ensemble_chorus PUBLIC "EC_API=")

add_library(ensemble_chorus_headers INTERFACE)
target_include_directories(ensemble_chorus_headers INTERFACE
  "sources/core/include")

add_subdirectory("thirdparty/gsl-lite" EXCLUDE_FROM_ALL)
add_subdirectory("thirdparty/jsl" EXCLUDE_FROM_ALL)

include_directories("thirdparty/boost/predef/include")
target_link_libraries(ensemble_chorus PRIVATE jsl gsl-lite)

set(DSPFilters_SOURCE_DIR
  "${PROJECT_SOURCE_DIR}/thirdparty/DSPFilters/shared/DSPFilters")
add_library(DSPFilters STATIC
  "${DSPFilters_SOURCE_DIR}/source/Biquad.cpp"
  "${DSPFilters_SOURCE_DIR}/source/Cascade.cpp"
  "${DSPFilters_SOURCE_DIR}/source/Legendre.cpp"
  "${DSPFilters_SOURCE_DIR}/source/PoleFilter.cpp"
  "${DSPFilters_SOURCE_DIR}/source/RBJ.cpp"
  "${DSPFilters_SOURCE_DIR}/source/RootFinder.cpp")
target_include_directories(DSPFilters
  PUBLIC "${DSPFilters_SOURCE_DIR}/include")

target_link_libraries(ensemble_chorus PRIVATE DSPFilters)

set(pugixml_SOURCE_DIR "${PROJECT_SOURCE_DIR}/thirdparty/pugixml/src")
add_library(pugixml STATIC "${pugixml_SOURCE_DIR}/pugixml.cpp")
target_include_directories(pugixml PUBLIC "${pugixml_SOURCE_DIR}")

target_link_libraries(ensemble_chorus PRIVATE pugixml)

###
set(FLTK_SKIP_OPENGL ON)
set(FLTK_SKIP_FORMS ON)
set(FLTK_SKIP_IMAGES ON)
find_package(FLTK)

add_library(ring_buffer STATIC EXCLUDE_FROM_ALL
  "thirdparty/ring-buffer/sources/ring_buffer.cc")
target_include_directories(ring_buffer
  PUBLIC "thirdparty/ring-buffer/include")

if(FLTK_FOUND)
  add_executable(ensemble_chorus_fl
    "sources/fl/widgets/Fl_Knob/Fl_Knob/Fl_Knob.cxx"
    "sources/fl/messages.cc"
    "sources/fl/message_queue.cc"
    "sources/fl/modulator_visu.cc"
    "sources/fl/main_view.cxx"
    "sources/fl/main_view_impl.cc"
    "sources/fl/main_controller.cc"
    "sources/fl/main.cc")
  target_include_directories(ensemble_chorus_fl
    PRIVATE "sources"
    PRIVATE "${FLTK_INCLUDE_DIR}"
    PRIVATE "${PROJECT_SOURCE_DIR}/sources/fl/widgets/Fl_Knob")
  target_link_libraries(ensemble_chorus_fl PRIVATE "${FLTK_LIBRARIES}")

  set(RtAudio_SOURCE_DIR "${PROJECT_SOURCE_DIR}/thirdparty/rtaudio")
  include(RtAudio)

  target_link_libraries(ensemble_chorus_fl PRIVATE ensemble_chorus RtAudio ring_buffer jsl gsl-lite)

  if(ENABLE_PROFILING)
    find_library(PROFILER_LIBRARY "profiler" REQUIRED)
    target_link_libraries(ensemble_chorus_fl PRIVATE "${PROFILER_LIBRARY}")
  endif()

  install(TARGETS ensemble_chorus_fl
    DESTINATION "bin")
  install(FILES "resources/icon/ensemble_chorus.xpm" "resources/icon/ensemble_chorus.png"
    DESTINATION "share/pixmaps")
  install(FILES "resources/desktop/ensemble_chorus.desktop"
    DESTINATION "share/applications")
endif()

if(BUILD_PLUGIN)
  set(DPF_root_dir "${PROJECT_SOURCE_DIR}/thirdparty/DPF")
  include(DPF)
  ###
  add_dpf_processor(ensemble_chorus_processor
    "sources/plugin/processor.cc")
  target_compile_definitions(ensemble_chorus_processor PRIVATE
    "PROJECT_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}"
    "PROJECT_VERSION_MINOR=${PROJECT_VERSION_MINOR}"
    "PROJECT_VERSION_PATCH=${PROJECT_VERSION_PATCH}")
  target_include_directories(ensemble_chorus_processor
    PRIVATE "sources" "thirdparty/DPF"
    PUBLIC "sources/plugin/meta")
  target_link_libraries(ensemble_chorus_processor
    PRIVATE ensemble_chorus ring_buffer)
  ###
  add_dpf_ui(ensemble_chorus_editor
    "sources/plugin/editor/editor.cc"
    "sources/plugin/editor/modulation_view.cc"
    "sources/plugin/editor/widgets/knob.cc"
    "sources/plugin/editor/widgets/slider.cc"
    "sources/plugin/editor/widgets/button.cc"
    "sources/plugin/editor/graphics/font.cc"
    "sources/plugin/editor/graphics/box.cc"
    "sources/plugin/editor/graphics/text.cc")
  target_include_directories(ensemble_chorus_editor
    PRIVATE "sources" "thirdparty/DPF"
    PUBLIC "sources/plugin/meta")
  target_link_libraries(ensemble_chorus_editor
    PRIVATE ensemble_chorus_headers ring_buffer)

  find_package(ZLIB REQUIRED)
  target_include_directories(ensemble_chorus_editor PRIVATE "${ZLIB_INCLUDE_DIRS}")
  if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    include(StaticLinking)
    find_static_library_of_library(ZLIB_STATIC_LIBRARY "${ZLIB_LIBRARY}")
    if(ZLIB_STATIC_LIBRARY)
      message(STATUS "Found static ZLIB: ${ZLIB_STATIC_LIBRARY}")
    else()
      message(FATAL_ERROR "Could not find static ZLIB")
    endif()
    target_link_libraries(ensemble_chorus_editor PRIVATE "${ZLIB_STATIC_LIBRARY}")
  else()
    target_link_libraries(ensemble_chorus_editor PRIVATE "${ZLIB_LIBRARY}")
  endif()

  target_include_directories(ensemble_chorus_editor PRIVATE "thirdparty/gzip-hpp/include")

  ###
  add_dpf_plugin(ensemble_chorus
    ensemble_chorus_processor ensemble_chorus_editor)
endif()
