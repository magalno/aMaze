CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS= -std=c++11 `pkg-config --cflags opencv`
LDFLAGS= -std=c++11
LDLIBS= `pkg-config --libs opencv`

SRCS=main.cpp preprocessing.cpp perspective_correction.cpp solver.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all: aMaze

aMaze: $(OBJS)
	$(CXX) $(LDFLAGS) -o aMaze $(OBJS) $(LDLIBS)

depend: .depend

.depend: $(SRCS)
	rm -f ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	$(RM) $(OBJS)

dist-clean: clean
	$(RM) *~ .depend

include .depend
