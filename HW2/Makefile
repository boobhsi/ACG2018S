INCLUDE_DIR = "include"
CXX = g++
CXXFLAGS = -O3 -Wall -I ${INCLUDE_DIR} -std=c++11
EXECUTABLE = main.out
OBJECT_DIR = obj
SOURCE_DIR = src

LIGHT_MODULE = light.o point_light.o
OBJECT_MODULE = mesh.o object_sphere.o object_triangle.o
RAYTRACER_MODULE = ray.o raytracer.o
UTILS_MODULE = algebra3.o
MAIN = main.o

all: $(EXECUTABLE) clear

DEBUG: CXXFLAGS += -DDEBUG
DEBUG: all

$(EXECUTABLE): $(MAIN) $(UTILS_MODULE)  $(LIGHT_MODULE) $(OBJECT_MODULE) $(RAYTRACER_MODULE)
	$(CXX) $^ $(CXXFLAGS) -o $@

main.o: $(SOURCE_DIR)/main.cpp
	$(CXX) $^ $(CXXFLAGS) -c -o $@

light.o: $(SOURCE_DIR)/lights/light.cpp
	$(CXX) $^ $(CXXFLAGS) -c -o $@

point_light.o: $(SOURCE_DIR)/lights/point_light.cpp
	$(CXX) $^ $(CXXFLAGS) -c -o $@

mesh.o: $(SOURCE_DIR)/objects/mesh.cpp
	$(CXX) $^ $(CXXFLAGS) -c -o $@

object_sphere.o: $(SOURCE_DIR)/objects/object_sphere.cpp
	$(CXX) $^ $(CXXFLAGS) -c -o $@

object_triangle.o: $(SOURCE_DIR)/objects/object_triangle.cpp
	$(CXX) $^ $(CXXFLAGS) -c -o $@

ray.o: $(SOURCE_DIR)/raytracer/ray.cpp
	$(CXX) $^ $(CXXFLAGS) -c -o $@

raytracer.o: $(SOURCE_DIR)/raytracer/raytracer.cpp
	$(CXX) $^ $(CXXFLAGS) -c -o $@

algebra3.o: $(SOURCE_DIR)/utils/algebra3.cpp
	$(CXX) $^ $(CXXFLAGS) -c -o $@

.PHONY: clean clear all

clear:
	@rm -rf *.o

clean:
	clear
	@rm -rf $(EXECUTABLE)

