CC := g++
CFLAGS := -g --std=c++11
INCLUDE = -I/usr/local/include
INCLUDE += -I/www/server/mysql/include
LDLIBS  = -lpthread -lmysqlclient -lprotobuf
LIB_PATH = -L/usr/local/lib
LIB_PATH =  -L/www/server/mysql/lib
TARGET := ../bin/ygamesvr
SRCS := $(wildcard *.cpp ./Common/Event/*.cpp ./Common/Task/*.cpp ./Common/Thread/*.cpp ./Common/Network/*.cpp ./Common/Log/*.cpp ./Common/Coroutine/*.cpp ./Common/DataBase/*.cpp ./Resource/*.cpp ./Protocol/*.cpp)
OBJS := $(SRCS:.cpp=.o)
all:$(TARGET)
$(OBJS) : %.o: %.cpp
	$(CC) -c $(CFLAGS) $(INCLUDE) $< -o $@
$(TARGET) : $(OBJS) $(CCOBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LIB_PATH) $(LDLIBS)
clean:
	rm -rf $(TARGET) *.o ./Common/Event/*.o ./Common/Task/*.o ./Common/Thread/*.o ./Common/Network/*.o ./Common/Log/*.o ./Common/Coroutine/*.o ./Common/DataBase/*.o ./Resource/*.o ./Protocol/*.o
