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

target_include_directories( ${target_name} PUBLIC ${CMAKE_SOURCE_DIR}/include)
