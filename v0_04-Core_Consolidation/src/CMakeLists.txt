set(VERSION_NAME "CoreConsolidation")

#Executable files and their respective headers
add_executable(${VERSION_NAME})
target_sources(${VERSION_NAME}
  PRIVATE
    main.cpp

    OpenGL/camera.cpp
    OpenGL/index_buffer.cpp
    OpenGL/renderer.cpp
    OpenGL/shader.cpp
    OpenGL/texture.cpp
    OpenGL/vertex_array.cpp
    OpenGL/vertex_buffer.cpp

    ThirdParty/glad/glad.c

    World/entity.cpp
    World/mesh.cpp
    World/model.cpp
    World/things.cpp

    Modules/bitmap.cpp
    Modules/font.cpp
    Modules/fontAtlas.cpp
    Modules/imgLoader.cpp

  PUBLIC
    FILE_SET HEADERS
    FILES
      engine_config.h

      OpenGL/camera.h
      OpenGL/index_buffer.h
      OpenGL/renderer.h
      OpenGL/shader.h
      OpenGL/texture.h
      OpenGL/vertex_array.h
      OpenGL/vertex_buffer.h
      OpenGL/vertex_buffer_layout.h

      ThirdParty/glad/glad.h
      ThirdParty/stb_image.h

      World/entity.h
      World/mesh.h
      World/model.h
      World/things.h

      Modules/bitmap.h
      Modules/font.h
      Modules/fontAtlas.h
      Modules/imgLoader.h
)

#Includes folders
target_include_directories(${VERSION_NAME}
  PRIVATE
    ThirdParty
)

#Look for third party packages on linux
add_subdirectory(ThirdParty/glfw-3.4)
target_link_libraries(${VERSION_NAME} PRIVATE glfw)

add_subdirectory(ThirdParty/assimp/)
target_link_libraries(${VERSION_NAME} PRIVATE assimp)

find_package(Freetype)
target_link_libraries(${VERSION_NAME} PRIVATE Freetype::Freetype)

#Copies the resources folder into bin to run the project
add_custom_target(resCopy ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/res"
    "${CMAKE_CURRENT_BINARY_DIR}/res"
    COMMENT "Copying resources into binary directory"
    VERBATIM
)

add_dependencies(${VERSION_NAME} resCopy)