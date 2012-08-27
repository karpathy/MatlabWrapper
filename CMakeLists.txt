cmake_minimum_required (VERSION 2.6)
project (MatlabWrapper)

# includes MATLAB dependenices. You may have to modify this based on
# your installation directories, exactly :(
set(MATLAB_LIBS eng mx m)
set(MATLAB_INCLUDES /usr/local/MATLAB/R2011b/extern/include /usr/local/MATLAB/R2011b/simulink/include /usr/local/MATLAB/R2011b/extern/include/cpp /usr/local/MATLAB/R2011b/extern/include)
set(MATLAB_LIB_PATHS /usr/local/MATLAB/R2011b/bin/glnxa64)
set(MATLAB_DEFS -DMATLAB_MEX_FILE -D_GNU_SOURCE -DGLNXA64 -DGCC -DMX_COMPAT_32 -DNDEBUG)
include_directories(${MATLAB_INCLUDES})
link_directories(${MATLAB_LIB_PATHS})
add_definitions(${MATLAB_DEFS})

# eigen library dependency. May have to change this too depending on where
# your eigen is. Good news is that eigen is just a bunch of header files,
# no library linking needed!
include_directories(/usr/local/include/eigen3)

# specify our library
add_library(mwlib src/MatlabWrapper.cpp)
include_directories(${PROJECT_SOURCE_DIR}/include)

# the provided test example of usage
add_executable (test1 test/test1.cpp)
target_link_libraries (test1 mwlib ${MATLAB_LIBS})
