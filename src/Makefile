CRAZY_FLAGS = -Wcast-align -Wcast-qual \
-Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self \
-Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept \
-Woverloaded-virtual -Wredundant-decls -Wshadow \
-Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 \
-Wswitch-default -Wundef -Wno-unused

# -Wold-style-cast

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
  CXX = g++-15
  CXXFLAGS = -O3 -Wall -Wextra -pedantic -std=c++17
  GLLINK = `pkg-config --libs gl glu glut`
	GLFLAGS = `pkg-config --cflags gl glu glut`
	# on apple, use brew to install freeglut and mesa-glu

	# Actually, it does not compile (yet) on mac
  VTK_CFLAGS  = -I /opt/homebrew/Cellar/vtk/9.5.2/include/vtk-9.5
  VTK_LDFLAGS = -L /opt/homebrew/Cellar/vtk/9.5.2/lib -lvtkCommonCore-9.5 -lvtkCommonDataModel-9.5 \
	 -lvtkIOXML-9.5 -lvtkIOCore-9.5
else
  CXX = g++
  CXXFLAGS = -O3 -Wall -std=c++17
  GLLINK = -lGLU -lGL -L/usr/X11R6/lib -lglut -lXext -lX11

  # --- VTK setup ---
  VTK_CFLAGS = -I/usr/include/vtk-9.1
  VTK_LDFLAGS = -lvtkCommonCore-9.1 -lvtkCommonDataModel-9.1 -lvtkIOXML-9.1 -lvtkIOCore-9.1
endif

# Ensure the toofus directory exists
ifeq ($(wildcard ./toofus),)
$(info Cloning ToOfUs repository)
$(shell git clone https://github.com/richefeu/toofus.git > /dev/null 2>&1)
endif

# The list of source files for the library SPICE
SOURCES = SPICE.cpp Particle.cpp Interaction.cpp Loading.cpp \
          pacOptionsManager.cpp packingManager.cpp farConnectionManager.cpp propertiesManager.cpp

# Each cpp file listed below corresponds to an object file
OBJECTS = $(SOURCES:%.cpp=%.o)

.PHONY: all clean clone_toofus

all: pac run see

clean:
	@echo "\033[0;32m-> Remove object files\033[0m"
	rm -f *.o
	@echo "\033[0;32m-> Remove compiled applications\033[0m"
	rm -f pac run see
	@echo "\033[0;32m-> Remove libSPICE.a\033[0m"
	rm -f libSPICE.a

clean+: clean
	@echo "\033[0;32m-> Remove local folder toofus\033[0m"
	rm -rf ./toofus

clone_toofus:
	@if [ ! -d "./toofus" ]; then \
		@echo "\033[0;32m-> CLONING ToOfUs (Tools often used)\033[0m"; \
		git clone https://github.com/richefeu/toofus.git; \
	fi

%.o: %.cpp
	@echo "\033[0;32m-> COMPILING OBJECT" $@ "\033[0m"
	$(CXX) $(CXXFLAGS) -c $< -o $@

libSPICE.a: $(OBJECTS)
	@echo "\033[0;32m-> BUILDING LIBRARY" $@ "\033[0m"
	ar rcs $@ $^

pac: pac.cpp libSPICE.a
	@echo "\033[0;32m-> BUILDING PAC APPLICATION" $@ "\033[0m"
	$(CXX) $(CXXFLAGS) -c $< -o pac.o
	$(CXX) -o $@ pac.o libSPICE.a

run: run.cpp libSPICE.a
	@echo "\033[0;32m-> BUILDING RUN APPLICATION" $@ "\033[0m"
	$(CXX) $(CXXFLAGS) -c $< -o run.o
	$(CXX) -o $@ run.o libSPICE.a

see: see.cpp libSPICE.a
	@echo "\033[0;32m-> BUILDING SEE APPLICATION" $@ "\033[0m"
	$(CXX) $(CXXFLAGS) -c $< -o see.o $(GLFLAGS)
	$(CXX) -o $@ see.o libSPICE.a $(GLLINK)

vtk: vtk.cpp libSPICE.a
	@echo "\033[0;32m-> BUILDING VTK APPLICATION" $@ "\033[0m"
	$(CXX) $(CXXFLAGS) $(VTK_CFLAGS) -c $< -o vtk.o $(GLFLAGS)
	$(CXX) -o $@ vtk.o libSPICE.a $(VTK_LDFLAGS) -lstdc++fs
