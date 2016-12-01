configure_file(testdata.py.in testdata.py @ONLY)
set(BC_PYTHONPATH_EXTRA ${CMAKE_CURRENT_BUILD_DIR})
def_integration_test(process-splitters TestProcessSplitters process_splitters_test.py)
