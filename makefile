TARGETS = myShell

all: $(TARGETS)

%o: %.c
	gcc -g -c -o  $@ $<
myShell: input.o cd.o echo.o pwd.o pinfo.o ls.o execute.o main.o envvar.o jobs.o kjob.o overkill.o
	gcc -o myShell input.o cd.o echo.o pwd.o pinfo.o ls.o execute.o main.o envvar.o jobs.o kjob.o overkill.o

.PHONY: clean
clean:
	rm -f $(TARGETS) *.o
