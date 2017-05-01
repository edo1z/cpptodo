LDLIBS   = -lm -lmysqlclient
FLAGS = -Wall -I/usr/local/include
LDFLAGS  = -L/usr/local/lib

todo: main.cpp
	c++ -O2 -o $@ $(LDLIBS) $(FLAGS) $(LDFLAGS) $^