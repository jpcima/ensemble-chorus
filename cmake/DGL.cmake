# Disthro plugin framework - DGL library

set(DGL_sources
  "${DPF_root_dir}/dgl/src/Application.cpp"
  "${DPF_root_dir}/dgl/src/Color.cpp"
  "${DPF_root_dir}/dgl/src/Geometry.cpp"
  "${DPF_root_dir}/dgl/src/Image.cpp"
  "${DPF_root_dir}/dgl/src/ImageWidgets.cpp"
  "${DPF_root_dir}/dgl/src/NanoVG.cpp"
  "${DPF_root_dir}/dgl/src/Resources.cpp"
  "${DPF_root_dir}/dgl/src/Widget.cpp")
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  list(APPEND DGL_sources "${DPF_root_dir}/dgl/src/Window.mm")
else()
  list(APPEND DGL_sources "${DPF_root_dir}/dgl/src/Window.cpp")
endif()

add_library(dgl STATIC EXCLUDE_FROM_ALL ${DGL_sources})
target_include_directories(dgl
  PUBLIC thirdparty/DPF
  PRIVATE thirdparty/DPF/dgl/src)

set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED)
target_link_libraries(dgl PUBLIC ${OPENGL_LIBRARIES})

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
else()
  find_package(X11 REQUIRED)
  target_link_libraries(dgl PUBLIC ${X11_LIBRARIES})
endif()
