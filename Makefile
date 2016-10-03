#
# 'make depend' uses makedepend to automatically generate dependencies 
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'mycc'
# 'make clean'  removes all .o and executable files
#

CXX = g++
CXXFLAGS = -Wall -std=c++11 -g -I./
LDFLAGS = 
LIBS = -lcrypto

# define the C source files
COMMON_SRCS 	= 	./Utils.cpp 

CHALLENGE1_SRCS = 	./challenge1.cpp
CHALLENGE2_SRCS = 	./challenge2.cpp
CHALLENGE3_SRCS = 	./challenge3.cpp
CHALLENGE4_SRCS = 	./challenge4.cpp
CHALLENGE5_SRCS = 	./challenge5.cpp
CHALLENGE6_SRCS = 	./challenge6.cpp
CHALLENGE7_SRCS = 	./challenge7.cpp
CHALLENGE8_SRCS = 	./challenge8.cpp
CHALLENGE9_SRCS = 	./challenge9.cpp
CHALLENGE10_SRCS = 	./challenge10.cpp
CHALLENGE11_SRCS = 	./challenge11.cpp

# Binaries
CHALLENGE1 = 'challenge1.out'
CHALLENGE2 = 'challenge2.out'
CHALLENGE3 = 'challenge3.out'
CHALLENGE4 = 'challenge4.out'
CHALLENGE5 = 'challenge5.out'
CHALLENGE6 = 'challenge6.out'
CHALLENGE8 = 'challenge8.out'
CHALLENGE9 = 'challenge9.out'
CHALLENGE10 = 'challenge10.out'
CHALLENGE11 = 'challenge11.out'

#
# The following part of the makefile is generic; it can be used to 
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#

COMMON_OBJS = $(COMMON_SRCS:.cpp=.o)
CHALLENGE1_OBJS = $(CHALLENGE1_SRCS:.cpp=.o)
CHALLENGE2_OBJS = $(CHALLENGE2_SRCS:.cpp=.o)
CHALLENGE3_OBJS = $(CHALLENGE3_SRCS:.cpp=.o)
CHALLENGE4_OBJS = $(CHALLENGE4_SRCS:.cpp=.o)
CHALLENGE5_OBJS = $(CHALLENGE5_SRCS:.cpp=.o)
CHALLENGE6_OBJS = $(CHALLENGE6_SRCS:.cpp=.o)
CHALLENGE7_OBJS = $(CHALLENGE7_SRCS:.cpp=.o)
CHALLENGE8_OBJS = $(CHALLENGE8_SRCS:.cpp=.o)
CHALLENGE9_OBJS = $(CHALLENGE9_SRCS:.cpp=.o)
CHALLENGE10_OBJS = $(CHALLENGE10_SRCS:.cpp=.o)
CHALLENGE11_OBJS = $(CHALLENGE11_SRCS:.cpp=.o)

.PHONY: depend clean

all: $(CHALLENGE1) $(CHALLENGE2) $(CHALLENGE3) $(CHALLENGE4) $(CHALLENGE5) $(CHALLENGE6) $(CHALLENGE7) \
	 $(CHALLENGE8) $(CHALLENGE9) $(CHALLENGE10) $(CHALLENGE11)
	@echo Build All

$(CHALLENGE1): $(COMMON_OBJS) $(CHALLENGE1_OBJS)
	$(CXX) $(COMMON_OBJS) $(CHALLENGE1_OBJS) $(CXXFLAGS) $(LDFLAGS) $(LIBS) -o $(CHALLENGE1)

$(CHALLENGE2): $(COMMON_OBJS) $(CHALLENGE2_OBJS)
	$(CXX) $(COMMON_OBJS) $(CHALLENGE2_OBJS) $(CXXFLAGS) $(LDFLAGS) $(LIBS) -o $(CHALLENGE2)

$(CHALLENGE3): $(COMMON_OBJS) $(CHALLENGE3_OBJS)
	$(CXX) $(COMMON_OBJS) $(CHALLENGE3_OBJS) $(CXXFLAGS) $(LDFLAGS) $(LIBS) -o $(CHALLENGE3)

$(CHALLENGE4): $(COMMON_OBJS) $(CHALLENGE4_OBJS)
	$(CXX) $(COMMON_OBJS) $(CHALLENGE4_OBJS) $(CXXFLAGS) $(LDFLAGS) $(LIBS) -o $(CHALLENGE4)

$(CHALLENGE5): $(COMMON_OBJS) $(CHALLENGE5_OBJS)
	$(CXX) $(COMMON_OBJS) $(CHALLENGE5_OBJS) $(CXXFLAGS) $(LDFLAGS) $(LIBS) -o $(CHALLENGE5)

$(CHALLENGE6): $(COMMON_OBJS) $(CHALLENGE6_OBJS)
	$(CXX) $(COMMON_OBJS) $(CHALLENGE6_OBJS) $(CXXFLAGS) $(LDFLAGS) $(LIBS) -o $(CHALLENGE6)

$(CHALLENGE7): $(COMMON_OBJS) $(CHALLENGE7_OBJS)
	$(CXX) $(COMMON_OBJS) $(CHALLENGE7_OBJS) $(CXXFLAGS) $(LDFLAGS) $(LIBS) -o $(CHALLENGE7)

$(CHALLENGE8): $(COMMON_OBJS) $(CHALLENGE8_OBJS)
	$(CXX) $(COMMON_OBJS) $(CHALLENGE8_OBJS) $(CXXFLAGS) $(LDFLAGS) $(LIBS) -o $(CHALLENGE8)

$(CHALLENGE9): $(COMMON_OBJS) $(CHALLENGE9_OBJS)
	$(CXX) $(COMMON_OBJS) $(CHALLENGE9_OBJS) $(CXXFLAGS) $(LDFLAGS) $(LIBS) -o $(CHALLENGE9)

$(CHALLENGE10): $(COMMON_OBJS) $(CHALLENGE10_OBJS)
	$(CXX) $(COMMON_OBJS) $(CHALLENGE10_OBJS) $(CXXFLAGS) $(LDFLAGS) $(LIBS) -o $(CHALLENGE10)

$(CHALLENGE11): $(COMMON_OBJS) $(CHALLENGE11_OBJS)
	$(CXX) $(COMMON_OBJS) $(CHALLENGE11_OBJS) $(CXXFLAGS) $(LDFLAGS) $(LIBS) -o $(CHALLENGE11)

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

