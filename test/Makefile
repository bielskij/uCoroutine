TEST_EXE  := test.elf

DIR_OUT := out
DIR_SRC := src

GTEST_DIR := googletest/googletest

GTEST_OBJS :=
GTEST_SRCS := \
	$(GTEST_DIR)/src/gtest-all.cc

TEST_OBJS :=
TEST_SRCS := $(find src -name *.cpp)




CPPFLAGS += -I$(GTEST_DIR)/include
CXXFLAGS += -g -Wall -Wextra -pthread


all: $(TEST_EXE)

clean:
	rm -f $(TEST_EXE)

$(TEST): $(TEST).a