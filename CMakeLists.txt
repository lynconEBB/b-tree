cmake_minimum_required(VERSION 3.16)
project(BTree C)

set(CMAKE_C_STANDARD 99)

add_executable(BTree main.c src/view/App.c src/view/App.h src/memory/Memory.c src/memory/Memory.h src/model/BTree.h src/model/Professional.h src/controller/TreeController.c src/controller/TreeController.h src/controller/ProfessionalController.c src/controller/ProfessionalController.h)
