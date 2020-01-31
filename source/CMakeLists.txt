find_package(Eigen3)
if(Eigen3_FOUND)
    include_directories(${Eigen3_INCLUDE_DIR})
endif()

find_package(MPI)
if (MPI_CXX_FOUND)
    include_directories(${MPI_INCLUDE_DIR})
endif()

set(SOURCES 
    main.cpp
    )

set(HEADERS
    ./LinearAlgebra/MatrixMultiplication.h

    ./PRAM-CRCW/GlobalAND.h
    ./PRAM-CRCW/GlobalOR.h
    ./PRAM-CRCW/Maximum.h
    )

add_executable(parallel_algorithms ${HEADERS} ${SOURCES})

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(parallel_algorithms PUBLIC OpenMP::OpenMP_CXX)
endif()
