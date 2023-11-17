FIND_PACKAGE(OpenMP REQUIRED)

if (APPLE)
    # for my development env
#    set(CMAKE_CXX_COMPILER /usr/bin/c++)
    set(CMAKE_CXX_COMPILER /Library/Developer/CommandLineTools/usr/bin/c++)
    set(OPENMP_LIBRARIES "/opt/homebrew/opt/libomp/lib")
    set(OPENMP_INCLUDES "/opt/homebrew/opt/libomp/include")
    set(OpenMP_CXX "${CMAKE_CXX_COMPILER}")
    set(OpenMP_CXX_FLAGS "-lomp -Wno-unused-command-line-argument")
    set(OpenMP_CXX_LIB_NAMES "libomp" "libgomp" "libiomp5")
    set(OpenMP_libomp_LIBRARY ${OpenMP_CXX_LIB_NAMES})
    set(OpenMP_libgomp_LIBRARY ${OpenMP_CXX_LIB_NAMES})
    set(OpenMP_libiomp5_LIBRARY ${OpenMP_CXX_LIB_NAMES})
    if (OPENMP_FOUND)
        include_directories("${OPENMP_INCLUDES}")
        link_directories("${OPENMP_LIBRARIES}")
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    endif(OPENMP_FOUND)
else()
    # set(OpenMP_CXX_FLAGS "-fopenmp")
    if (OPENMP_FOUND)
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    endif(OPENMP_FOUND)
endif(APPLE)
