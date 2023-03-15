all: main init remove

main: ./src/main.c
	clang -lcurses -lsqlite3 ./src/main.c -Wall -o ./out/main.out

init: ./db/init.py
	python3 ./db/init.py

remove: ./db/log.txt
	rm ./db/log.txt

run: ./out/main.out
	./out/main.out

clean: ./out/main.out
	rm ./out/main.out
