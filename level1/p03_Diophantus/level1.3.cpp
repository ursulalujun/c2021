#include <stdio.h>

int main()
{
	int m=0,i=0,k;
	do
	{
		i++;
		if(i%12==0&&i%7==0&&(i/6+i/12+i/7+5+i/2+4)==i)//ֱ����ѭ���ⷽ�̳���ȡ�����õ�����������Ҫ��֤������6,12,7�Ĺ�����
		{
			m=i;
		}
	}
	while(m!=i);
	printf("������ʱ����ͼ��������%d",m-4);
} 