cmake_minimum_required (VERSION 3.0)

set (project_name Xer)

project (${project_name})

set (version_major 0)
set (version_minor 0)
set (version_patch 1)

set (root_dir ${PROJECT_SOURCE_DIR})
set (include_dir ${root_dir}/include)
set (source_dir ${root_dir}/src)

set (program_srcs 
  ${source_dir}/gui.c
  ${source_dir}/main.c
  ${source_dir}/player.c
  ${source_dir}/guiutils.c
  ${source_dir}/mapgen.c
  ${source_dir}/standard_objects.c
  ${source_dir}/logic.c
  ${source_dir}/monsters.c
  ${source_dir}/utils.c
  )

configure_file (
  ${root_dir}/GlobalConfig.h.in
  ${include_dir}/GlobalConfig.h
  )



set (gcc_flags 
  -Wextra
  -Wall
  -Wfloat-equal
  -Wundef
  -Wshadow
  -Wpointer-arith
  -Wcast-align
  -Wstrict-prototypes
  -Wwrite-strings
  -Waggregate-return
  )

add_definitions (${gcc_flags})

add_executable (${project_name} ${program_srcs})

find_package (Curses REQUIRED)
include_directories (${include_dir} ${CURSES_INCLUDE_DIRS})
target_link_libraries (${project_name} ${CURSES_LIBRARIES})
