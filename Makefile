# Warnings frequently signal eventual errors:
CXXFLAGS=`sdl-config --cflags` -std=gnu++98 -g -W -Wall -Weffc++ -Wextra -pedantic -O0
#CXXFLAGS=-g -W -Wall -Weffc++ -Wextra -pedantic -O0
# Linker flags for both OS X and Linux
LDFLAGS = `sdl-config --libs` -lSDL_ttf -lSDL_image -lexpat
#LDFLAGS =

# Generates list of object files from all the
#   source files in directory
OBJS = $(addsuffix .o, $(basename $(shell find *.cpp)))
DEPS = $(OBJS:.o=.d)

# Set executable name
EXEC = run

# Declare the phony targets
.PHONY: echo clean r clang gcc setclang setgcc vg lint

# Phony targets to run dependencies in order
clang: | setclang $(EXEC)
gcc: | setgcc $(EXEC)

# For use with the clang static analyzer by
#  using the environment variable for CXX
sb: | $(clean) $(EXEC)

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
