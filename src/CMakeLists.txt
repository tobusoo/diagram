set(target_name diagram)

add_executable(${target_name})

target_sources(
    ${target_name}
    PRIVATE
    CircleSegment.cpp
    Item.cpp
    ItemSegment.cpp
    CircleDiagram.cpp
    main.cpp
)

target_link_libraries(
  ${target_name}
  PRIVATE
    sfml-graphics
    sfml-system
    sfml-window
    ImGui-SFML::ImGui-SFML
)

IF (WIN32)
  add_custom_target(copy-files ALL
      COMMAND ${CMAKE_COMMAND} -E copy_directory
      ${PROJECT_SOURCE_DIR}/resources
      ${PROJECT_BINARY_DIR}/resources
      COMMENT "COPY RESOURCE FILES"
  )
ENDIF()


target_include_directories( ${target_name} PUBLIC ${CMAKE_SOURCE_DIR}/include)
