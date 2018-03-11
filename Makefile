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

define EXTRA_CONF_FILE
def FlagsForFile( filename, **kwargs ):
  return {
    'flags': '$(CXXFLAGS)'.split( ' ' )
  }
endef

all: .ycm_extra_conf.py ${TARGET} 

${TARGET}: ${OBJECTS}
	${CXX} ${LDFLAGS} $< ${LOADLIBES} ${LDLIBS} -o ${TARGET}

export EXTRA_CONF_FILE
.ycm_extra_conf.py: Makefile
	echo "$$EXTRA_CONF_FILE" > $@


clean:
	rm -f ${OBJECTS}
	rm -f ${TARGET}

