CC=gcc

all: clean mycat mygrep

mycat:
	$(MAKE) -C mycat $@

mygrep:
	$(MAKE) -C mygrep $@

test: mycat mygrep
	./test_mycat.sh
	./test_mygrep.sh

cppcheck:
	cppcheck -q --enable=performance,portability,warning,style mycat/*.c
	cppcheck -q --enable=performance,portability,warning,style mycat/*.h
	cppcheck -q --enable=performance,portability,warning,style mygrep/*.c
	cppcheck -q --enable=performance,portability,warning,style mygrep/*.h

clean:
	$(MAKE) -C mycat $@
	$(MAKE) -C mygrep $@
	rm -f *.out *_diff .cfg

fclean: clean
	$(MAKE) -C mycat $@
	$(MAKE) -C mygrep $@
