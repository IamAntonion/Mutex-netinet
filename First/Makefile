PROJECT_ROOT = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

OBJS = First.o
LDFLAGS += -lpthread
CFLAGS += -std=c++17
CFLAGS += -O2

all:	First

First:	$(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)
	$(EXTRA_CMDS)

%.o:	$(PROJECT_ROOT)%.cpp
	$(CXX) -c $(CFLAGS) $(CXXFLAGS) $(CPPFLAGS) -o $@ $<

%.o:	$(PROJECT_ROOT)%.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<

clean:
	rm -fr First $(OBJS) $(EXTRA_CLEAN)
