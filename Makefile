# ###
# ### Makefile for Gerp
# ###
# ### Author:  Sam Miller & Sami Hakkarainen
# ###
# ### Purpose: The purpose of this file is to build, clean and compile all of
# ###   		 the files that are needed to build the ultimate execulatable 
# ###			 which is Gerp. 

MAKEFLAGS += -L 
CXX      = clang++
CXXFLAGS = -g3 -Wall -Wextra -Wpedantic -Wshadow
LDFLAGS  = -g3
CFLAGS_COMMON += -O2

# # make treeTraversal
gerp:  gerp.o stringProcessing.o FSTree.o DirNode.o 
	$(CXX) $(CXXFLAGS) $(CFLAGS_COMMON) -o gerp gerp.o stringProcessing.o \
						 FSTree.o DirNode.o 

stringProcessing.o: stringProcessing.cpp
	$(CXX) $(CXXFLAGS) -c stringProcessing.cpp

gerp.o: gerp.cpp 
	$(CXX) $(CXXFLAGS) -c gerp.cpp

hashTable.o: hashTable.cpp
	$(CXX) $(CXXFLAGS) -c hashTable.cpp



# unit_test: unit_test_driver.o stringProcessing.o FSTreeTraversal.o \
# 	FSTree.o DirNode.o hashTable.o
# 	$(CXX) $(CXXFLAGS) $^

unit_test: unit_test_driver.o stringProcessing.o
	$(CXX) $(CXXFLAGS) $^

# clean:
# 	rm -f DatumStack *~a.out

# ## Here is a rule for compiling .cyl files for use in your testing
# ##
# %.cylc:  %.cyl
# 	./cylc $^