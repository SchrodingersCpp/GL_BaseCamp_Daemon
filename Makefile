CC=g++
CFLAGS=-c -Wall --std=c++14
EXECUTABLE=process_executor

all: main

main: main.o logger.o parser.o process_launcher.o
  $(CC) main.o logger.o parser.o process_launcher.o -o $(EXECUTABLE)

main.o: main.cc
  $(CC) $(CFLAGS) main.cc

logger.o: ilogger.h logger.h logger.cc
  $(CC) $(CFLAGS) ilogger.h logger.h logger.cc

parser.o: iparser.h parser.h parser.cc
  $(CC) $(CFLAGS) iparser.h parser.h parser.cc

process_launcher.o: iprocess_launcher.h process_launcher.h process_launcher.cc
  $(CC) $(CFLAGS) iprocess_launcher.h process_launcher.h process_launcher.cc

clean:
  rm -rf *.o *.gch $(EXECUTABLE)
