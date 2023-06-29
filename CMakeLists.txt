cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(muons_simulation)

find_package(Geant4 REQUIRED ui_all vis_all)

include(${Geant4_USE_FILE})

file(GLOB sources ${PROJECT_SOURCE_DIR}/*.cc)
file(GLOB sources ${PROJECT_SOURCE_DIR}/*.hh)

add_executable(mu mu.cc ${sources} ${headers} construction.cc physics.cc action.hh action.cc generator.hh generator.cc detector.cc detector.hh)
target_link_libraries(mu ${Geant4_LIBRARIES})

add_custom_target(muons_simulation DEPENDS mu)

