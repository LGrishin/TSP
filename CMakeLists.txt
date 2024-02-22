cmake_minimum_required(VERSION 2.6)

set(HEADERS
    commons.h
    core.h
    data_model.h
    parser.h
    svg_dumper.h
    approximation.h
    milp_solver.h
    )

set(SOURCES
    main.cpp
    parser.cpp
    svg_dumper.cpp
    core.cpp
    approximation.cpp
    milp_solver.cpp
    )

file(COPY data DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

project(TSP)
find_package(glpk)
find_package(BOOST 1.71)
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

add_executable(run ${SOURCES})
target_link_libraries(run LINK_PUBLIC glpk ${Boost_LIBRARIES})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -Wall")
