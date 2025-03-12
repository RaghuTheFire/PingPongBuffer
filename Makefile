all: pingpong
	chmod u+x ping-pong
	./ping-pong

clean:
	rm -f pingpong

pingpong:
	gcc -I../ -O3 -g -o ping-pong TestPingPong.cpp
