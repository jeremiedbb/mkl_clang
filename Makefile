CC          = clang-8
CFLAGS      = -Wall -fopenmp
LDFLAGS     = -shared -fopenmp $(LIBS)
LIBS        = -lmkl_rt
# CC          = /home/jeremie/R/C/llvm-project/build/bin/clang
# CFLAGS      = -Wall -I/home/jeremie/R/C/llvm-project/build/projects/openmp/runtime/src
# LDFLAGS     = -shared -L/home/jeremie/R/C/llvm-project/build/lib -Wl,-rpath=/home/jeremie/R/C/llvm-project/build/lib
# LIBS        = -lomp -lmkl_rt
LINKER      = $(CC)

nothing:

c: clean_c build_c test_c

build_c:
	@echo "Build\n"
	$(CC) $(CFLAGS) -fPIC -c -o C/cmodule.o C/cmodule.c
	$(LINKER) $(LDFLAGS) -o C/libcmodule.so C/cmodule.o
	$(CC) -Wall -IC -c -o C/ctest.o C/ctest.c 
	$(LINKER) -LC -Wl,-rpath=C -o C/ctest C/ctest.o -ldl

test_c:
	@echo "\n\nTest\n"
	C/ctest

clean_c:
	rm -f C/cmodule.o C/libcmodule.so C/ctest.o C/ctest

cy: clean_cy build_cy test_cy

build_cy:
	@bash -c "pushd CY/ &&\
			  echo 'Build\n' &&\
			  CC=$(CC) CFLAGS='-fopenmp' LDFLAGS='$(LDFLAGS)' python setup.py build_ext -i &&\
			  popd"

test_cy:
	@echo "\n\nTest\n"
	python CY/cytest.py

clean_cy:
	rm -rf CY/build
	rm -f CY/cymodule.c CY/cymodule.cp*

cp: clean_cp build_cp test_cp

build_cp:
	@echo "Build\n"
	$(CC) $(CFLAGS) -I/home/jeremie/miniconda/envs/mkl-segfault/include/python3.7m -fPIC -c -o CP/cpmodule.o CP/cpmodule.c
	$(LINKER) $(LDFLAGS) -o CP/cpmodule.so CP/cpmodule.o

test_cp:
	@echo "\n\nTest\n"
	python CP/cptest.py

clean_cp:
	rm -f CP/cpmodule.o CP/cpmodule.so

clean: clean_c clean_cy clean_cp
