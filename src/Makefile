CC=g++

obj=Server.o ServerMain.o

Server : $(obj)
	$(CC) $(obj) -o $@

Server.o : Server.cc
	$(CC) -c $^

ServerMain.o : ServerMain.cc
	$(CC) -c $^

clean :
	rm $(obj) Server
.PHONY :
	clean
