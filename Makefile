SOURCES=$(shell find src -name '*.cpp')
TOOLS=$(shell find tools -name '*.java')
OBJS=$(SOURCES:src/%.cpp=build/%.o)
TEST_SOURCES=$(shell find unit_test/src -name '*.cpp')
TEST_OBJS=$(TEST_SOURCES:unit_test/src/%.cpp=unit_test/build/%.o)
LIBS=$(shell pkg-config --static --libs x11 xrandr xi xxf86vm glew glfw3)
TARGET=bot
CPPFLAGS=-std=c++11 -Werror -g -Isrc -DENABLE_LOG
TEST_CPPFLAGS=-std=c++11 -Werror -g -Isrc -Iunit_test/src -DENABLE_LOG
TEST_TARGET=testmain

.PHONY: build

default: build

setup:
	sudo apt update && sudo apt install mesa-utils xorg-dev libglfw3 \
	libglfw3-dev libglew-dev

build/%.o: src/%.cpp
	g++ $(CPPFLAGS) -c -o $@ $<

build: check_dir $(OBJS)
	g++ $(CPPFLAGS) -o build/$(TARGET) $(OBJS) $(LIBS)

check_dir:
	@if [ ! -d "build" ]; then \
		mkdir build; \
	fi

check_testdir:
	@if [ ! -d "unit_test/build" ]; then \
		mkdir unit_test/buid; \
	fi

clean:
	rm -rf build/* unit_test/build/*

run:
	./run.sh

build_tools: $(TOOLS)
	javac -d build $^

create_fonts:
	cd build; \
	java CreateCharImage; \
	mv *.png ../res

build_test: check_testdir $(TEST_OBJS)
	g++ $(TEST_CPPFLAGS) -o unit_test/build/$(TEST_TARGET) $(TEST_OBJS)

unit_test/build/%.o: unit_test/src/%.cpp
	g++ $(TEST_CPPFLAGS) -c -o $@ $<

