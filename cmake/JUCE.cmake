# JUCE library targets for EnsembleChorus

set(JUCE_CMAKE_DIR "${CMAKE_CURRENT_LIST_DIR}/JUCE")

if(NOT JUCE_ROOT_DIR)
  message(FATAL_ERROR "JUCE_ROOT_DIR is not set")
endif()
if(NOT JUCE_PROJECT_DIR)
  message(FATAL_ERROR "JUCE_PROJECT_DIR is not set")
endif()
set(JUCE_LIBRARY_CODE_DIR "${JUCE_PROJECT_DIR}/JuceLibraryCode")
set(JUCE_BUILDS_DIR "${JUCE_PROJECT_DIR}/Builds")

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  set(JUCE_MODULE_SOURCE_SUFFIX ".mm")
else()
  set(JUCE_MODULE_SOURCE_SUFFIX ".cpp")
endif()

macro(add_juce_module NAME)
  set(_sources ${ARGN})
  if(NOT _sources)
    set(_sources "${JUCE_LIBRARY_CODE_DIR}/include_${NAME}${JUCE_MODULE_SOURCE_SUFFIX}")
  endif()
  add_library("${NAME}" STATIC EXCLUDE_FROM_ALL "${_sources}")
  target_include_directories("${NAME}" PUBLIC "${JUCE_ROOT_DIR}/modules")
  set_target_properties("${NAME}" PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
    C_VISIBILITY_PRESET "hidden"
    CXX_VISIBILITY_PRESET "hidden"
    VISIBILITY_INLINES_HIDDEN ON
    POSITION_INDEPENDENT_CODE ON)
  unset(_sources)
endmacro()

add_juce_module(juce_core)
add_juce_module(juce_events)
add_juce_module(juce_data_structures)
add_juce_module(juce_audio_basics)
add_juce_module(juce_audio_devices)
add_juce_module(juce_audio_formats)
add_juce_module(juce_audio_processors)
add_juce_module(juce_audio_utils)
add_juce_module(juce_graphics)
add_juce_module(juce_gui_basics)
add_juce_module(juce_gui_extra)
target_link_libraries(juce_events PUBLIC juce_core)
target_link_libraries(juce_data_structures PUBLIC juce_events)
target_link_libraries(juce_audio_basics PUBLIC juce_core)
target_link_libraries(juce_audio_devices PUBLIC juce_audio_basics juce_events)
target_link_libraries(juce_audio_formats PUBLIC juce_audio_basics)
target_link_libraries(juce_audio_processors PUBLIC juce_gui_extra juce_audio_basics)
target_link_libraries(juce_audio_utils PUBLIC juce_gui_extra juce_audio_processors juce_audio_formats juce_audio_devices)
target_link_libraries(juce_gui_basics PUBLIC juce_graphics juce_data_structures)
target_link_libraries(juce_gui_extra PUBLIC juce_gui_basics)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  # need this circular link dependency on Windows
  target_link_libraries(juce_events PUBLIC juce_gui_extra)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
else()
  # need this circular link dependency on X11
  target_link_libraries(juce_gui_basics PUBLIC juce_gui_extra)
endif()

include(LinkHelpers)
target_link_static_threads(juce_core)

add_library(vst3sdk INTERFACE)
target_include_directories(vst3sdk INTERFACE "${PROJECT_SOURCE_DIR}/thirdparty/vst3sdk")

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  # Windows
  target_link_libraries(juce_core PRIVATE winmm wininet ws2_32 shlwapi version)
  target_link_libraries(juce_gui_basics PRIVATE imm32)
  #
  set(ADLplug_ASIO_SDK "${PROJECT_SOURCE_DIR}/thirdparty/ASIOSDK2.3" CACHE STRING "ASIO SDK location")
  if(NOT EXISTS "${ADLplug_ASIO_SDK}/common/iasiodrv.h")
    message(FATAL_ERROR "ASIO SDK not found in directory ${ADLplug_ASIO_SDK}")
  endif()
  target_include_directories(juce_audio_devices PRIVATE "${ADLplug_ASIO_SDK}/common")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  # Mac
  find_library(FOUNDATION_LIBRARY "Foundation")
  find_library(COCOA_LIBRARY "Cocoa")
  find_library(CORE_SERVICES_LIBRARY "CoreServices")
  find_library(APPLICATION_SERVICES_LIBRARY "ApplicationServices")
  find_library(CORE_AUDIO_LIBRARY "CoreAudio")
  find_library(CORE_MIDI_LIBRARY "CoreMIDI")
  find_library(AUDIO_TOOLBOX_LIBRARY "AudioToolbox")
  find_library(ACCELERATE_LIBRARY "Accelerate")
  find_library(CORE_IMAGE_LIBRARY "CoreImage")
  find_library(IOKIT_LIBRARY "IOKit")
  target_link_libraries(juce_core PRIVATE "${FOUNDATION_LIBRARY}")
  target_link_libraries(juce_core PRIVATE "${COCOA_LIBRARY}")
  target_link_libraries(juce_core PRIVATE "${CORE_SERVICES_LIBRARY}")
  target_link_libraries(juce_core PRIVATE "${APPLICATION_SERVICES_LIBRARY}")
  target_link_libraries(juce_audio_basics PRIVATE "${ACCELERATE_LIBRARY}")
  target_link_libraries(juce_audio_devices PRIVATE "${CORE_AUDIO_LIBRARY}")
  target_link_libraries(juce_audio_devices PRIVATE "${CORE_MIDI_LIBRARY}")
  target_link_libraries(juce_audio_formats PRIVATE "${AUDIO_TOOLBOX_LIBRARY}")
  target_link_libraries(juce_graphics PRIVATE "${CORE_IMAGE_LIBRARY}")
  target_link_libraries(juce_gui_basics PRIVATE "${IOKIT_LIBRARY}")
  if(CMAKE_SYSTEM_PROCESSOR MATCHES "^i.86$" OR
      CMAKE_SYSTEM_PROCESSOR STREQUAL "x86")
    find_library(CARBON_LIBRARY "Carbon")
    target_link_libraries(juce_core PRIVATE "${CARBON_LIBRARY}")
  endif()
else()
  # Linux and others
  include(FindPkgConfig)
  #
  target_link_libraries(juce_core PRIVATE dl)
  #
  find_package(ALSA REQUIRED)
  target_link_libraries(juce_audio_devices PRIVATE ${ALSA_LIBRARIES})
  target_include_directories(juce_audio_devices PRIVATE ${ALSA_INCLUDE_DIRS})
  #
  find_package(Freetype REQUIRED)
  target_link_libraries(juce_graphics PRIVATE ${FREETYPE_LIBRARIES})
  target_include_directories(juce_graphics PRIVATE ${FREETYPE_INCLUDE_DIRS})
  #
  find_package(X11 REQUIRED)
  target_link_libraries(juce_gui_basics PRIVATE ${X11_LIBRARIES})
  target_include_directories(juce_gui_basics PRIVATE ${X11_INCLUDE_DIRS})
  #
  pkg_check_modules(GTK gtk+-3.0 REQUIRED)
  target_link_libraries(juce_gui_extra PRIVATE ${GTK_LIBRARIES})
  target_include_directories(juce_gui_extra PRIVATE ${GTK_INCLUDE_DIRS})
  link_directories(${GTK_LIBRARY_DIRS})
endif()

set(VST2_SOURCES
  "${JUCE_LIBRARY_CODE_DIR}/include_juce_audio_plugin_client_utils.cpp"
  "${JUCE_LIBRARY_CODE_DIR}/include_juce_audio_plugin_client_VST2.cpp")
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  list(APPEND VST2_SOURCES "${JUCE_LIBRARY_CODE_DIR}/include_juce_audio_plugin_client_VST_utils.mm")
endif()
add_juce_module(juce_audio_plugin_client_VST2 ${VST2_SOURCES})
target_link_libraries(juce_audio_plugin_client_VST2 PUBLIC juce_gui_basics juce_audio_basics juce_audio_processors)

set(VST3_SOURCES
  "${JUCE_LIBRARY_CODE_DIR}/include_juce_audio_plugin_client_utils.cpp"
  "${JUCE_LIBRARY_CODE_DIR}/include_juce_audio_plugin_client_VST3.cpp")
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  list(APPEND VST3_SOURCES "${JUCE_LIBRARY_CODE_DIR}/include_juce_audio_plugin_client_VST_utils.mm")
endif()
add_juce_module(juce_audio_plugin_client_VST3 ${VST3_SOURCES})
target_link_libraries(juce_audio_plugin_client_VST3 PUBLIC vst3sdk juce_gui_basics juce_audio_basics juce_audio_processors)

set(LV2_SOURCES
  "${JUCE_LIBRARY_CODE_DIR}/include_juce_audio_plugin_client_utils.cpp"
  "${JUCE_LIBRARY_CODE_DIR}/include_juce_audio_plugin_client_LV2.cpp")
add_juce_module(juce_audio_plugin_client_LV2 ${LV2_SOURCES})
target_link_libraries(juce_audio_plugin_client_LV2 PUBLIC juce_gui_basics juce_audio_basics juce_audio_processors)
target_compile_definitions(juce_audio_plugin_client_LV2
  PUBLIC "JucePlugin_Build_LV2=1"
  PUBLIC "JucePlugin_LV2URI=\"${JUCE_LV2_URI}\"")

set(Standalone_SOURCES
  "${JUCE_LIBRARY_CODE_DIR}/include_juce_audio_plugin_client_utils.cpp"
  "${JUCE_LIBRARY_CODE_DIR}/include_juce_audio_plugin_client_Standalone.cpp")
add_juce_module(juce_audio_plugin_client_Standalone ${Standalone_SOURCES})
target_link_libraries(juce_audio_plugin_client_Standalone PUBLIC juce_gui_basics juce_audio_basics juce_audio_processors)

set(StandaloneCustom_SOURCES
  "${JUCE_LIBRARY_CODE_DIR}/include_juce_audio_plugin_client_utils.cpp"
  "${JUCE_LIBRARY_CODE_DIR}/include_juce_audio_plugin_client_Standalone.cpp")
add_juce_module(juce_audio_plugin_client_StandaloneCustom ${StandaloneCustom_SOURCES})
target_compile_definitions(juce_audio_plugin_client_StandaloneCustom PUBLIC "JUCE_USE_CUSTOM_PLUGIN_STANDALONE_APP=1")
target_link_libraries(juce_audio_plugin_client_StandaloneCustom PUBLIC juce_gui_basics juce_audio_basics juce_audio_processors)

macro(add_juce_standalone NAME)
  add_executable("${NAME}" "${JUCE_CMAKE_DIR}/nothing.cc" ${ARGN})
  if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_sources("${NAME}" PRIVATE "${JUCE_BUILDS_DIR}/VisualStudio2017/resources.rc")
  endif()
  static_link_mingw_crt("${NAME}")
  target_link_libraries("${NAME}" PRIVATE juce_audio_plugin_client_Standalone)
  if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "^i.86$" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "x86")
      require_symbols("${NAME}" "_WinMain@16")
    else()
      require_symbols("${NAME}" "WinMain")
    endif()
  else()
    require_symbols("${NAME}" "main")
  endif()
endmacro()

macro(add_juce_vst NAME)
  add_library("${NAME}" MODULE "${JUCE_CMAKE_DIR}/nothing.cc" ${ARGN})
  target_link_libraries("${NAME}" PRIVATE juce_audio_plugin_client_VST2)
  set_property(TARGET "${NAME}" PROPERTY PREFIX "")
  if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_sources("${NAME}" PRIVATE "${JUCE_BUILDS_DIR}//VisualStudio2017/resources.rc")
  endif()
  static_link_mingw_crt("${NAME}")
  if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "^i.86$" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "x86")
      target_sources("${NAME}" PRIVATE "${JUCE_CMAKE_DIR}/VST2-32.def")
    else()
      target_sources("${NAME}" PRIVATE "${JUCE_CMAKE_DIR}/VST2-64.def")
    endif()
  else()
    require_symbols("${NAME}" "VSTPluginMain")
  endif()
endmacro()

macro(add_juce_lv2 NAME)
  add_library("${NAME}" MODULE "${JUCE_CMAKE_DIR}/nothing.cc" ${ARGN})
  target_link_libraries("${NAME}" PRIVATE juce_audio_plugin_client_LV2)
  set_property(TARGET "${NAME}" PROPERTY PREFIX "")
  if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_sources("${NAME}" PRIVATE "${JUCE_BUILDS_DIR}//VisualStudio2017/resources.rc")
  endif()
  static_link_mingw_crt("${NAME}")
  if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_sources("${NAME}" PRIVATE "${JUCE_CMAKE_DIR}/LV2.def")
  else()
    require_symbols("${NAME}" "lv2_generate_ttl" "lv2_descriptor" "lv2ui_descriptor")
  endif()
endmacro()
