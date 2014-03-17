CFLAGS=
LDFLAGS=
DEBUG=-g
INC = -I src/ \
	  -I src/hash


all: test/test_list \
	 test/test_rbtree

test/test_list: test/test_list.c 
	gcc -o $@ $^ $(INC)

test/test_rbtree: test/test_rbtree.c src/rbtree.c
	gcc -o $@ $^ $(INC)

clean:
	find test/ -type f -perm 755 -exec rm {} \;
	rm -rf src/*.o

