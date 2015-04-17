CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS= `pkg-config --cflags opencv`
LDFLAGS=
LDLIBS= `pkg-config --libs opencv`

SRCS=main.cpp perspective_correction.cpp solver.cpp
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
