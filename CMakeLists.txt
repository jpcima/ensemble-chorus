cmake_minimum_required(VERSION 3.5)

project(EnsembleChorus VERSION "0.0.1" LANGUAGES C CXX)
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

option(BUILD_PLUGIN "Build plugin" ON)
option(ENABLE_PROFILING "Enable profiling" OFF)

include(GNUWarnings)
enable_gnu_warning(all)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")

add_library(ensemble_chorus STATIC
  "sources/core/bbd.cc"
  "sources/core/chorus.cc"
  "sources/core/lfos.cc"
  "sources/core/core.cc")
target_include_directories(ensemble_chorus PUBLIC
  "sources/core/include")
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  target_compile_definitions(ensemble_chorus PRIVATE "EC_API=__declspec(dllexport)")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "^(GNU|Clang)$")
  target_compile_definitions(ensemble_chorus PRIVATE "EC_API=__attribute__((visibility(\"default\")))")
endif()
set_target_properties(ensemble_chorus PROPERTIES
  CXX_STANDARD 11
  CXX_STANDARD_REQUIRED ON
  CXX_VISIBILITY_PRESET "hidden"
  VISIBILITY_INLINES_HIDDEN ON)

add_library(ensemble_chorus_headers INTERFACE)
target_include_directories(ensemble_chorus_headers INTERFACE
  "sources/core/include")

add_subdirectory("thirdparty/DSPFilters/shared/DSPFilters" EXCLUDE_FROM_ALL)
add_subdirectory("thirdparty/gsl-lite" EXCLUDE_FROM_ALL)
add_subdirectory("thirdparty/jsl" EXCLUDE_FROM_ALL)

include_directories("thirdparty/boost/predef/include")
target_link_libraries(ensemble_chorus DSPFilters jsl gsl-lite)

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
    PRIVATE "${FLTK_INCLUDE_DIR}"
    PRIVATE "${PROJECT_SOURCE_DIR}/sources/fl/widgets/Fl_Knob")
  target_link_libraries(ensemble_chorus_fl PRIVATE "${FLTK_LIBRARIES}")

  set(RtAudio_SOURCE_DIR "${PROJECT_SOURCE_DIR}/thirdparty/rtaudio")
  include(RtAudio)

  add_subdirectory("thirdparty/pugixml" EXCLUDE_FROM_ALL)

  target_link_libraries(ensemble_chorus_fl PRIVATE ensemble_chorus RtAudio pugixml ring_buffer)

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
  add_library(ensemble_chorus_processor
    "sources/plugin/processor.cc")
  target_compile_definitions(ensemble_chorus_processor PRIVATE
    "PROJECT_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}"
    "PROJECT_VERSION_MINOR=${PROJECT_VERSION_MINOR}"
    "PROJECT_VERSION_PATCH=${PROJECT_VERSION_PATCH}")
  target_include_directories(ensemble_chorus_processor
    PRIVATE "sources" "thirdparty/DPF"
    PUBLIC "sources/plugin/meta")
  target_link_libraries(ensemble_chorus_processor
    PRIVATE ensemble_chorus)
  set_target_properties(ensemble_chorus_processor PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
    CXX_VISIBILITY_PRESET "hidden"
    VISIBILITY_INLINES_HIDDEN ON)
  ###
  add_library(ensemble_chorus_editor
    "sources/plugin/editor/editor.cc"
    "sources/plugin/editor/widgets/knob.cc")
  target_include_directories(ensemble_chorus_editor
    PRIVATE "sources" "thirdparty/DPF"
    PUBLIC "sources/plugin/meta")
  target_link_libraries(ensemble_chorus_editor
    PRIVATE ensemble_chorus_headers)
  set_target_properties(ensemble_chorus_editor PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
    CXX_VISIBILITY_PRESET "hidden"
    VISIBILITY_INLINES_HIDDEN ON)
  ###
  add_dpf_plugin(ensemble_chorus
    ensemble_chorus_processor ensemble_chorus_editor)
endif()
