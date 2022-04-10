matlang2c: main.o eval.o assignment_declaration.o matlang_functions.o globals.o
	gcc main.o eval.o assignment_declaration.o matlang_functions.o globals.o -o matlang2c -w

main.o: main.c eval.c matlang_functions.c globals.c structs.h
	gcc -c main.c -w

eval.o: eval.c assignment_declaration.c matlang_functions.c globals.c structs.h
	gcc -c eval.c -w

assignment_declaration.o: assignment_declaration.c globals.c structs.h
	gcc -c assignment_declaration.c -w

matlang_functions.o: matlang_functions.c globals.c structs.h
	gcc -c matlang_functions.c -w

globals.o: globals.c structs.h
	gcc -c globals.c -w

clean:
	rm *.o matlang2c