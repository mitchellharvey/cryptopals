#
# 'make depend' uses makedepend to automatically generate dependencies 
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'mycc'
# 'make clean'  removes all .o and executable files
#

CXX = g++
CXXFLAGS = -Wall -std=c++11 -O3 -I./
LDFLAGS = 
LIBS = 

# define the C source files
COMMON_SRCS 	= 	./Utils.cpp 

CHALLENGE1_SRCS = 	./challenge1.cpp
CHALLENGE2_SRCS = 	./challenge2.cpp

# Binaries
CHALLENGE1 = 'challenge1.out'
CHALLENGE2 = 'challenge2.out'

#
# The following part of the makefile is generic; it can be used to 
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#

COMMON_OBJS = $(COMMON_SRCS:.cpp=.o)
CHALLENGE1_OBJS = $(CHALLENGE1_SRCS:.cpp=.o)
CHALLENGE2_OBJS = $(CHALLENGE2_SRCS:.cpp=.o)

.PHONY: depend clean

all: $(CHALLENGE1) $(CHALLENGE2)
	@echo Build All

$(CHALLENGE1): $(COMMON_OBJS) $(CHALLENGE1_OBJS)
	$(CXX) $(COMMON_OBJS) $(CHALLENGE1_OBJS) $(CXXFLAGS) $(LDFLAGS) $(LIBS) -o $(CHALLENGE1)

$(CHALLENGE2): $(COMMON_OBJS) $(CHALLENGE2_OBJS)
	$(CXX) $(COMMON_OBJS) $(CHALLENGE2_OBJS) $(CXXFLAGS) $(LDFLAGS) $(LIBS) -o $(CHALLENGE2)

# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file) 
# (see the gnu make manual section about automatic variables)
.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<  -o $@

clean:
	@$(RM) *.o *~ *.out
	@echo Clean Complete

depend: $(SRCS)
	makedepend  $^

# DO NOT DELETE THIS LINE -- make depend needs it

