project(process_splitters)

set(SOURCES
    AlignmentOffsets.hpp
    AlignmentOffsets.cpp
    Utility.hpp
    Parse.hpp
)

xadd_library(process_splitters ${SOURCES})
target_link_libraries(process_splitters ${Boost_LIBRARIES})
