SRC=lodepng.cc color.cc matrix2D.cc
FLAGS=-std=c++2a -Wall -Wextra -I.
OPT_FLAGS=-o3 -DDEBUG
DEBUG_FLAGS=-g -lasan

debug : ${SRC}
	g++ ${FLAGS} ${DEBUG_FLAGS} main.cc ${SRC} -o filter

final : ${SRC}
	g++ ${FLAGS} ${OPT_FLAGS} main.cc ${SRC} -o filter