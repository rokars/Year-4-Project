
#include <stdio.h>
#include <stdlib.h>

void Pointers1();
void Pointers2();
void Pointers3();
void Increment(int*);
void Pointers4();
void Pointers5();
void Pointers6();
int SumOfElements(int*, int);
void Double(int*, int);
void Pointers7();
int* Add(int* a, int* b);
void PrintHelloWorld();

int main() {

	Pointers7();
}

void Pointers1() {
	int a = 10;
	int* p;
	p = &a;
	// Pointer arithmetic
	printf("Address p is %d\n", p);
	printf("value at address p is %d\n", *p);
	printf("Size of integer is %d byte\n", sizeof(int));
	printf("Address p+1 is %d\n", p + 1);
	printf("value at address p+1 is %d\n", *(p + 1));

	/*int a = 10;
	int* p;		//int* p = &a;

	p = &a; // &a = address of a
	printf("Address of P is %d\n", p);
	printf("Value at p is %d\n", *p);
	*p = 12;	// dereferencing
	int b = 20;
	*p = b;
	printf("Address of P is %d\n", p);
	printf("Value at p is %d\n", *p);
	printf("%d\n", p);
	printf("%d\n", *p); // *p - value at address pointed by p
	printf("%d\n", &a);*/
}

void Pointers2() {
	int a = 1025;
	int* p;
	p = &a;
	printf("size of integer is %d bytes\n", sizeof(int));
	printf("Address = %d, value = %d\n", p, *p);
	printf("Address = %d, value = %d\n", p + 1, *(p + 1));
	char* p0;
	p0 = (char*)p;	// type casting
	printf("size of char is %d bytes\n", sizeof(char));
	printf("Address = %d, value = %d\n", p0, *p0);
	printf("Address = %d, value = %d\n", p0 + 1, *(p0 + 1));
	// 1025 = 00000000 00000000 00000100 00000001

	// void pointer - generic pointer
	void* p1;
	p1 = p;
	printf("Address = %d", p1);
}

void Pointers3() {

	int a;
	a = 10;
	Increment(&a);		// call by reference
	printf("a = %d", a);
	//printf("Address of variable a in increment %d\n", &a);
}

void Increment(int* p) {
	*p = (*p) + 1;
	//printf("Address of variable a in increment %d\n", &p);
}

void Pointers4() {
	int A[] = { 2,4,5,8,1 };
	int i;
	int* p = A;
	p++;
	for (i = 0; i < 5; i++) {
		printf("Address = %d\n", A + i);
		printf("Address = %d\n", &A[i]);
		printf("Value = %d\n", A[i]);
		printf("Value = %d\n", *(A + i));
	}
	//printf("%d\n", A);
	//printf("%d\n", &A[0]);
	//printf("%d\n", A[0]);
	//printf("%d\n", *A);
}

void Pointers5() {
	int n;
	printf("Enter size of array\n");
	scanf_s("%d", &n);
	int* A = (int*)malloc(n * sizeof(int));	// dynamically allocated array
	//int* A = (int*)calloc(n , sizeof(int));	// dynamically allocated array
	for (int i = 0; i < n; i++)
	{
		A[i] = i + 1;
	}
	//free(A);
	//A = NULL;	// after free, adjust pointer to NULL;
	int* B = (int*)realloc(A, 2 * n * sizeof(int));
	printf("Previous block address = %d, new address = %d\n", A, B);
	for (int i = 0; i < 2 * n; i++) 
	{
		printf("%d ", B[i]);
	}
}

void Pointers6() {
	// Arrays as function arguments
	int A[] = { 1,2,3,4,5 };
	int size = sizeof(A) / sizeof(A[0]);
	int i;
	int total = SumOfElements(A, size);	// A can be used for &A[0]
	printf("Sum of elements = %d\n", total);
	printf("Main - Size of A = %d, size of A[0] = %d\n", sizeof(A), sizeof(A[0]));

	Double(A, size);
	for (i = 0; i < size; i++) {
		printf("%d ", A[i]);
	}
}

int SumOfElements(int* A, int size)
{
	int i, sum = 0;
	printf("SOE - Size of A = %d, size of A[0] = %d\n", sizeof(A), sizeof(A[0]));
	for (i = 0; i < size; i++) 
	{
		sum += A[i];	// A[i] is *(A+i)
	}
	return sum;
}

void Double(int* A, int size)
{
	int i, sum = 0;
	for (i = 0; i < size; i++)
	{
		A[i] = 2 * A[i];
	}
}

void Pointers7() {	// calling function
	int a = 2, b = 4;
	// call by value
	int* ptr = Add(&a, &b);
	PrintHelloWorld();
	printf("Sum = %d\n", *ptr);
}

int* Add(int* a, int* b) {		// calling function - returns pointer to integer
	int* c = (int*)malloc(sizeof(int));
	*c = (*a) + (*b);
	return c;
}

void PrintHelloWorld() {
	printf("Hello World\n");
}