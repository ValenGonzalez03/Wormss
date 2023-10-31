cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(Worms LANGUAGES CXX)

# add compilation flags
set(CMAKE_CXX_STANDARD 17)

# Agregar un ejecutable llamado "client" y especificar los archivos fuente
add_executable(client ./src/client/client_main.cpp
                        ./src/client/client_client.cpp
                        ./src/client/client_receiver_thread.cpp
                        ./src/client/client_sender_thread.cpp
                        ./src/common/socket.cpp 
	                    ./src/common/liberror.cpp 
	                    ./src/common/resolver.cpp 
	                    ./src/common/resolvererror.cpp
                        ./src/common/protocol.cpp)


target_link_libraries(client pthread)

# define targets
add_executable(server ./src/server/server_main.cpp
                    ./src/server/server_server.cpp
		            ./src/common/socket.cpp 
		            ./src/common/liberror.cpp 
		            ./src/common/resolver.cpp 
		            ./src/common/resolvererror.cpp)
		          
		          
