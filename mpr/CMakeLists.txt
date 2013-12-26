

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /wd4996 /wd4805")

file(GLOB src_dirs *.c *.h *.cpp)
file(GLOB WIN_dirs WIN/*.cpp WIN/*.h)
file(GLOB UNIX_dirs UNIX/*.cpp UNIX/*.h)

set(mpr_STAT_SRCS
   ${src_dirs}
   ${WIN_dirs}
)

include_directories(
	${CMAKE_SOURCE_DIR}  
	${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(mpr STATIC ${mpr_STAT_SRCS})
SET_TARGET_PROPERTIES(mpr PROPERTIES LINKER_LANGUAGE C)

