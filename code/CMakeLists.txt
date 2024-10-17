add_executable(${PROJECT_NAME})

add_custom_target(run 
    COMMAND ${PROJECT_NAME}
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/build"
)

#target_precompile_headers(${PROJECT_NAME}  PRIVATE private.hpp)
#target_link_libraries(${PROJECT_NAME}      PRIVATE engine_platform)
#target_include_directories(${PROJECT_NAME} PRIVATE code)

file(GLOB SOURCES *.cpp)

#set(SOURCES
#    main.cpp
#    example.cpp
#)
#message(STATUS "The value of SOURCES is: ${SOURCES}")

target_sources(${PROJECT_NAME}            PRIVATE ${SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/build"
                                                 RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/build")  