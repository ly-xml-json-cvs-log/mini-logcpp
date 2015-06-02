CC   = gcc
CXX  = g++

### project path
PROJECT_PATH = ..

include ${PROJECT_PATH}/lib/makefile

C_FLAGS += -O2 
C_FLAGS += -fno-strict-aliasing
C_FLAGS += -Wall -Wno-unused -Wno-missing-field-initializers  -Wno-missing-braces
C_FLAGS += -g 
C_FLAGS += -z 

INC = -I../include/ ${INC_THREAD_LOCK}
LIB = -lz -lrt -lpthread ${LIB_THREAD_LOCK}

SOURCE = $(wildcard *.cpp)
OBJS    = $(patsubst %.cpp,%.o,$(SOURCE))

OUTPUT  := ../lib/libmini-logcpp.a

all: $(OUTPUT)

.SUFFIXES: .o .cpp

$(OUTPUT):$(OBJS)
	 ar -rs $@ $(EX_LIB) $^

.cpp.o:
	g++ $(C_FLAGS) $(LIB) $(INC) -c  $^ -o $(patsubst %.cpp,%.o,$^)

clean:
	rm -f $(OBJS)
