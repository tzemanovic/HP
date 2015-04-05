// Includes
#include <functional>

// Raw function pointer in C++
void( *fnPtr1 )( int );

// Function wrapper from STL
std::function<void( int )> fnPtr2;

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