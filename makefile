IDIR =../include
CC=gcc
CFLAGS= -g -Wall

all: encode

encode: encodeUTF16.c
	$(CC) $(CFLAGG) encodeUTF16.c -o encodeUTF16

testCheck:
	./encodeUTF16 utf32-val-7a-le tmp
	cmp tmp utf16-val-7a-le
	./encodeUTF16 utf32-val-7a-be tmp
	cmp tmp utf16-val-7a-be

	./encodeUTF16 utf32-val-760-le tmp
	cmp tmp utf16-val-760-le
	./encodeUTF16 utf32-val-760-be tmp
	cmp tmp utf16-val-760-be

	./encodeUTF16 utf32-val-a999-le tmp
	cmp tmp utf16-val-a999-le
	./encodeUTF16 utf32-val-a999-be tmp
	cmp tmp utf16-val-a999-be

	./encodeUTF16 utf32-val-12345-le tmp
	cmp tmp utf16-val-12345-le
	./encodeUTF16 utf32-val-12345-be tmp
	cmp tmp utf16-val-12345-be

	./encodeUTF16 utf32-100-vals-be tmp
	cmp utf16-100-vals-be tmp
	./encodeUTF16 utf32-100-vals-le tmp
	cmp utf16-100-vals-le tmp

	./encodeUTF16 utf32-100-vals-be-test tmp

	./encodeUTF16 utf32-val-12345-be-test tmp
	cmp tmp utf16-val-12345-be-test

ta: testErr

testErr:
	od -tx1 utf32.errA
	./encodeUTF16 utf32.errA tmp

currentTest:

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
