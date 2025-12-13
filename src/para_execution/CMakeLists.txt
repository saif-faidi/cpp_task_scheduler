set(LIBRARY_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/para_execution.cc")
set(LIBRARY_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/para_execution.h")
set(LIBRARY_INCLUDES "./" "${CMAKE_BINARY_DIR}/configured_files/include")



add_library( ${LIBRARY_PARA_EXEC_NAME} STATIC ${LIBRARY_SOURCES} ${LIBRARY_HEADERS})
target_include_directories( ${LIBRARY_PARA_EXEC_NAME} PUBLIC ${LIBRARY_INCLUDES})
target_link_libraries(
        ${LIBRARY_PARA_EXEC_NAME}
        PRIVATE 
        spdlog::spdlog
        )

if(${ENABLE_WARNINGS})
    target_set_warnings(
        TARGET
         ${LIBRARY_PARA_EXEC_NAME}
        ENABLE
        ${ENABLE_WARNINGS}
        AS_ERRORS
        ${ENABLE_WARNINGS_AS_ERRORS})
endif()

if(${ENABLE_LTO})
    target_enable_lto(
        TARGET
         ${LIBRARY_PARA_EXEC_NAME}
        ENABLE
        ON)
endif()

if(${ENABLE_CLANG_TIDY})
    add_clang_tidy_to_target( ${LIBRARY_PARA_EXEC_NAME})
endif()
