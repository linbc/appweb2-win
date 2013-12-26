#cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(appweb)

# CMake policies (can not be handled elsewhere)
cmake_minimum_required(VERSION 2.6)
cmake_policy(SET CMP0005 OLD)

add_definitions(-DWIN)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

add_definitions(-DBLD_FEATURE_CONFIG_PARSE)
#add_definitions(-DBLD_FEATURE_CONFIG_SAVE)

#add_definitions(-DBLD_FEATURE_ADMIN_MODULE)
#add_definitions(-DBLD_FEATURE_AUTH_MODULE)
add_definitions(-DBLD_FEATURE_COPY_MODULE)
#add_definitions(-DBLD_FEATURE_CGI_MODULE)
add_definitions(-DBLD_FEATURE_DIR_MODULE)
#add_definitions(-DBLD_FEATURE_EGI_MODULE)
#add_definitions(-DBLD_FEATURE_EJS)
#add_definitions(-DBLD_FEATURE_ESP_MODULE)
#add_definitions(-DBLD_FEATURE_SSL_MODULE)
#add_definitions(-DBLD_FEATURE_UPLOAD_MODULE)
#add_definitions(-DBLD_FEATURE_PUT_MODULE)
#add_definitions(-DBLD_FEATURE_C_API_MODULE)
#add_definitions(-DBLD_FEATURE_GACOMPAT_MODULE)

#add_definitions(-DBLD_FEATURE_SESSION)
add_definitions(-DBLD_FEATURE_KEEP_ALIVE)
#add_definitions(-DBLD_FEATURE_ACCESS_LOG)



include_directories(
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/mpr  
  ${CMAKE_SOURCE_DIR}/http
  ${CMAKE_SOURCE_DIR}/http/modules
  ${CMAKE_SOURCE_DIR}/http/modules/gaCompat
)
LINK_DIRECTORIES(
  ${CMAKE_BINARY_DIR}/lib
)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)


add_subdirectory(http)
add_subdirectory(mpr)
add_subdirectory(simpleServer)
#add_subdirectory(appweb)
