cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(muons_simulation)

find_package(Geant4 REQUIRED ui_all vis_all)

include(${Geant4_USE_FILE})

add_executable(
        mu
        mu.cc
        src/construction.cc
        src/construction.hh
        src/detector.cc
        src/detector.hh
        src/generator.cc
        src/generator.hh
        src/action.cc
        src/action.hh
        src/physics.cc
        src/physics.hh
        src/steppingAction.hh
        src/steppingAction.cc
)
target_link_libraries(mu ${Geant4_LIBRARIES})

add_custom_target(muons_simulation DEPENDS mu)

