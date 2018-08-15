# Disthro plugin framework

add_library(dpf-interface INTERFACE)
target_include_directories(dpf-interface INTERFACE "${DPF_include_dirs}")

include(DGL)
target_compile_definitions(dpf-interface INTERFACE "HAVE_DGL")
target_link_libraries(dpf-interface INTERFACE dgl)

add_library(dpf-internal INTERFACE)
target_include_directories(dpf-internal INTERFACE "${PROJECT_SOURCE_DIR}/thirdparty/DPF/dgl")
target_link_libraries(dpf-internal INTERFACE dpf-interface)

macro(add_dpf_plugin NAME LIBRARY UI_LIBRARY)
  target_link_libraries("${LIBRARY}" PRIVATE dpf-interface)
  target_link_libraries("${UI_LIBRARY}" PRIVATE dpf-interface)

  _dpf_add_vst("${NAME}" "${LIBRARY}" "${UI_LIBRARY}")
  _dpf_add_lv2("${NAME}" "${LIBRARY}" "${UI_LIBRARY}")

  include(FindPkgConfig)
  pkg_check_modules(JACK "jack")
  if(JACK_FOUND)
    _dpf_add_jack("${NAME}" "${LIBRARY}" "${UI_LIBRARY}")
  endif()
endmacro()

macro(_dpf_add_vst NAME LIBRARY UI_LIBRARY)
  add_library("${NAME}-VST" MODULE
    "${DPF_root_dir}/distrho/DistrhoUIMain.cpp"
    "${DPF_root_dir}/distrho/DistrhoPluginMain.cpp")
  target_compile_definitions("${NAME}-VST" PRIVATE DISTRHO_PLUGIN_TARGET_VST)
  target_link_libraries("${NAME}-VST" PRIVATE "${LIBRARY}" "${UI_LIBRARY}" dpf-internal)
  set_target_properties("${NAME}-VST" PROPERTIES
    PREFIX ""
    OUTPUT_NAME "${NAME}"
    LIBRARY_OUTPUT_DIRECTORY "vst")

  install(TARGETS "${NAME}-VST" DESTINATION "lib/vst")
endmacro()

macro(_dpf_add_lv2 NAME LIBRARY UI_LIBRARY)
  add_library("${NAME}-LV2" MODULE
    "${DPF_root_dir}/distrho/DistrhoPluginMain.cpp")
  target_compile_definitions("${NAME}-LV2" PRIVATE DISTRHO_PLUGIN_TARGET_LV2)
  target_link_libraries("${NAME}-LV2" PRIVATE "${LIBRARY}" dpf-internal)
  set_target_properties("${NAME}-LV2" PROPERTIES
    PREFIX ""
    OUTPUT_NAME "${NAME}"
    LIBRARY_OUTPUT_DIRECTORY "lv2/${NAME}.lv2")

  add_library("${NAME}-LV2UI" MODULE
    "${DPF_root_dir}/distrho/DistrhoUIMain.cpp")
  target_compile_definitions("${NAME}-LV2UI" PRIVATE DISTRHO_PLUGIN_TARGET_LV2)
  target_link_libraries("${NAME}-LV2UI" PRIVATE "${UI_LIBRARY}" dpf-internal)
  set_target_properties("${NAME}-LV2UI" PROPERTIES
    PREFIX ""
    OUTPUT_NAME "${NAME}_ui"
    LIBRARY_OUTPUT_DIRECTORY "lv2/${NAME}.lv2")

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
