# Created by DJ Edmonson
#   dedmons@g.clemson.edu
# Warnings frequently signal eventual errors:
CXXFLAGS=`sdl-config --cflags` -std=gnu++98 -g -W -Wall -Weffc++ -Wextra -pedantic -O0
# Linker flags for both OS X and Linux
LDFLAGS = `sdl-config --libs` -lSDL_ttf -lSDL_image -lexpat

# Generates list of object files from all the
#   source files in directory
OBJS = $(addsuffix .o, $(basename $(shell find *.cpp)))
DEPS = $(OBJS:.o=.d)

# Set executable name
EXEC = run

SCAN_BUILD = $(shell which scan-build)
ifeq "$(SCAN_BUILD)" ""
SCAN_BUILD="./checker-271/scan-build"
endif


# Declare the phony targets
.PHONY: echo clean r clang gcc \
  setclang setgcc vg lint csa sbsetup sbclean sb

# Phony targets to run dependencies in order
clang: | setclang $(EXEC)
gcc: | setgcc $(EXEC)

# For use with the clang static analyzer by
#  using the environment variable for CXX
sb: | clean $(DEPS) $(EXEC)

ifeq "$(shell uname)" "Darwin"
# Mac only!!! if using linux, install clang with package manager
#   and it should install scan-build. Check with `which scan-build`
sbsetup:
	@curl -o checker.tar.bz2 https://attache.apple.com/AttacheWeb/dl?id=ATC44d356e48110443aa990771381dd5cc0
	@tar -xvf checker.tar.bz2
	@rm -f checker.tar.bz2

# Remove the folder created to get the clang static analyzer
sbclean:
	rm -r checker*
endif

csa:
	$(SCAN_BUILD) --view make sb

# target to run valgrind on executable
vg: $(EXEC)
	valgrind ./$(EXEC)

# Uses cppcheck for more static analysys
lint:
	cppcheck ./*.h ./*.cpp

# Run the executable
r:
	./$(EXEC)

# Clean all the created files
clean:
	rm -rf $(OBJS)
	rm -rf $(DEPS)
	rm -rf $(EXEC)

# Phony target to use clang for compile and linking
setclang:
	@echo "Setting clang"
	$(eval CXX = clang++)
	$(eval CXX_LINK = clang++)

# Phony target to use g++ for compile and linking
setgcc:
	@echo "Setting g++"
	$(eval CXX = g++)
	$(eval CXX_LINK = g++)

# $< refers to the first dependency
# Uses static pattern rule to keep from compiling all
#   objects every time.
$(OBJS): %.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Static pattern to build dependencies
$(DEPS): %.d: %.cpp
	@echo "Generating "$@
	@set -e; rm -f $@; \
      g++ -MM $(CPPFLAGS) $< > $@.$$$$; \
      sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
      rm -f $@.$$$$

# Link all the object files together into exicutible
$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)

include $(DEPS)
