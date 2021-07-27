cmake_minimum_required(VERSION 3.18)
project(redmicrodnf CXX)

find_package(Threads)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBFMT REQUIRED fmt)
pkg_check_modules(SMARTCOLS REQUIRED smartcols)
pkg_check_modules(REDPAK REQUIRED red-pak)

# set gettext domain for translations
add_definitions(-DGETTEXT_DOMAIN=\"microdnf\")

# use any sources found under the current directory
file(GLOB_RECURSE MICRODNF_SOURCES *.cpp)

add_executable(redmicrodnf ${MICRODNF_SOURCES})

target_link_libraries(redmicrodnf PUBLIC libdnf5 libdnf5-cli stdc++ stdc++fs Threads::Threads redlib)
install(TARGETS redmicrodnf RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})


find_package(bash-completion)
if(BASH_COMPLETION_FOUND)
  message(STATUS "Using bash completion dir ${BASH_COMPLETION_COMPLETIONSDIR}")
else()
  set (BASH_COMPLETION_COMPLETIONSDIR "/etc/bash_completion.d")
  message (STATUS "Using fallback bash completion dir ${BASH_COMPLETION_COMPLETIONSDIR}")
endif()

install(FILES bash-completion/microdnf DESTINATION ${BASH_COMPLETION_COMPLETIONSDIR})
