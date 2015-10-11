project(superbible7)

cmake_minimum_required(VERSION 2.6)

link_directories(${CMAKE_SOURCE_DIR}/lib)

set(CMAKE_MACOSX_RPATH 1) 
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/lib)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/lib)
endforeach(OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES)

find_package(OpenGL)

if (APPLE)
    # For OSX, framework CoreFoundation is required
    find_library(CORE_LIBS CoreFoundation)
endif()

if(NOT CORE_LIBS)
    message(FATAL_ERROR "CoreFoundation not found")
endif()

set(CMAKE_DEBUG_POSTFIX "_d")

if(WIN32)
    set(COMMON_LIBS sb7 optimized glfw3 debug glfw3_d ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES})
elseif(APPLE)
    set(COMMON_LIBS sb7 glfw.3.1) 
elseif(UNIX)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GLFW REQUIRED glfw3)
    set(COMMON_LIBS sb7 glfw3 X11 Xrandr Xinerama Xi Xxf86vm Xcursor GL rt dl)
else()
    set(COMMON_LIBS sb7)
endif()

set(COMMON_LIBS ${COMMON_LIBS} ${EXTRA_LIBS})

add_library(sb7 SHARED
            src/sb7/sb7.cpp
            src/sb7/sb7color.cpp
            src/sb7/sb7ktx.cpp
            src/sb7/sb7object.cpp
            src/sb7/sb7shader.cpp
            src/sb7/sb7textoverlay.cpp
            src/sb7/gl3w.c
            )
target_link_libraries(sb7 ${CORE_LIBS})

set(RUN_DIR ${PROJECT_SOURCE_DIR}/bin)

set(EXAMPLES
    tessellatedtri)

find_package(OpenMP)
if(OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

foreach(EXAMPLE ${EXAMPLES})
    add_executable(${EXAMPLE} MACOSX_BUNDLE src/${EXAMPLE}/${EXAMPLE}.cpp)
    set_property(TARGET ${EXAMPLE} PROPERTY DEBUG_POSTFIX _d)
    target_link_libraries(${EXAMPLE} ${COMMON_LIBS})
    if(APPLE)
        # Use custom plist file in order to support Retina screen
        set_target_properties(${EXAMPLE} PROPERTIES MACOSX_BUNDLE_INFO_PLIST
            ${CMAKE_SOURCE_DIR}/resource/MacOSXBundleInfo.plist.in)
    endif()
    if(MSVC)
        configure_file(${PROJECT_SOURCE_DIR}/build/templates/vs2013.vcxproj.user.in 
                       ${CMAKE_CURRENT_BINARY_DIR}/${EXAMPLE}.vcxproj.user @ONLY)
    endif(MSVC)
endforeach(EXAMPLE)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_LINUX -std=c++0x")
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++")
ENDIF()

include_directories(include)
include_directories(extern/glfw-3.0.4/include)

add_custom_target(debug ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE:STRING=Debug ${PROJECT_SOURCE_DIR})
add_custom_target(release ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE:STRING=Release ${PROJECT_SOURCE_DIR})
