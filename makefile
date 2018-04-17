#
# Copyright (c) 2016-present, Facebook, Inc.
# All rights reserved.
#
# This source code is licensed under the BSD-style license found in the
# LICENSE file in the root directory of this source tree. An additional grant
# of patent rights can be found in the PATENTS file in the same directory.
#

CXX = g++
CXXFLAGS = -pthread -std=c++11

BOOST_DIR = /usr/local/bin/boost_1_63_0/
GTEST_DIR = /usr/local/bin/googletest

OBJS = normalize.o dict.o args.o proj.o parser.o data.o model.o starspace.o doc_parser.o doc_data.o utils.o starspace_api.o
TESTS = matrix_test proj_test
INCLUDES = -I$(BOOST_DIR)

STATIC_LIB= libstarspace.a

all: starspace apps lib

opt: CXXFLAGS += -O3 -funroll-loops
opt: starspace

debug: CXXFLAGS += -g -O0 -fno-inline
debug: starspace


TEST_INCLUDES = -I$(GTEST_DIR)/include

GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

CPPFLAGS += -isystem $(GTEST_DIR)/include

GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

# For simplicity and to avoid depending on Google Test's
# implementation details, the dependencies specified below are
# conservative and not optimized.  This is fine as Google Test
# compiles fast and for ordinary users its source rarely changes.

gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc

gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest_main.cc

gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

gtest_main.a : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^


normalize.o: src/utils/normalize.cpp src/utils/normalize.h
	$(CXX) $(CXXFLAGS) -g -c src/utils/normalize.cpp

dict.o: src/dict.cpp src/dict.h src/utils/args.h
	$(CXX) $(CXXFLAGS) -g -c src/dict.cpp

args.o: src/utils/args.cpp src/utils/args.h
	$(CXX) $(CXXFLAGS) -g -c src/utils/args.cpp

matrix_test.o: src/test/matrix_test.cpp src/matrix.h $(GTEST_HEADERS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(TEST_INCLUDES) -g -c src/test/matrix_test.cpp

model.o: data.o src/model.cpp src/model.h src/utils/args.h src/proj.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -g -c src/model.cpp

matrix_test: matrix_test.o gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

proj.o: src/proj.cpp src/proj.h src/matrix.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -g -c src/proj.cpp

proj_test.o: src/test/proj_test.cpp src/proj.h $(GTEST_HEADERS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(TEST_INCLUDES) -g -c src/test/proj_test.cpp

proj_test: proj.o proj_test.o gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

data.o: parser.o src/data.cpp src/data.h
	$(CXX) $(CXXFLAGS) -g -c src/data.cpp -o data.o

utils.o: src/utils/utils.cpp src/utils/utils.h
	$(CXX) $(CXXFLAGS) -g -c src/utils/utils.cpp -o utils.o

doc_data.o: doc_parser.o data.o src/doc_data.cpp src/doc_data.h
	$(CXX) $(CXXFLAGS) -g -c src/doc_data.cpp -o doc_data.o

parser.o: dict.o src/parser.cpp src/parser.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -g -c src/parser.cpp -o parser.o

doc_parser.o: dict.o src/doc_parser.cpp src/doc_parser.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -g -c src/doc_parser.cpp -o doc_parser.o

starspace.o: src/starspace.cpp src/starspace.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -g -c src/starspace.cpp

starspace_api.o: src/api/starspace_api.cpp src/api/starspace_api.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -g -c src/api/starspace_api.cpp

starspace: $(OBJS) src/main.cpp
	$(CXX) $(CXXFLAGS) $(OBJS) $(INCLUDES) -g src/main.cpp -o starspace

apps: query_nn query_predict print_ngrams embed_doc cf_predict

query_nn: $(OBJS) src/apps/query_nn.cpp
	$(CXX) $(CXXFLAGS) $(OBJS) $(INCLUDES) -g src/apps/query_nn.cpp -o query_nn

query_predict: $(OBJS) src/apps/query_predict.cpp
	$(CXX) $(CXXFLAGS) $(OBJS) $(INCLUDES) -g src/apps/query_predict.cpp -o query_predict

print_ngrams: $(OBJS) src/apps/print_ngrams.cpp
	$(CXX) $(CXXFLAGS) $(OBJS) $(INCLUDES) -g src/apps/print_ngrams.cpp -o print_ngrams

embed_doc: $(OBJS) src/apps/embed_doc.cpp
	$(CXX) $(CXXFLAGS) $(OBJS) $(INCLUDES) -g src/apps/embed_doc.cpp -o embed_doc

cf_predict: $(OBJS) src/apps/cf_predict.cpp
	$(CXX) $(CXXFLAGS) $(OBJS) $(INCLUDES) -g src/apps/cf_predict.cpp -o cf_predict

$(STATIC_LIB): $(OBJS)
	ar -crv $@ $(OBJS)

test: $(TESTS)

lib: $(STATIC_LIB)

clean:
	rm -rf *.o starspace gtest.a gtest_main.a *_test query_nn query_predict print_ngrams embed_doc cf_predict
