cmake_minimum_required(VERSION 3.16)
project(BTree C)

set(CMAKE_C_STANDARD 99)

add_executable(BTree main.c src/view/App.c src/view/App.h src/handlers/TreeHandler.c src/handlers/TreeHandler.h  src/model/Queue.c src/model/Queue.h src/handlers/DataHandler.c src/handlers/DataHandler.h src/handlers/IndexHandler.c src/handlers/IndexHandler.h)
