cmake_minimum_required(VERSION 3.6)
project(YoutubeListDownloaderCLI)

include(cmake/conan.cmake)

set(PROJECT_VERSION 1.0)

### Platform dependend
if (MINGW)
    set(CMAKE_EXE_LINKER_FLAGS "-static") # link everything statically so the user does not need to install MinGW
    set(CONAN_SETTINGS compiler.exception=seh compiler.threads=posix)
elseif (ANDROID)
    set(Platform_LINKAGE log)
    set(CMAKE_CXX_COMPILER_VERSION "5.0")
endif ()

### CONAN
# Runs conan install to get dependencies
conan_cmake_run(CONANFILE conanfile.txt
        BASIC_SETUP
        SETTINGS ${CONAN_SETTINGS})

### Youtube-List-Downloader build
add_executable(YoutubeListDownloaderCLI
        src/main.cpp)

target_compile_features(YoutubeListDownloaderCLI PUBLIC cxx_std_11)

target_link_libraries(YoutubeListDownloaderCLI
        PRIVATE ${CONAN_LIBS}
        PRIVATE ${Platform_LINKAGE})

target_include_directories(YoutubeListDownloaderCLI PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CONAN_INCLUDE_DIRS}>)

install(TARGETS YoutubeListDownloaderCLI
        RUNTIME DESTINATION bin)

### For installation of the program
install(TARGETS YoutubeListDownloaderCLI
        RUNTIME DESTINATION bin)

