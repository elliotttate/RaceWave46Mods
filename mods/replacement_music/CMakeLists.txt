cmake_minimum_required(VERSION 3.20)
project(RaceWaveMusicMod LANGUAGES CXX)
get_filename_component(WR64_ROOT "../.." ABSOLUTE BASE_DIR "${CMAKE_CURRENT_LIST_DIR}")
if(NOT RECOMP_SDK_ROOT)
    message(FATAL_ERROR "Set RECOMP_SDK_ROOT to the RaceWave46 source checkout")
endif()
set(SDL_ROOT "${RECOMP_SDK_ROOT}/lib/rt64/src/contrib/mupen64plus-win32-deps/SDL2-2.26.3")
add_library(racewave_music SHARED native.cpp "${WR64_ROOT}/src/wr64_music.cpp")
target_compile_features(racewave_music PRIVATE cxx_std_20)
target_compile_definitions(racewave_music PRIVATE NOMINMAX WIN32_LEAN_AND_MEAN)
target_include_directories(racewave_music PRIVATE "${WR64_ROOT}/include"
    "${RECOMP_SDK_ROOT}/lib/N64ModernRuntime/N64Recomp/include"
    "${SDL_ROOT}/include" "${RECOMP_SDK_ROOT}/lib/rt64/src/contrib")
target_link_libraries(racewave_music PRIVATE "${SDL_ROOT}/lib/x64/SDL2.lib")
