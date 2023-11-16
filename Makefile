OBJS=jmalloc.o
CXX=g++
CXXFLAGS=-Wall -Wextra -std=c++17 -pedantic

jallocator: $(OBJS)
	$(CXX) $(CFLAGS) -o $@ $^ $(LIBS)