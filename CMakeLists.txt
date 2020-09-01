cmake_minimum_required(VERSION 3.15)
project(bars)
set(CMAKE_CXX_STANDARD 14)


find_path(INCLUDE_DIR NAMES SDL2/SDL.h)
message("FOUND SDL2 include dir = ${INCLUDE_DIR}")

include_directories(${INCLUDE_DIR}/SDL2)
include_directories(/Library/Frameworks/SDL2.framework/Headers)
include_directories(/Library/Frameworks/SDL2_image.framework/Headers)
include_directories(/Library/Frameworks/SDL2_ttf.framework/Headers)



# include_directories(/usr/include/SDL2)

find_library(SDA2_LIB SDL2)
find_library(SDA2_TTF_LIB SDL2_ttf)
find_library(SDA2_IMG_LIB SDL2_image)


add_executable(bars main.cpp LTexture.cpp LTexture.h)

target_link_libraries(bars PRIVATE "${SDA2_LIB}")
target_link_libraries(bars PRIVATE "${SDA2_TTF_LIB}")
target_link_libraries(bars PRIVATE "${SDA2_IMG_LIB}")

