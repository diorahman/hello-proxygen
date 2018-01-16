UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
	INC += -I/usr/local/include -I/usr/local/opt/openssl/include
	LIB += -L/usr/local/lib -L/usr/local/opt/openssl/lib
endif

server: Server.cpp HelloHandler.o
	g++ -std=c++14 $(INC) Server.cpp HelloHandler.o -o server $(LIB) -lproxygenhttpserver -lfolly -lglog -pthread

HelloHandler.o: HelloHandler.cpp
	g++ -std=c++14 $(INC) -c HelloHandler.cpp

clean:
	rm -fr server *.o
