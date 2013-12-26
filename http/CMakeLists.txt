
#file(GLOB_RECURSE sources_server Server/*.cpp Server/*.h)
file(GLOB sources_localdir *.cpp *.h *.c)
file(GLOB sources_modules modules/*.cpp modules/*.h modules/openSsl/*.cpp modules/openSsl/*.h)
file(GLOB src_utils utils/*.cpp utils/*.h utils/*.c)

set(http_STAT_SRCS
   ${sources_localdir}
   ${sources_modules}
   ${src_utils}
)

set(http_LINK_FLAGS "")

add_library(http STATIC ${http_STAT_SRCS})

include_directories(
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/
  ${CMAKE_SOURCE_DIR}/esp
  ${CMAKE_SOURCE_DIR}/ejs
  ${CMAKE_SOURCE_DIR}/mpr
  ${CMAKE_SOURCE_DIR}/http
  ${CMAKE_CURRENT_SOURCE_DIR}
)

if( UNIX )
  set(gzdbsvr_LINK_FLAGS "-pthread ${http_LINK_FLAGS}")
endif()

set_target_properties(http PROPERTIES LINK_FLAGS "${http_LINK_FLAGS}")

target_link_libraries(http)


