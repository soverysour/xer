cmake_minimum_required (VERSION 3.0)

set (project_name Xer)

project (${project_name})

set (${project_name}_version_major 0)
set (${project_name}_version_minor 0)
set (${project_name}_version_patch 1)

set (root_dir ${PROJECT_SOURCE_DIR})
set (include_dir ${PROJECT_SOURCE_DIR}/include)
set (source_dir ${PROJECT_SOURCE_DIR}/src)

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

add_executable (${project_name} ${program_srcs})

find_package(Curses REQUIRED)
include_directories(${include_dir} ${CURSES_INCLUDE_DIRS})
target_link_libraries(${project_name} ${CURSES_LIBRARIES})
