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

  add_library("${NAME}-VST" MODULE
    "${DPF_root_dir}/distrho/DistrhoUIMain.cpp"
    "${DPF_root_dir}/distrho/DistrhoPluginMain.cpp")
  target_compile_definitions("${NAME}-VST" PRIVATE DISTRHO_PLUGIN_TARGET_VST)
  target_link_libraries("${NAME}-VST" PRIVATE "${LIBRARY}" "${UI_LIBRARY}" dpf-internal)

  add_library("${NAME}-LV2" MODULE
    "${DPF_root_dir}/distrho/DistrhoPluginMain.cpp")
  target_compile_definitions("${NAME}-LV2" PRIVATE DISTRHO_PLUGIN_TARGET_LV2)
  target_link_libraries("${NAME}-LV2" PRIVATE "${LIBRARY}" dpf-internal)

  add_library("${NAME}-LV2UI" MODULE
    "${DPF_root_dir}/distrho/DistrhoUIMain.cpp")
  target_compile_definitions("${NAME}-LV2UI" PRIVATE DISTRHO_PLUGIN_TARGET_LV2)
  target_link_libraries("${NAME}-LV2UI" PRIVATE "${UI_LIBRARY}" dpf-internal)

  include(FindPkgConfig)
  pkg_check_modules(JACK "jack")
  if(JACK_FOUND)
    add_executable("${NAME}-JACK"
      "${DPF_root_dir}/distrho/DistrhoUIMain.cpp"
      "${DPF_root_dir}/distrho/DistrhoPluginMain.cpp")
    target_compile_definitions("${NAME}-JACK" PRIVATE DISTRHO_PLUGIN_TARGET_JACK)
    target_include_directories("${NAME}-JACK" PRIVATE "${DPF_include_dirs}" "${JACK_INCLUDE_DIRS}")
    target_link_libraries("${NAME}-JACK" PRIVATE "${LIBRARY}" "${UI_LIBRARY}" dpf-internal ${JACK_LIBRARIES})
  endif()
endmacro()
