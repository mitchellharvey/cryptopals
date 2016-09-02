#
# 'make depend' uses makedepend to automatically generate dependencies 
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'mycc'
# 'make clean'  removes all .o and executable files
#

CXX = g++
CXXFLAGS = -Wall -std=c++11 -g -I./
LDFLAGS = 
LIBS = 

# define the C source files
COMMON_SRCS 	= 	./Utils.cpp 

HEX2BASE64_SRCS = 	./hex2base64.cpp

# Binaries
HEX2BASE64 = 'hex2base64.out'

#
# The following part of the makefile is generic; it can be used to 
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#

COMMON_OBJS = $(COMMON_SRCS:.cpp=.o)
HEX2BASE64_OBJS = $(HEX2BASE64_SRCS:.cpp=.o)

.PHONY: depend clean

all: $(HEX2BASE64)
	@echo Build All

$(HEX2BASE64): $(COMMON_OBJS) $(HEX2BASE64_OBJS)
	$(CXX) $(COMMON_OBJS) $(HEX2BASE64_OBJS) $(CXXFLAGS) $(LDFLAGS) $(LIBS) -o $(HEX2BASE64)

# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file) 
# (see the gnu make manual section about automatic variables)
.cpp.o:
	$(CXX) $(CFLAGS) -c $<  -o $@

clean:
	@$(RM) *.o *~ $(HEX2BASE64)
	@echo Clean Complete

depend: $(SRCS)
	makedepend  $^

# DO NOT DELETE THIS LINE -- make depend needs it

