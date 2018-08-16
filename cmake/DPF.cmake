# Disthro plugin framework

include(FindPkgConfig)
pkg_check_modules(JACK "jack")

set(DPF_RESOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/DPF")

add_library(dpf-interface INTERFACE)
target_include_directories(dpf-interface INTERFACE "${DPF_include_dirs}")

include(DGL)
target_compile_definitions(dpf-interface INTERFACE "HAVE_DGL")
target_link_libraries(dpf-interface INTERFACE dgl)

add_library(dpf-internal INTERFACE)
target_include_directories(dpf-internal INTERFACE "${PROJECT_SOURCE_DIR}/thirdparty/DPF/dgl")
target_link_libraries(dpf-internal INTERFACE dpf-interface)

macro(add_dpf_processor NAME)
  add_library("${NAME}" STATIC ${ARGN})
  target_link_libraries("${NAME}" PRIVATE dpf-interface)
endmacro()

macro(add_dpf_ui NAME)
  add_library("${NAME}" STATIC ${ARGN})
  target_link_libraries("${NAME}" PRIVATE dpf-interface)
endmacro()

macro(add_dpf_plugin NAME LIBRARY UI_LIBRARY)
  _dpf_add_vst("${NAME}" "${LIBRARY}" "${UI_LIBRARY}")
  _dpf_add_lv2("${NAME}" "${LIBRARY}" "${UI_LIBRARY}")
  if(JACK_FOUND)
    _dpf_add_jack("${NAME}" "${LIBRARY}" "${UI_LIBRARY}")
  endif()
endmacro()

macro(_dpf_add_module NAME)
  add_library("${NAME}" MODULE ${ARGN})
  set_target_properties("${NAME}" PROPERTIES PREFIX "")
  if(CMAKE_SYSTEM_NAME STREQUAL "Windows" AND MINGW)
    # force the static link of standard libraries
    set_property(TARGET "${NAME}" APPEND_STRING
      PROPERTY LINK_FLAGS " -static-libgcc -static-libstdc++")
    # use a statically link winpthread
    target_link_libraries("${NAME}" PRIVATE "-Wl,-Bstatic,--whole-archive -lwinpthread -Wl,-Bdynamic,--no-whole-archive")
  endif()
endmacro()

macro(_dpf_add_vst NAME LIBRARY UI_LIBRARY)
  _dpf_add_module("${NAME}-VST"
    "${DPF_root_dir}/distrho/DistrhoUIMain.cpp"
    "${DPF_root_dir}/distrho/DistrhoPluginMain.cpp")
  target_compile_definitions("${NAME}-VST" PRIVATE DISTRHO_PLUGIN_TARGET_VST)
  target_link_libraries("${NAME}-VST" PRIVATE "${LIBRARY}" "${UI_LIBRARY}" dpf-internal)
  set_target_properties("${NAME}-VST" PROPERTIES
    OUTPUT_NAME "${NAME}"
    LIBRARY_OUTPUT_DIRECTORY "vst")

  if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_sources("${NAME}-VST" PRIVATE "${DPF_RESOURCE_DIR}/VST2.def")
  endif()

  install(TARGETS "${NAME}-VST" DESTINATION "lib/vst")
endmacro()

macro(_dpf_add_lv2 NAME LIBRARY UI_LIBRARY)
  _dpf_add_module("${NAME}-LV2"
    "${DPF_root_dir}/distrho/DistrhoPluginMain.cpp")
  target_compile_definitions("${NAME}-LV2" PRIVATE DISTRHO_PLUGIN_TARGET_LV2)
  target_link_libraries("${NAME}-LV2" PRIVATE "${LIBRARY}" dpf-internal)
  set_target_properties("${NAME}-LV2" PROPERTIES
    OUTPUT_NAME "${NAME}"
    LIBRARY_OUTPUT_DIRECTORY "lv2/${NAME}.lv2")

  _dpf_add_module("${NAME}-LV2UI"
    "${DPF_root_dir}/distrho/DistrhoUIMain.cpp")
  target_compile_definitions("${NAME}-LV2UI" PRIVATE DISTRHO_PLUGIN_TARGET_LV2)
  target_link_libraries("${NAME}-LV2UI" PRIVATE "${UI_LIBRARY}" dpf-internal)
  set_target_properties("${NAME}-LV2UI" PROPERTIES
    OUTPUT_NAME "${NAME}_ui"
    LIBRARY_OUTPUT_DIRECTORY "lv2/${NAME}.lv2")

  if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_sources("${NAME}-LV2" PRIVATE "${DPF_RESOURCE_DIR}/LV2.def")
    target_sources("${NAME}-LV2UI" PRIVATE "${DPF_RESOURCE_DIR}/LV2UI.def")
  endif()

  if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(LV2_UI_CLASS "ui:WindowsUI")
  elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(LV2_UI_CLASS "ui:CocoaUI")
  else()
    set(LV2_UI_CLASS "ui:X11UI")
  endif()
  set(LV2_BINARY_SUFFIX "${CMAKE_SHARED_MODULE_SUFFIX}")

  configure_file(
    "${PROJECT_SOURCE_DIR}/resources/lv2/${NAME}.lv2/manifest.ttl.in"
    "${CMAKE_CURRENT_BINARY_DIR}/lv2build/${NAME}.lv2/manifest.ttl"
    @ONLY)

  set("${NAME}-LV2-manifests"
    "${PROJECT_SOURCE_DIR}/resources/lv2/${NAME}.lv2/${NAME}.ttl"
    "${CMAKE_CURRENT_BINARY_DIR}/lv2build/${NAME}.lv2/manifest.ttl")
  if(EXISTS "${PROJECT_SOURCE_DIR}/resources/lv2/${NAME}.lv2/presets.ttl")
    list(APPEND "${NAME}-LV2-manifests" "${PROJECT_SOURCE_DIR}/resources/lv2/${NAME}.lv2/presets.ttl")
  endif()
  add_custom_target("${NAME}-LV2-manifests-copy" ALL
    COMMAND "${CMAKE_COMMAND}" "-E" "make_directory" "${CMAKE_CURRENT_BINARY_DIR}/lv2/${NAME}.lv2/"
    COMMAND "${CMAKE_COMMAND}" "-E" "copy" ${${NAME}-LV2-manifests} "${CMAKE_CURRENT_BINARY_DIR}/lv2/${NAME}.lv2/"
    DEPENDS "${${NAME}-LV2-manifests}"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

  install(TARGETS "${NAME}-LV2" "${NAME}-LV2UI" DESTINATION "lib/lv2/${NAME}.lv2")
  install(FILES ${${NAME}-LV2-manifests} DESTINATION "lib/lv2/${NAME}.lv2")
endmacro()

macro(_dpf_add_jack NAME LIBRARY UI_LIBRARY)
  add_executable("${NAME}-JACK"
    "${DPF_root_dir}/distrho/DistrhoUIMain.cpp"
    "${DPF_root_dir}/distrho/DistrhoPluginMain.cpp")
  target_compile_definitions("${NAME}-JACK" PRIVATE DISTRHO_PLUGIN_TARGET_JACK)
  target_include_directories("${NAME}-JACK" PRIVATE "${DPF_include_dirs}" "${JACK_INCLUDE_DIRS}")
  target_link_libraries("${NAME}-JACK" PRIVATE "${LIBRARY}" "${UI_LIBRARY}" dpf-internal ${JACK_LIBRARIES})
  set_target_properties("${NAME}-JACK" PROPERTIES
    OUTPUT_NAME "${NAME}_jack"
    RUNTIME_OUTPUT_DIRECTORY "jack")

  install(TARGETS "${NAME}-JACK" DESTINATION "bin")
endmacro()
