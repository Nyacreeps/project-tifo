cmake_minimum_required (VERSION 3.14)

project(VIDEO_FILTER CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra  -std=c++2a -O3 -DNDEBUG")

# SET(OpenCV_DIR ~/Programs/build/)
FIND_PACKAGE( OpenCV REQUIRED )                              
INCLUDE_DIRECTORIES( ${OpenCV_INCLUDE_DIRS} )

add_executable(videofilter)
target_sources(videofilter PRIVATE video.cc)
target_include_directories(videofilter PRIVATE .)
TARGET_LINK_LIBRARIES(videofilter ${OpenCV_LIBS})

add_executable(pngfilter)
target_sources(pngfilter PRIVATE main.cc lodepng.cc color.cc matrix2D.cc image.cc morpho.cc)
target_include_directories(pngfilter PRIVATE .)

add_executable(webcamfilter)
target_sources(webcamfilter PRIVATE webcam.cc lodepng.cc color.cc matrix2D.cc image.cc morpho.cc)
target_include_directories(webcamfilter PRIVATE .)