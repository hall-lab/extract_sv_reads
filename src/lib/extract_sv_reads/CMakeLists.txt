project(extract_sv_reads)

set(SOURCES
    Alignment.hpp
    AlignmentOffsets.hpp
    AlignmentOffsets.cpp
    Utility.hpp
    Utility.cpp
    Parse.hpp
    Options.hpp
    Options.cpp
    SamReader.hpp
    BamTransform.hpp
)

xadd_library(extract_sv_reads ${SOURCES})
target_link_libraries(extract_sv_reads ${Boost_LIBRARIES})
