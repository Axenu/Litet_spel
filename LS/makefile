# Object files to either reference or create
SRCDIR=src/
ODIR=build/
lIBDIR=lib/OSX/
IDIR=include/
HDIR=header/

SRC_DIR   := src src/gl src/Event src/Event/Input src/Game src/Game/Level src/Game/Objects src/Game/Scene src/gui src/gui/Views src/Render src/Render/Mesh src/Render/Mesh/Shader src/Render/Mesh/Animation src/Render/Composition src/Math
BUILD_DIR := $(SRC_DIR:src%=build%)
SRC       := $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.cpp))
OBJ_files := $(SRC:src%=build%)
OBJ       := $(OBJ_files:.cpp=.o)

# The executable file that will be created
EXEC = main
# The c++ flags to use for compilation
CXXFLAGS = -Wall -Wextra -std=c++11 -Wdouble-promotion -Wno-sign-conversion -Wno-unused-parameter -Wno-reorder -Wno-unknown-pragmas -Wno-unused-variable -Wno-overloaded-virtual
# The c++ compiler to use for compilation
CC = g++

LIB=-lglfw3 -lil -ltiff -lpng16 -lz -ljasper -ljpeg -llzma -lfreetype -lbz2 -lmng -llcms -lassimp
#OSX frameworks
FRAMEWORKS=-framework IOKit -framework Cocoa -framework OpenGL -framework CoreVideo

# This section is called on 'make'
# Will call compile, and then call clean
all: checkdirs buildAll

buildAll: $(OBJ)
	$(CC) -L $(lIBDIR) -o $(EXEC) $^ $(FRAMEWORKS) $(LIB)

$(ODIR)%.o: $(SRCDIR)%.cpp
	$(CC) -I $(IDIR) -I$(HDIR) $(CXXFLAGS) -c -o $@ $<

checkdirs: $(BUILD_DIR)

$(BUILD_DIR):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR)
	rm $(EXEC)
