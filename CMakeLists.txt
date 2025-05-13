cmake_minimum_required(VERSION 2.8.3)
project(coverage_test)
add_compile_options(-std=c++11)

message(STATUS ${ROOTPATH})

# find_package(Gflags)
# set(OpenCV_DIR "/home/wz/github/install/opencv4.2.0/lib/cmake/opencv4")
find_package(OpenCV REQUIRED)

include_directories(histogram)
#add lib
file(GLOB_RECURSE COV_SRC ${ROOTPATH}/coverage_planners/*.cpp)

#add executable
add_executable(histogram_stretch histogram_stretch.cpp histogram/histogram.cpp)
target_link_libraries(histogram_stretch 
    ${OpenCV_LIBS}
    )

add_executable(compare compare.cpp)
target_link_libraries(compare 
    ${OpenCV_LIBS}
    )

# add_executable(match match.cpp)
# target_link_libraries(match 
#     ${OpenCV_LIBS}
# )
# add_executable(detect detect_feature.cpp)
# target_link_libraries(detect 
#     ${OpenCV_LIBS}
#     )

# add_executable(rectangle rectangle.cpp)
# target_link_libraries(rectangle 
#     ${OpenCV_LIBS}
# )

# add_executable(test_inhert test_inhert.cpp)
# target_link_libraries(test_inhert 
#     ${OpenCV_LIBS}
# )
    