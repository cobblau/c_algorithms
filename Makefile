CFLAGS=
LDFLAGS=
DEBUG=-g
INC = -I src/ \
	  -I src/hash


all: test/test_list

test/test_list: test/test_list.c 
	gcc -o $@ $^ $(INC)

clean:
	find test/ -type f -perm 755 -exec rm {} \;

