.PHONY = all clean

# Includes path
FREETYPE_INCLUDE := -I/usr/include/freetype2
THIRDPARTY_INCLUDES := -Idep/include/ThirdParty
INCLUDES := $(THIRDPARTY_INCLUDES) -Idep/include
#WIN_INCLUDES := -Ldep/lib

# Compilers and linker
CC := gcc
CCFLAGS := -O2 $(INCLUDES)

CXX := g++
CXXFLAGS := -O2 $(INCLUDES)

# Flags
FREETYPE_LIB := -L/usr/include/libpng16 -lfreetype
OPENGLFLAGS := $(FREETYPE_LIB) -lglfw -lX11 -lpthread -lXrandr -lXi -ldl
#CXXFLAGS := -Wall -Wextra -Werror -g -O2

# Directory structure
SRCDIR := src
OBJDIR := obj
BINDIR := bin

# Executable name
TARGET := core_consolidation_linux_build
EXEC := $(BINDIR)/$(TARGET)

# Source and object files
CPP_SRCS := $(shell find $(SRCDIR) -name '*.cpp')
C_SRCS := $(shell find $(SRCDIR) -name '*.c')

CPP_OBJS := $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(CPP_SRCS))
C_OBJS := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(C_SRCS))

OBJS := $(CPP_OBJS) $(C_OBJS)

# Default target
all: $(EXEC)

# Link objects into the final binary
$(EXEC): $(OBJS) | $(BINDIR)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@ $(OPENGLFLAGS)

# Compile .c to .o
$(OBJDIR)/%.o: $(SRCDIR)/%.c | prepare-dirs
	$(CC) $(CCFLAGS) -c $< -o $@

# Compile .cpp to .o
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | prepare-dirs
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(OPENGLFLAGS)

# Ensure bin/ and obj/ exist
prepare-dirs:
	mkdir -p $(sort $(dir $(OBJS)))

$(BINDIR):
	mkdir -p $@

clean:
	@echo "Cleaning up..."
	rm -rf $(OBJDIR) $(BINDIR)
