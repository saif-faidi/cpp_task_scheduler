set(LIBRARY_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/thread_pool.cc")
set(LIBRARY_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/thread_pool.h")
set(LIBRARY_INCLUDES "./" "${CMAKE_BINARY_DIR}/configured_files/include")



add_library( ${LIBRARY_THREAD_POOL_NAME} STATIC ${LIBRARY_SOURCES} ${LIBRARY_HEADERS})
target_include_directories( ${LIBRARY_THREAD_POOL_NAME} PUBLIC ${LIBRARY_INCLUDES})
target_link_libraries(
        ${LIBRARY_THREAD_POOL_NAME}
        PRIVATE 
        spdlog::spdlog
        )

if(${ENABLE_WARNINGS})
    target_set_warnings(
        TARGET
         ${LIBRARY_THREAD_POOL_NAME}
        ENABLE
        ${ENABLE_WARNINGS}
        AS_ERRORS
        ${ENABLE_WARNINGS_AS_ERRORS})
endif()

if(${ENABLE_LTO})
    target_enable_lto(
        TARGET
         ${LIBRARY_THREAD_POOL_NAME}
        ENABLE
        ON)
endif()

if(${ENABLE_CLANG_TIDY})
    add_clang_tidy_to_target( ${LIBRARY_THREAD_POOL_NAME})
endif()
