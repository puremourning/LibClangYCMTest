# Configuration
#
# PATH_TO_LLVM_ROOT (mandatory) = Directory where llvm/clang is installed
#

PATH_TO_LLVM_ROOT ?= $(shell llvm-config --prefix)

TARGET = YCMTest
OBJECTS = main.o
CXXFLAGS += -x c++ -std=c++11 -Wall -Wextra -Werror -I${PATH_TO_LLVM_ROOT}/include
LDFLAGS += -L${PATH_TO_LLVM_ROOT}/lib
LDLIBS += -lclang

${TARGET}: ${OBJECTS}
	${CXX} ${LDFLAGS} $< ${LOADLIBES} ${LDLIBS} -o ${TARGET}


clean:
	rm -f ${OBJECTS}
	rm -f ${TARGET}

