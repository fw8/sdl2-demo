program_NAME := spiel
program_C_SRCS := $(wildcard *.c)
program_CXX_SRCS := $(wildcard *.cpp)
program_C_OBJS := ${program_C_SRCS:.c=.o}
program_CXX_OBJS := ${program_CXX_SRCS:.cpp=.o}
program_OBJS := $(program_C_OBJS) $(program_CXX_OBJS)
program_INCLUDE_DIRS := /usr/local/Cellar/sdl2/2.0.8/include/SDL2 /usr/local/Cellar/sdl2_image/2.0.3/include/SDL2 /usr/local/Cellar/sdl2_ttf/2.0.14/include/SDL2 /usr/local/Cellar/sdl2_gfx/1.0.4/include/SDL2
program_LIBRARY_DIRS := /usr/local/Cellar/sdl2/2.0.8/lib /usr/local/Cellar/sdl2_image/2.0.3/lib /usr/local/Cellar/sdl2_ttf/2.0.14/lib /usr/local/Cellar/sdl2_gfx/1.0.4/lib
program_LIBRARIES := SDL2-2.0.0 SDL2_image-2.0.0 SDL2_ttf-2.0.0 SDL2_gfx-1.0.0

CPPFLAGS += $(foreach includedir,$(program_INCLUDE_DIRS),-I$(includedir))
LDFLAGS += $(foreach librarydir,$(program_LIBRARY_DIRS),-L$(librarydir))
LDFLAGS += $(foreach library,$(program_LIBRARIES),-l$(library))

.PHONY: all clean distclean

all: $(program_NAME)

$(program_NAME): $(program_OBJS)
	$(LINK.cc) $(program_OBJS) -o $(program_NAME)

clean:
	@- $(RM) $(program_NAME)
	@- $(RM) $(program_OBJS)

distclean: clean
