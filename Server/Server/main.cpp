#include <iostream>
#include "AbsIMediator.h"

using namespace std;

int main()
{
	CABsIMediator mediator;

	mediator.Open();

	mediator.Run();

	mediator.Close();

	cout<<"�ɹ��رո÷�����"<<endl;
	system("pause");
	return 0;
}