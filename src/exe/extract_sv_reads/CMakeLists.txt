project(extract_sv_reads)

set(SOURCES
    extract_sv_reads.cpp
)

set(EXECUTABLE_NAME extract-sv-reads)
xadd_executable(${EXECUTABLE_NAME} ${SOURCES})
target_link_libraries(${EXECUTABLE_NAME}
    extract_sv_reads
    ${HTSlib_LIBRARIES}
    ${Boost_LIBRARIES})
set_target_properties(${EXECUTABLE_NAME} PROPERTIES PACKAGE_OUTPUT_NAME ${EXECUTABLE_NAME}${EXE_VERSION_SUFFIX})
install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin/)
