cmake_minimum_required(VERSION 3.10.2 FATAL_ERROR)
project(alWindow CXX)

set(CXX_STANDARD 20)

option(WITH_GEANT4_UIVIS "Build Geant4 with UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
    find_package(Geant4 REQUIRED ui_all vis_all)
else()
    find_package(Geant4 REQUIRED)
endif()

find_package(ROOT REQUIRED)

include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/inc
                    ${ROOT_INCLUDE_DIRS})

file(GLOB sources CMAKE_CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers CMAKE_CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/inc/*.hh)

# Dictionary for the sim data
add_library(EventDict SHARED)
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/detail
)
ROOT_GENERATE_DICTIONARY(
    G_EventDict
    ./inc/detail/RootDictDefs.h
    MODULE 
    EventDict 
    LINKDEF 
    ./inc/detail/LinkDef.h
)
target_link_libraries(
    EventDict 
    PRIVATE 
    ${Geant4_LIBRARIES}
    ${ROOT_LIBRARIES}
)

add_executable(alWindow main.cc ${sources} ${headers})
target_link_libraries(alWindow ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} EventDict)

configure_file(${PROJECT_SOURCE_DIR}/init_vis.mac ${PROJECT_BINARY_DIR}/init_vis.mac COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/vis.mac ${PROJECT_BINARY_DIR}/vis.mac COPYONLY)
