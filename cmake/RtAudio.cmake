# RtAudio

include(FindPkgConfig)

add_library(RtAudio STATIC "${RtAudio_SOURCE_DIR}/RtAudio.cpp")
target_include_directories(RtAudio PUBLIC "${RtAudio_SOURCE_DIR}")
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  target_compile_definitions(RtAudio PUBLIC "__LINUX_ALSA__")
  target_link_libraries(RtAudio PUBLIC "asound")
  pkg_check_modules(JACK "jack")
  if(JACK_FOUND)
    target_compile_definitions(RtAudio PUBLIC "__UNIX_JACK__")
    target_include_directories(RtAudio PUBLIC ${JACK_INCLUDE_DIRS})
    link_directories(${JACK_LIBRARY_DIRS})
    target_link_libraries(RtAudio PUBLIC ${JACK_LIBRARIES})
  endif()
  pkg_check_modules(PULSEAUDIO "libpulse-simple")
  if(PULSEAUDIO_FOUND)
    target_compile_definitions(RtAudio PUBLIC "__LINUX_PULSE__")
    target_include_directories(RtAudio PUBLIC ${PULSEAUDIO_INCLUDE_DIRS})
    link_directories(${PULSEAUDIO_LIBRARY_DIRS})
    target_link_libraries(RtAudio PUBLIC ${PULSEAUDIO_LIBRARIES})
  endif()
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  target_compile_definitions(RtAudio PUBLIC "__WINDOWS_DS__")
  target_link_libraries(RtAudio PUBLIC "dsound" "ole32")
  target_compile_definitions(RtAudio PUBLIC "__WINDOWS_WASAPI__")
  target_link_libraries(RtAudio PUBLIC "ksguid")
  target_compile_definitions(RtAudio PUBLIC "__WINDOWS_ASIO__")
  target_include_directories(RtAudio PRIVATE
    "${RtAudio_SOURCE_DIR}/include")
  target_sources(RtAudio PRIVATE
    "${RtAudio_SOURCE_DIR}/include/asio.cpp"
    "${RtAudio_SOURCE_DIR}/include/asiodrivers.cpp"
    "${RtAudio_SOURCE_DIR}/include/asiolist.cpp"
    "${RtAudio_SOURCE_DIR}/include/iasiothiscallresolver.cpp")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  target_compile_definitions(RtAudio PUBLIC "__MACOSX_CORE__")
  find_library(COREAUDIO_LIBRARY "CoreAudio")
  target_link_libraries(RtAudio PUBLIC "${COREAUDIO_LIBRARY}")
endif()

find_package(Threads REQUIRED)
target_link_libraries(RtAudio PUBLIC "${CMAKE_THREAD_LIBS_INIT}")
