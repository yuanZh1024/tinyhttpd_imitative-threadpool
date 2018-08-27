all: httpd

httpd: httpd.c
	gcc -W -Wall -lpthread -pthread -o httpd httpd.c

clean:
	rm httpd
