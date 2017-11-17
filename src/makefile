CC := g++
CFLAGS := -g --std=c++11
LDLIBS  =  -lpthread
TARGET := ygamesvr
SRCS := $(wildcard *.cpp ./Common/Event/*.cpp ./Common/Task/*.cpp ./Common/Thread/*.cpp ./Common/Network/*.cpp ./Common/Log/*.cpp ./Common/Coroutine/*.cpp)
OBJS := $(SRCS:.cpp=.o)
all:$(TARGET)
$(OBJS) : %.o: %.cpp  
	$(CC) -c $< -o $@ $(CFLAGS)
$(TARGET) : $(OBJS)   
	$(CC) -o $(TARGET) $(OBJS) $(LDLIBS)
clean:
	rm -rf $(TARGET) *.o ./Common/Event/*.o ./Common/Task/*.o ./Common/Thread/*.o ./Common/Network/*.o ./Common/Log/*.o ./Common/Coroutine/*.o
