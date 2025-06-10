#compiler
CXX = g++

#libs
LIBS = -lglfw -lGL

#dirs
BUILD = ./Build
INCLUDE = ./Include

#flags
CXXFLAGS = -Wall -g -I$(INCLUDE)

#objs
OBJS = ./Main.o \
			 ./Include/Shader/Shader.o \
			 ./Include/stb_image/stb_image.o \
			 ./Include/Texture/Texture.o \
			 ./Include/Memory/VBO.o \
			 ./Include/Memory/VAO.o \
			 ./Player/Player.o \
			 ./Include/glad/glad.o

#target
TARGET = $(BUILD)/lts

#all-rule
all: $(TARGET)

#linking
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LIBS) $(CXXFLAGS)

#compiling
%.o: %.cpp
	$(CXX) -c $< -o $@ $(LIBS) $(CXXFLAGS)

#clean-rule
.PHONY: clean
clean:
	rm -rf $(TARGET) $(OBJS)
