CC=gcc 
CPP=g++ -std=c++0x
NVCPP=nvcc -std=c++11 -Wno-deprecated-gpu-targets
CFLAGS=-c
DIR:=${CURDIR}
EXEC:=$(DIR)/Executables
INT:=$(EXEC)/Intermediates
NVLFLAGS=-L/usr/local/cuda-8.0/lib64 -lcudart
SRC=$(DIR)/src
OBJS=$(INT)/structure.cpp.o $(INT)/main.o $(INT)/log.o $(INT)/docker.cpp.o $(INT)/docker.cu.o

all: $(OBJS)
	$(CPP) $(OBJS) -o $(EXEC)/docker $(NVLFLAGS)

$(INT)/%.cpp.o: $(SRC)/%.cpp $(SRC)/%.h
	$(CPP) $(CFLAGS) $< -o $@

$(INT)/%.o: $(SRC)/%.cpp
	$(CPP) $(CFLAGS) $< -o $@
	
$(INT)/%.cu.o: $(SRC)/%.cu $(SRC)/%.h
	$(NVCPP) $(CFLAGS) $< -o $@
	
clean: 
	rm $(INT)/*.o $(EXEC)/docker


	
