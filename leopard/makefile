LDFLAGS=
# LDFLAGS+=-llibgo 
LDFLAGS+=-lpthread -ldl 
LIFLAGS=-I.

SRCS=app.cpp
SRCS+=log.cpp
SRCS+=Epoll.cpp
SRCS+=Socket.cpp
SRCS+=Exception.cpp
SRCS+=Leopard.cpp
SRCS+=Runway.cpp
SRCS+=LeopardConfig.cpp
SRCS+=cjson.cpp

app : ${SRCS}
	g++ -o $@ $^ ${LDFLAGS} ${LIFLAGS} -g -ggdb -w

client : client.cpp Socket.cpp Exception.cpp LeopardConfig.cpp log.cpp cjson.cpp Epoll.cpp LaneClient.cpp
	g++ -o $@ $^ ${LDFLAGS} ${LIFLAGS} -g -ggdb -w

clean:
	rm -rf app
	rm -rf test
	rm -rf server
	rm -rf client
	rm -rf client2

run:
	./app

# -ldl的作用：https://www.cnblogs.com/SZxiaochun/p/7718621.html