add_library(stb_image)

target_sources(stb_image
  PRIVATE
    stb_image.cpp

  PUBLIC
    FILE_SET HEADERS
    FILES
      stb_image.h
)
