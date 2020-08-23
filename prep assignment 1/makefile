all: mycopy myremove intsort intsort_fn_ptr sort sort_fn_temp
	@echo "all files ready"

mycopy: src/mycopy.c
	gcc src/mycopy.c -o mycopy

myremove: src/myremove.c
	gcc src/myremove.c -o myremove

intsort: src/intsort.c
	gcc src/intsort.c -o intsort

intsort_fn_ptr: src/intsort_fn_ptr.c
	gcc src/intsort_fn_ptr.c -o intsort_fn_ptr

sort: src/sort.cpp
	g++ src/sort.cpp -o sort

sort_fn_temp: src/sort_fn_temp.cpp
	g++ src/sort_fn_temp.cpp -o sort_fn_temp