# Object files to either reference or create
SRCDIR=src/
ODIR=build/
lIBDIR=lib/OSX/
IDIR=include/
HDIR=header/

SRC_DIR   := src src/gl src/Event src/Event/Input src/Game src/Game/Objects src/Game/Scene src/gui src/Render
BUILD_DIR := $(SRC_DIR:src%=build%)
SRC       := $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.cpp))
OBJ_files := $(SRC:src%=build%)
OBJ       := $(OBJ_files:.cpp=.o)

# The executable file that will be created
EXEC = main
# The c++ flags to use for compilation
CXXFLAGS = -Wall -std=c++11 -Wno-c++11-long-long -Wno-reorder -Wno-unknown-pragmas -Wno-unused-variable -Wno-unused-private-field -Wno-return-type -Wno-overloaded-virtual
# The c++ compiler to use for compilation
CC = g++

LIB=-lglfw3 -lil -ltiff -lpng16 -lz -ljasper -ljpeg -llzma -lfreetype -lbz2 -lmng -llcms
#OSX frameworks
FRAMEWORKS=-framework IOKit -framework Cocoa -framework OpenGL -framework CoreVideo

# This section is called on 'make'
# Will call compile, and then call clean
all: checkdirs buildAll

buildAll: $(OBJ)
	$(CC) -L $(lIBDIR) -o $(EXEC) $^ $(FRAMEWORKS) $(LIB)

# build/FireArrow.a: $(ENGINEOBJECTS)
	# ar rvs build/FireArrow.a $^

$(ODIR)%.o: $(SRCDIR)%.cpp
	$(CC) -I $(IDIR) -I$(HDIR) $(CXXFLAGS) -c -o $@ $<

checkdirs: $(BUILD_DIR)

$(BUILD_DIR):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR)
	rm -f $(ODIR)*.o
	rm -f $(ODIR)*.a
	rm $(EXEC)


# $(foreach bdir,$(BUILD_DIR),$(eval $(call make-goal,$(bdir))))
