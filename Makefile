CFLAGS=
LDFLAGS=
DEBUG=-g
INC = -I src/ \
	  -I src/hash


all: test/test_list \
	 test/test_rbtree \
	 test/test_ip_radix_tree \
	 test/test_hash

test/test_hash: test/test_hash.c src/hash/hash.c src/hash/hash_functions.c
	gcc -o $@ $^ $(INC)

test/test_list: test/test_list.c 
	gcc -o $@ $^ $(INC)

test/test_rbtree: test/test_rbtree.c src/rbtree.c
	gcc -o $@ $^ $(INC)

test/test_ip_radix_tree: test/test_ip_radix_tree.c src/ip_radix_tree.c
	gcc -o $@ $^ $(INC)

clean:
	find test/ -type f -perm 755 -exec rm {} \;
	rm -rf src/*.o

