all: U.data test
	./test

U.data: A.data get_U
	if [ ! -f U.data ]; then touch U.data; fi
	./get_U

A.data: get_A
	if [ ! -f A.data ]; then touch A.data; fi
	./get_A

get_A: f_coef.h parameter.h get_A.c
	gcc get_A.c -lm -o get_A

f_coef.h: config.py get_f_coef.py
	python3 get_f_coef.py > f_coef.h

get_U: parameter.h get_U.c
	gcc get_U.c -lm -o get_U

test: test.c parameter.h
	gcc test.c -lm -o test
