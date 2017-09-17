#third party packages

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/src/Drivers/SDL2Driver/cmake)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})

#files

file(GLOB EssexEngineSDL2Driver_HEADERS
    "src/Drivers/SDL2Driver/*.h"
)
file(GLOB EssexEngineSDL2Driver_SRC
    ${EssexEngineSDL2Driver_HEADERS}
    "src/Drivers/SDL2Driver/*.cpp"
)

#setup headers

file(COPY ${EssexEngineSDL2Driver_HEADERS} DESTINATION "${CMAKE_BINARY_DIR}/lib/include/EssexEngineSDL2Driver/")

#setup build

add_library(EssexEngineSDL2Driver SHARED ${EssexEngineSDL2Driver_SRC})
target_link_libraries(EssexEngineSDL2Driver ${SDL2_LIBRARY})
target_link_libraries(EssexEngineSDL2Driver ${SDL2_IMAGE_LIBRARY})
target_link_libraries(EssexEngineSDL2Driver ${SDL2_TTF_LIBRARY})
target_link_libraries(EssexEngineSDL2Driver ${OPENGL_LIBRARIES})
target_link_libraries(EssexEngineSDL2Driver EssexEngineCore)
target_link_libraries(EssexEngineSDL2Driver EssexEngineGfxDaemon)
target_link_libraries(EssexEngineSDL2Driver EssexEngineInputDaemon)
target_link_libraries(EssexEngineSDL2Driver EssexEngineSystemDaemon)
