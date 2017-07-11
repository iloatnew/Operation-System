4a.o: 4a.c 4a.h bst.h
4b.o: 4b.c 4b.h 4a.h bst.h
4c.o: 4c.c 4c.h 4b.h 4a.h bst.h
bst.o: bst.c bst.h
test_4a.o: test_4a.c 4a.h bst.h
test_4b.o: test_4b.c 4b.h 4a.h bst.h
test_4c.o: test_4c.c 4c.h 4b.h 4a.h bst.h
