### grpc and proto compilation for C++ ###
##########################################

file(GLOB proto_files ${CMAKE_CURRENT_LIST_DIR}/*.proto)

add_library(Proto OBJECT ${proto_files})

target_link_libraries(Proto PUBLIC ${_REFLECTION} ${_GRPC_GRPCPP} ${_PROTOBUF_LIBPROTOBUF})

set(PROTO_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}")
set(PROTO_IMPORT_DIRS "${CMAKE_CURRENT_LIST_DIR}")

target_include_directories(Proto PUBLIC "$<BUILD_INTERFACE:${PROTO_BINARY_DIR}>")

protobuf_generate(
    TARGET Proto
    IMPORT_DIRS ${PROTO_IMPORT_DIRS}
    PROTOC_OUT_DIR "${PROTO_BINARY_DIR}")

protobuf_generate(
    TARGET Proto
    LANGUAGE grpc
    GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc
    PLUGIN "protoc-gen-grpc=\$<TARGET_FILE:gRPC::grpc_cpp_plugin>"
    IMPORT_DIRS ${PROTO_IMPORT_DIRS}
    PROTOC_OUT_DIR "${PROTO_BINARY_DIR}")