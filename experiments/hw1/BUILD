package(default_visibility = ["//visibility:public"])

load("@rules_proto//proto:defs.bzl", "proto_library")
load("//bazel:cc_grpc_library.bzl", "cc_grpc_library")
load("//bazel:grpc_build_system.bzl", "grpc_proto_library")

proto_library(
    name = "hw1_proto",
    srcs = ["hw1.proto"],
)

cc_proto_library(
    name = "hw1_cc_proto",
    deps = [":hw1_proto"],
)

cc_grpc_library(
    name = "hw1_cc_grpc",
    srcs = [":hw1_proto"],
    grpc_only = True,
    deps = [":hw1_cc_proto"],
)

cc_binary(
    name = "hw1_client",
    srcs = ["hw1_client_main.cc"],
    deps = [
        ":hw1_cc_grpc",
        "//:grpc++",
    ],
)
