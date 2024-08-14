#	 Copyright (c) 2015 Microsoft Open Technologies, Inc.
#	 Copyright (c) 2021 Alibaba Group.
#    Licensed under the Apache License, Version 2.0 (the "License"); you may 
#    not use this file except in compliance with the License. You may obtain 
#    a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
#
#    THIS CODE IS PROVIDED ON AN  *AS IS* BASIS, WITHOUT WARRANTIES OR 
#    CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT 
#    LIMITATION ANY IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS 
#    FOR A PARTICULAR PURPOSE, MERCHANTABLITY OR NON-INFRINGEMENT.
#
#    See the Apache Version 2.0 License for specific language governing 
#    permissions and limitations under the License. 
#
#    Microsoft would like to thank the following companies for their review and
#    assistance with these files: Intel Corporation, Mellanox Technologies Ltd,
#    Dell Products, L.P., Facebook, Inc
#

CXX = $(CROSS_COMPILE)g++

LIBS = -lpthread -lotaivs -lotaimetadata
OTAI_IDIR = ../inc

#COMMON
MKDIR_P = mkdir -p 
ODIR = ../obj
LDIR = ../lib
BDIR = ../bin
IDIR = .
OUT_DIRS = $(BDIR) $(ODIR) $(LDIR)

#GTEST
GTEST_DIR = ../gtest-1.7.0
CXXFLAGS += -g -Wall -Wextra -pthread -I./  -std=c++11
GTEST_FLAGS = -isystem $(GTEST_DIR)/include
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
	$(GTEST_DIR)/include/gtest/internal/*.h

DEPS = $(wildcard $(OTAI_IDIR)/*.h)
DEPS += test_common.h

#basic_otn
_BROBJ = basic_otn.o linecard_test.o port_test.o oa_test.o transceiver_test.o osc_test.o aps_test.o
BROBJ = $(patsubst %,$(ODIR)/%,$(_BROBJ))

#####
all : directories $(LDIR)/gtest_main.a $(BDIR)/basic_otn
.PHONY: directories clean $(BDIR)/basic_otn

#####
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

$(BDIR)/gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(GTEST_FLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
	$(GTEST_DIR)/src/gtest-all.cc -o $@

$(BDIR)/gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(GTEST_FLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
	$(GTEST_DIR)/src/gtest_main.cc -o $@

$(LDIR)/gtest.a : $(BDIR)/gtest-all.o
	$(AR) $(ARFLAGS) -o $@ $^ 

$(LDIR)/gtest_main.a : $(BDIR)/gtest-all.o $(BDIR)/gtest_main.o
	$(AR) $(ARFLAGS) -o $@ $^

####
directories: 
	$(MKDIR_P) $(OUT_DIRS)

$(ODIR)/%.o : $(IDIR)/%.cpp 
	$(CXX) -c $^ -o $@ $(CXXFLAGS) -I$(OTAI_IDIR) -I $(GTEST_DIR)/include

$(ODIR)/basic_otn.o : $(IDIR)/basic_otn.cpp \
	$(GTEST_HEADERS) $(DEPS) 
	$(CXX) $(GTEST_FLAGS) $(CXXFLAGS) -I $(OTAI_IDIR) -I $(GTEST_DIR)/include/gtest/ \
	-I $(GTEST_DIR)/include/gtest/internal/ -c $(IDIR)/basic_otn.cpp -o $@ 

$(BDIR)/basic_otn: $(ODIR)/basic_otn.o $(LDIR)/gtest_main.a $(BROBJ)
	$(CXX) $(CXXFLAGS)  $^ -o $@ $(LIBS) 

clean :
	rm -rf $(BDIR) $(ODIR) $(LDIR)