
all: src/non_blocked_ijk.c src/blocked_ijk_atlas.c src/blocked_kij_atlas.c src/blas_routine.c
	gcc -o out/non_blocked_ijk.out src/non_blocked_ijk.c 
	gcc -o out/blocked_ijk_atlas.out src/blocked_ijk_atlas.c -lblas -latlas -lm -O3
	gcc -o out/blocked_kij_atlas.out src/blocked_kij_atlas.c -latlas -lblas -lm -O3
	gcc -o out/blas_routine.out src/blas_routine.c -lblas -lm -O3

clean:
	rm *.out