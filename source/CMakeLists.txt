set(SOURCES 
    main.cpp
    )

set(HEADERS
    GlobalAND.h
    )

add_executable(parallel_algorithms ${HEADERS} ${SOURCES})

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(parallel_algorithms PUBLIC OpenMP::OpenMP_CXX)
endif()