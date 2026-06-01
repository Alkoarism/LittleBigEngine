set(VERSION_NAME "CoreConsolidation")

#Executable files and their respective headers
add_executable(${VERSION_NAME})

target_compile_definitions(${VERSION_NAME}
  PRIVATE
    PROJECT_NAME="${PROJECT_NAME}"
    PROJECT_VERSION_NAME="${VERSION_NAME}"
    VERSION="${PROJECT_VERSION}"
    VERSION_MAJOR="${PROJECT_VERSION_MAJOR}"
    VERSION_MINOR="${PROJECT_VERSION_MINOR}"
    VERSION_PATCH="${PROJECT_VERSION_PATCH}"
)

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
    Modules/assetLoader.cpp

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
      Modules/assetLoader.h
)

#Includes folders
target_include_directories(${VERSION_NAME}
  PRIVATE
    ThirdParty
)

#Look for third party packages on linux
set(THIRDPARTY_DIR "ThirdParty")

add_subdirectory(${THIRDPARTY_DIR}/glfw-3.4)
target_link_libraries(${VERSION_NAME} PRIVATE glfw)

add_subdirectory(${THIRDPARTY_DIR}/assimp)
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