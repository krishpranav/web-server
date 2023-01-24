cmake_minimum_required(VERSION 3.0)

project(web-server)

add_executable(${PROJECT_NAME}
  web-server/server.c
  web-server/socket.c)

add_executable(webserver
  web-server/client.c
  web-server/socket.c
  web-server/utils.c)

if(WIN32)
  target_link_libraries(${PROJECT_NAME} ws2_32)
  target_link_libraries(web-server ws2_32)
endif()
