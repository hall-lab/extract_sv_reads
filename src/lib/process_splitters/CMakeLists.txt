project(process_splitters)

set(SOURCES
    Alignment.hpp
    AlignmentOffsets.hpp
    AlignmentOffsets.cpp
    Utility.hpp
    Utility.cpp
    Parse.hpp
    Options.hpp
    Options.cpp
)

xadd_library(process_splitters ${SOURCES})
target_link_libraries(process_splitters ${Boost_LIBRARIES})
