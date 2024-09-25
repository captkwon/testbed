#include <iostream>
using namespace std;

int main()
{
	int a=10, b=100;
	int *ptr = &a;

	cout << "ptr이 가리키는 곳의 값 : " << *ptr << endl;
	return 0;
}
