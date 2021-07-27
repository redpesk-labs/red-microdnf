cmake_minimum_required(VERSION 3.18)
project(redmicrodnf CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DPROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}")

find_package(Threads)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBFMT REQUIRED fmt)
pkg_check_modules(SMARTCOLS REQUIRED smartcols)
pkg_check_modules(REDPAK REQUIRED red-pak)
pkg_check_modules(LIBDNF REQUIRED libdnf5)
pkg_check_modules(LIBDNFCLI REQUIRED libdnf5-cli)

include_directories(${REDPAK_INCLUDE_DIRS})
include_directories(${LIBDNF_INCLUDE_DIRS})
include_directories(${LIBDNFCLI_INCLUDE_DIRS})

# set gettext domain for translations
add_definitions(-DGETTEXT_DOMAIN=\"microdnf\")

# use any sources found under the current directory
file(GLOB_RECURSE MICRODNF_SOURCES commands/*.cpp)

add_executable(redmicrodnf main.cpp context.cpp utils.cpp ${MICRODNF_SOURCES})

target_link_libraries(redmicrodnf PUBLIC ${LIBDNF_LIBRARIES} ${LIBDNFCLI_LIBRARIES} stdc++ stdc++fs Threads::Threads ${REDPAK_LIBRARIES} ${LIBFMT_LIBRARIES} ${SMARTCOLS_LIBRARIES})
install(TARGETS redmicrodnf RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})


find_package(bash-completion)
if(BASH_COMPLETION_FOUND)
  message(STATUS "Using bash completion dir ${BASH_COMPLETION_COMPLETIONSDIR}")
else()
  set (BASH_COMPLETION_COMPLETIONSDIR "/etc/bash_completion.d")
  message (STATUS "Using fallback bash completion dir ${BASH_COMPLETION_COMPLETIONSDIR}")
endif()

install(FILES bash-completion/microdnf DESTINATION ${BASH_COMPLETION_COMPLETIONSDIR})
