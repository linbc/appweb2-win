file(GLOB sources_simpleServer *.cpp *.h  *.conf)
add_executable(simpleServer ${sources_simpleServer})
target_link_libraries(simpleServer http mpr)
