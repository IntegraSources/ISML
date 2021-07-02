add_custom_target("${PROJECT_NAME}.docs" ALL
    COMMAND doxygen Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generating project documentation with Doxygen"
    VERBATIM)
