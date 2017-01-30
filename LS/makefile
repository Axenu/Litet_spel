# Object files to either reference or create
SRCDIR=src/
ODIR=build/
lIBDIR=lib/OSX/
IDIR=include/OSX/
HDIR=header/

SOURCESGAME=$(wildcard $(SRCDIR)*.cpp)
SOURCEGL=$(wildcard $(SRCDIR)gl/*.cpp)
SOURCERender=$(wildcard $(SRCDIR)Render/*.cpp)
SOURCEFILESGAME = $(SOURCESGAME:$(SRCDIR)%=%) $(SOURCEGL:$(SRCDIR)gl/%=%) $(SOURCERender:$(SRCDIR)Render/%=%)

# The executable file that will be created
EXEC = main
# The c++ flags to use for compilation
CXXFLAGS = -Wall -Wno-unknown-pragmas -std=c++11 -Wno-c++11-long-long
# The c++ compiler to use for compilation
CC = g++


GAMEOBJECTS = $(addprefix $(ODIR), $(SOURCEFILESGAME:.cpp=.o))


LIB=-lglfw3# -ltiff -lpng16 -lz -ljasper -ljpeg -llzma -lfreetype -lbz2 -lmng -llcms

#OSX frameworks
FRAMEWORKS=-framework IOKit -framework Cocoa -framework OpenGL -framework CoreVideo


# This section is called on 'make'
# Will call compile, and then call clean
all: $(GAMEOBJECTS)
	$(CC) -L $(lIBDIR) -o $(EXEC) $^ $(FRAMEWORKS) $(LIB)

# build/FireArrow.a: $(ENGINEOBJECTS)
	# ar rvs build/FireArrow.a $^

$(ODIR)%.o: $(SRCDIR)%.cpp
	$(CC) -I $(IDIR) -I$(HDIR) $(CXXFLAGS) -c -o $@ $<

$(ODIR)%.o: $(SRCDIR)gl/%.cpp
	$(CC) -I $(IDIR) -I$(HDIR) $(CXXFLAGS) -c -o $@ $<

$(ODIR)%.o: $(SRCDIR)Render/%.cpp
	$(CC) -I $(IDIR) -I$(HDIR) $(CXXFLAGS) -c -o $@ $<

# $(ODIR)/%.o: $(ENGINESDIR)/%.cpp
# 	$(CC) -I $(IDIR) -I$(HDIR) -std=c++11 -Wno-c++11-long-long -Wall -c -o $@ $<

clean:
	rm -f $(ODIR)/*.o
	rm -f $(ODIR)/*.a
	rm $(EXEC)
