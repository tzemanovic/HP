// Includes
#include <functional>
#include <iostream>

// Raw function pointer in C++
void( *fnPtr1 )( int );

// Function wrapper from STL
std::function<void( int )> fnPtr2;

// Partial application in C++
void f( int n1, int n2, int n3 )
{
	std::cout << n1 << ' ' << n2 << ' ' << n3 << std::endl;
}
int main( )
{
	auto f1 = std::bind( f, std::placeholders::_2,
		std::placeholders::_1, 3 );
	f1(1, 2);
}

// Tail-recursive function
unsigned int factorial( const unsigned int x )
{
	if ( x == 0 )
	{
		return 0;
	}
	return factorial( x - 1 );
}

// Lambda function for adding two numbers
auto add = []( const int a, const int b )
{
	return a + b;
};

// Closure that captures a value of variable outside its scope
int five = 5;
auto addToFive = [five]( const int x )
{
	return x + five;
}