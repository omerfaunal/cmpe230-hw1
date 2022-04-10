matlang2c: main.o eval.o assignment_declaration.o matlang_functions.o globals.o
	gcc main.o eval.o assignment_declaration.o matlang_functions.o globals.o -o matlang2c -w

main.o: eval.c matlang_functions.c globals.c structs.h
	gcc -c main.c -w

eval.o: assignment_declaration.c matlang_functions.c globals.c structs.h
	gcc -c eval.c -w

assignment_declaration.o: globals.c structs.h
	gcc -c assignment_declaration.c -w

matlang_functions.o: globals.c structs.h
	gcc -c matlang_functions.c -w

globals.o: structs.h
	gcc -c globals.c -w

clean:
	rm *.o out.exe