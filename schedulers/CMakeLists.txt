
file(GLOB scheds "*.hpp")

message(STATUS "headers : ${headers}")
add_executable(DUMMY ${headers} ${scheds} "dummy.cpp")
