get_filename_component(SELF_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
include(${SELF_DIR}/${CMAKE_BUILD_TYPE}/src/CMakeLists.txt)
install(FILES ccl-config.cmake DESTINATION ${ccl_lib_dest})
install(EXPORT ccl DESTINATION "${ccl_lib_dest}")
