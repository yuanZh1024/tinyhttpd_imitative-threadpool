all: httpd

httpd: simpleclient.c
	gcc -W -Wall -lpthread -o client simpleclient.c

clean:
	rm client
