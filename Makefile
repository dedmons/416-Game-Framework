# Created by DJ Edmonson
#   dedmons@g.clemson.edu

# Warnings frequently signal eventual errors:
CXXFLAGS=`sdl-config --cflags` -g -W -Wall -Weffc++ -Wextra -pedantic -O0
# Linker flags for both OS X and Linux
LDFLAGS = `sdl-config --libs` -lSDL_ttf -lSDL_image -lSDL_gfx -lSDL_mixer

# Generates list of object files from all the
#   source files in directory
OBJS = $(addsuffix .o, $(basename $(shell find *.cpp)))
DEPS = $(OBJS:.o=.d)

# Set executable name
EXEC = run

ifeq (/,$(findstring /,$(shell which g++-4.8)))
	CXX = g++-4.8
else
ifeq (/,$(findstring /,$(shell which g++-4.7)))
	CXX = g++-4.7
else
ifeq (/,$(findstring /,$(shell which g++-4.6)))
	CXX = g++-4.6
else
	CXX = g++
endif
endif
endif

# Declare the phony targets
.PHONY: echo clean r clang gcc \
  setclang setgcc vg

# Phony targets to run dependencies in order
clang: | setclang $(EXEC)
gcc: | setgcc $(EXEC)

comptest:
	echo $(CXX)

# target to run valgrind on executable
vg: $(EXEC)
	valgrind ./$(EXEC)

# Run the executable
r:
	./$(EXEC)

# Clean all the created files
clean:
	rm -rf $(OBJS)
	rm -rf $(DEPS)
	rm -rf $(EXEC)
	rm -rf *.d.*

# Phony target to use clang for compile and linking
setclang:
	@echo "Setting clang"
	$(eval CXX = clang++)
	$(eval CXX_LINK = clang++)
ifeq "$(shell uname)" "Darwin"
	$(eval CXXFLAGS = $(CXXFLAGS) -std=c++11 -stdlib=libc++)
else
	$(eval CXXFLAGS = $(CXXFLAGS) -std=c++11)
endif

# Phony target to use g++ for compile and linking
setgcc:
	@echo "Setting g++"
	$(eval CXXFLAGS = $(CXXFLAGS) -std=c++11)

# $< refers to the first dependency
# Uses static pattern rule to keep from compiling all
#   objects every time.
$(OBJS): %.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Static pattern to build dependencies
$(DEPS): %.d: %.cpp
	@echo "Generating "$@
	@set -e; rm -f $@; \
      $(CXX) -MM $(CPPFLAGS) -std=c++0x $< > $@.$$$$; \
      sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
      rm -f $@.$$$$

# Link all the object files together into exicutible
$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)

include $(DEPS)
