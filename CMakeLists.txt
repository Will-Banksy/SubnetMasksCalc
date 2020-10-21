cmake_minimum_required(VERSION 3.0)

project(SubnetMasks)

set(SourceDir "${PROJECT_SOURCE_DIR}/src")

file(GLOB SourceFiles "${SourceDir}/*.cpp")

add_executable(SubnetMasksExe "${SourceFiles}")

install(TARGETS SubnetMasksExe RUNTIME DESTINATION bin)