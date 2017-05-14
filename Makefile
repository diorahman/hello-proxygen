server: Server.cpp HelloHandler.o
	g++ -std=c++14 Server.cpp HelloHandler.o -o server -lproxygenhttpserver -lfolly -lglog -pthread

HelloHandler.o: HelloHandler.cpp
	g++ -std=c++14 -c HelloHandler.cpp

clean:
	rm -fr server *.o
