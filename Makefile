COMPILER = g++
OUTPUT = ticky
TEST_OUTPUT = test_ticky
LIBS = -lglad -lglfw3 -lX11 -lpthread -ldl

SOURCES = ${wildcard ./src/*.cpp}

# TODO: CREATE A WILDCARD HERE
# +++++++++++++++++++++++++++++++++++++
SOURCE_WO_MAIN = ./src/board.cpp  ./src/controller.cpp ./src/view_terminal.cpp ./src/view_opengl.cpp ./src/stb_image.cpp
# +++++++++++++++++++++++++++++++++++++

TEST_SOURCES = ${wildcard ./test/src/*.cpp}


test: ${SOURCE_WO_MAIN} ${TEST_SOURCES}
	${COMPILER} $^ -I./include -L./lib ${LIBS} -o ${TEST_OUTPUT}
	./${TEST_OUTPUT}

sandbox:
	echo ${SOURCE_WO_MAIN}


.PHONY: test sandbox
