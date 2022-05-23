SRC= lodepng.cc color.cc matrix2D.cc image.cc morpho.cc
FLAGS=-std=c++2a -Wall -Wextra -I.
OPT_FLAGS=-O3 -DNDEBUG
DEBUG_FLAGS=-g -lasan

final : ${SRC} main.cc
	g++ ${FLAGS} ${OPT_FLAGS} main.cc ${SRC} -o filter

debug : ${SRC} main.cc
	g++ ${FLAGS} ${DEBUG_FLAGS} main.cc ${SRC} -o filter

videofilter : ${SRC} video.cc
	g++ ${FLAGS} ${OPT_FLAGS} $^ -o $@