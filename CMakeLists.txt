cmake_minimum_required(VERSION 3.5)

project(EnsembleChorus VERSION "0.0.1" LANGUAGES C CXX)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

include(SystemOption)
system_option(EnsembleChorus_VST2 "Build VST2" ON)
system_option(EnsembleChorus_LV2 "Build LV2" ON "Windows" OFF "Darwin" OFF)
system_option(EnsembleChorus_Standalone "Build Standalone" OFF)
option(EnsembleChorus_NoUI "Build without UI" OFF)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_VISIBILITY_PRESET "hidden")
set(CMAKE_CXX_VISIBILITY_PRESET "hidden")
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

option(ENABLE_PROFILING "Enable profiling" OFF)

include(GNUWarnings)
enable_gnu_warning(all)

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

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

###
set(JUCE_ROOT_DIR "${PROJECT_SOURCE_DIR}/thirdparty/JUCE")
set(JUCE_PROJECT_DIR "${PROJECT_SOURCE_DIR}/sources/plugin")
set(JUCE_NO_UI "${EnsembleChorus_NoUI}")
include(JUCE)

set(ensemble_chorus_JUCE_MODULES
  juce_core juce_events juce_data_structures
  juce_audio_basics juce_audio_devices juce_audio_formats juce_audio_processors juce_audio_utils
  juce_graphics)
if(NOT EnsembleChorus_NoUI)
  list(APPEND ensemble_chorus_JUCE_MODULES
    juce_gui_basics juce_gui_extra)
endif()

add_library(ensemble_chorus_plugin_core STATIC EXCLUDE_FROM_ALL
  sources/plugin/Source/PluginProcessor.cpp
  sources/plugin/JuceLibraryCode/BinaryData.cpp)
if(NOT EnsembleChorus_NoUI)
  target_sources(ensemble_chorus_plugin_core PRIVATE
    sources/plugin/Source/PluginEditor.cpp
    sources/plugin/Source/LookAndFeel.cpp
    sources/plugin/Source/MainComponent.cpp
    sources/plugin/Source/ModulatorVisu.cpp)
endif()
target_include_directories(ensemble_chorus_plugin_core
  PRIVATE "sources"
  PRIVATE "sources/plugin/JuceLibraryCode")
target_link_libraries(ensemble_chorus_plugin_core
  PUBLIC ensemble_chorus ring_buffer ${ensemble_chorus_JUCE_MODULES})

if(EnsembleChorus_Standalone)
  add_juce_standalone(ensemble_chorus_standalone)
  target_link_libraries(ensemble_chorus_standalone PRIVATE ensemble_chorus_plugin_core)
  set_target_properties(ensemble_chorus_standalone PROPERTIES
    OUTPUT_NAME "EnsembleChorus" RUNTIME_OUTPUT_DIRECTORY "standalone")
endif()

if(EnsembleChorus_VST2)
  add_juce_vst(ensemble_chorus_vst)
  target_link_libraries(ensemble_chorus_vst PRIVATE ensemble_chorus_plugin_core)
  set_target_properties(ensemble_chorus_vst PROPERTIES
    OUTPUT_NAME "EnsembleChorus" LIBRARY_OUTPUT_DIRECTORY "vst2")

  if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set_target_properties(ensemble_chorus_vst PROPERTIES
      PREFIX "" SUFFIX "" LIBRARY_OUTPUT_DIRECTORY "vst2/EnsembleChorus.vst/Contents/MacOS")
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/vst2/EnsembleChorus.vst/Contents/")
    file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/vst2/EnsembleChorus.vst/Contents/PkgInfo" "BNDL????")
    add_custom_target(ensemble_chorus_vst_InfoPlist ALL
      "${CMAKE_COMMAND}"
      "-DCONFIGURE_FILE_INPUT=${PROJECT_SOURCE_DIR}/sources/plugin/Builds/MacOSX/Info-VST.plist"
      "-DCONFIGURE_FILE_OUTPUT=${CMAKE_CURRENT_BINARY_DIR}/vst2/EnsembleChorus.vst/Contents/Info.plist"
      "-DEXECUTABLE_NAME=EnsembleChorus"
      "-P" "${PROJECT_SOURCE_DIR}/cmake/ConfigureFile.cmake"
      DEPENDS "${PROJECT_SOURCE_DIR}/sources/plugin/Builds/MacOSX/Info-VST.plist")
  else()
    install(TARGETS ensemble_chorus_vst DESTINATION "lib/vst")
  endif()
endif()

if(EnsembleChorus_LV2)
  add_juce_lv2(ensemble_chorus_lv2)
  target_link_libraries(ensemble_chorus_lv2 PRIVATE ensemble_chorus_plugin_core)
  set_target_properties(ensemble_chorus_lv2 PROPERTIES
    OUTPUT_NAME "EnsembleChorus" LIBRARY_OUTPUT_DIRECTORY "lv2/EnsembleChorus.lv2")

  if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(LV2_UI_CLASS "ui:WindowsUI")
  elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(LV2_UI_CLASS "ui:CocoaUI")
  else()
    set(LV2_UI_CLASS "ui:X11UI")
  endif()
  set(LV2_BINARY_SUFFIX "${CMAKE_SHARED_MODULE_SUFFIX}")

  if(NOT EnsembleChorus_NoUI)
    configure_file(
      "${PROJECT_SOURCE_DIR}/resources/lv2/EnsembleChorus.lv2/manifest.ttl.in"
      "${PROJECT_BINARY_DIR}/lv2-build/EnsembleChorus.lv2/manifest.ttl"
      @ONLY)
  else()
    configure_file(
      "${PROJECT_SOURCE_DIR}/resources/lv2/EnsembleChorus.lv2/manifest-noui.ttl.in"
      "${PROJECT_BINARY_DIR}/lv2-build/EnsembleChorus.lv2/manifest.ttl"
      @ONLY)
  endif()

  set(ensemble_chorus_lv2_MANIFESTS
    "${PROJECT_BINARY_DIR}/lv2-build/EnsembleChorus.lv2/manifest.ttl"
    "resources/lv2/EnsembleChorus.lv2/presets.ttl")
  add_custom_target(ensemble_chorus_lv2_manifests_copy ALL
    COMMAND "${CMAKE_COMMAND}" "-E" "make_directory" "${CMAKE_CURRENT_BINARY_DIR}/lv2/EnsembleChorus.lv2/"
    COMMAND "${CMAKE_COMMAND}" "-E" "copy" ${ensemble_chorus_lv2_MANIFESTS} "${CMAKE_CURRENT_BINARY_DIR}/lv2/EnsembleChorus.lv2/"
    DEPENDS ${ensemble_chorus_lv2_MANIFESTS}
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

  install(TARGETS ensemble_chorus_lv2 DESTINATION "lib/lv2/EnsembleChorus.lv2")
  install(FILES ${ensemble_chorus_lv2_MANIFESTS} DESTINATION "lib/lv2/EnsembleChorus.lv2")

  if(NOT EnsembleChorus_NoUI)
    execute_process(COMMAND "${CMAKE_COMMAND}" -E copy
      "${PROJECT_SOURCE_DIR}/resources/lv2/EnsembleChorus.lv2/EnsembleChorus.ttl"
      "${CMAKE_CURRENT_BINARY_DIR}/lv2/EnsembleChorus.lv2/EnsembleChorus.ttl")
    install(FILES "resources/lv2/EnsembleChorus.lv2/EnsembleChorus.ttl"
      DESTINATION "lib/lv2/EnsembleChorus.lv2")
  else()
    execute_process(COMMAND "${CMAKE_COMMAND}" -E copy
      "${PROJECT_SOURCE_DIR}/resources/lv2/EnsembleChorus.lv2/EnsembleChorus-noui.ttl"
      "${CMAKE_CURRENT_BINARY_DIR}/lv2/EnsembleChorus.lv2/EnsembleChorus.ttl")
    install(FILES "resources/lv2/EnsembleChorus.lv2/EnsembleChorus-noui.ttl"
      DESTINATION "lib/lv2/EnsembleChorus.lv2" RENAME "EnsembleChorus.ttl")
  endif()

  execute_process(COMMAND "${CMAKE_COMMAND}" -E copy
    "${PROJECT_SOURCE_DIR}/resources/lv2/EnsembleChorus.lv2/modgui.ttl"
    "${CMAKE_CURRENT_BINARY_DIR}/lv2/EnsembleChorus.lv2/modgui.ttl")
  execute_process(COMMAND "${CMAKE_COMMAND}" -E copy_directory
    "${PROJECT_SOURCE_DIR}/resources/lv2/EnsembleChorus.lv2/modgui"
    "${CMAKE_CURRENT_BINARY_DIR}/lv2/EnsembleChorus.lv2/modgui")
  install(FILES "resources/lv2/EnsembleChorus.lv2/modgui.ttl"
    DESTINATION "lib/lv2/EnsembleChorus.lv2")
  install(DIRECTORY "resources/lv2/EnsembleChorus.lv2/modgui"
    DESTINATION "lib/lv2/EnsembleChorus.lv2/modgui")
endif()
