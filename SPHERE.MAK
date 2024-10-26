CC = gcc
SOURCE = SPHERE.C
BUILD = SPHERE.EXE
INCLUDE = -Ic:\atom\include
LIB = -lopengl32 -lglu32 -lgdi32

$(BUILD): $(SOURCE)
	$(CC) -o $(BUILD) $(SOURCE) $(INCLUDE) $(LIB)