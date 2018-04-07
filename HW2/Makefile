UTILS_DIR = ./utils
CXX = g++
CXXFLAGS = -O3 -Wall
EXECUTABLE = main.out

.SUFFIXS: .CPP

UTILS_FILES = \
	algebra3.o \
	object_sphere.o \
	object_triangle.o \
	ray.o

all: $(EXECUTABLE) clear

DEBUG: CXXFLAGS += -DDEBUG
DEBUG: all

$(EXECUTABLE): main.o raytracer.o $(UTILS_FILES)
	$(CXX) $^ $(CXXFLAGS) -o $@

main.o: main.cpp
	$(CXX) $< $(CXXFLAGS) -c

raytracer.o: raytracer.cpp
	$(CXX) $< $(CXXFLAGS) -c

%.o: $(UTILS_DIR)/%.cpp
	$(CXX) $< $(CXXFLAGS) -c


.PHONY: clean clear all

clear:
	@rm -rf *.o

clean:
	clear
	rm -rf $(EXECUTABLE)

