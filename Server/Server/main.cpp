#include <iostream>
#include "AbsIMediator.h"

using namespace std;

int main()
{
	CABsIMediator mediator;

	mediator.Open();

	mediator.Run();

	mediator.Close();

	cout<<"成功关闭该服务器"<<endl;
	system("pause");
	return 0;
}