
set(sudok_src
  main.cpp
)

add_executable(sudok  ${sudok_src})
# target_link_libraries(sudok
#   PRIVATE
#   fmt::fmt-header-only
# )

target_compile_features(sudok PRIVATE cxx_std_20)

# target_compile_options(sudok PRIVATE -Wall -Wextra)
# target_compile_options(sudok PRIVATE -fsanitize=address)
# target_link_options(sudok PRIVATE -fsanitize=address)


# if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" AND WITH_SDL)
#   # copy dll from libraries into destination folder:
#   add_custom_command(TARGET sudok
#     POST_BUILD COMMAND ${CMAKE_COMMAND} -E
#     copy_if_different
#     $<TARGET_FILE:some::lib>
#     $<TARGET_FILE_DIR:sudok>)
# endif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" AND WITH_SDL)

