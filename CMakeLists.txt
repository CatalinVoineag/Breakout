cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)
project(breakout VERSION 1.0.0)

add_executable(breakout
  "main.cpp"

  "Editor/src/Button.cpp"
  "Editor/src/Blocks.cpp"
  "Editor/src/Actor.cpp"
  "Editor/src/ActorTooltip.cpp"
  "Editor/src/Level.cpp"
  "Engine/src/Scene.cpp"

  "Engine/ECS/src/Component.cpp"
  "Engine/ECS/src/CollisionComponent.cpp"
  "Engine/ECS/src/ImageComponent.cpp"
  "Engine/ECS/src/PhysicsComponent.cpp"
  "Engine/ECS/src/InputComponent.cpp"
  "Engine/ECS/src/Commands.cpp"

  # We'll add these later
  # "breakout/src/BreakoutScene.cpp"
  # "breakout/src/Ball.cpp"
)

target_compile_definitions(
  breakout PUBLIC
  WITH_EDITOR
  CHECK_ERRORS

  # We'll use these later
  # DRAW_DEBUG_HELPERS
  # ENABLE_CHEATS
)

target_include_directories(
  breakout PUBLIC
  ${PROJECT_SOURCE_DIR}
)

set(SDLTTF_VENDORED ON)
set(VENDOR_DIR "${PROJECT_SOURCE_DIR}/vendor")
add_subdirectory(${VENDOR_DIR}/SDL)
add_subdirectory(${VENDOR_DIR}/SDL_image)
add_subdirectory(${VENDOR_DIR}/SDL_ttf)

target_link_libraries(breakout PRIVATE
  SDL3::SDL3
  SDL3_image::SDL3_image
  SDL3_ttf::SDL3_ttf
)

set(AssetDirectory "${PROJECT_SOURCE_DIR}/Assets")
add_custom_command(
  TARGET breakout POST_BUILD
  COMMAND
  ${CMAKE_COMMAND} -E copy_if_different
    "$<TARGET_FILE:SDL3::SDL3>"
    "$<TARGET_FILE:SDL3_image::SDL3_image>"
    "$<TARGET_FILE:SDL3_ttf::SDL3_ttf>"
    "$<TARGET_FILE_DIR:breakout>"

  COMMAND
  ${CMAKE_COMMAND} -E copy_directory_if_different
    "${AssetDirectory}"
    "$<TARGET_FILE_DIR:breakout>/Assets"
  VERBATIM
)
