#include <stdio.h>


int main()
{
	int i,j,k;
	int arrayA[100];
	int max = 0;
	int temp;
	int n;
	scanf("%d", &n);	//输入数组长度
	for (i = 0; i < n; ++i) {
		scanf("%d", &arrayA[i]);	//定义数组元素
	}
	for (i = 0; i < n; ++i) {
		for (j = i; j < n; ++j) {
			//截取第i到第j个元素
			int temp = 0;
			for (k = i; k <= j; k++) {
				//对第i到第j个元素求和
				temp += arrayA[k];
			}
			if (temp > max) {
				//更新最大片段的值
				max = temp;
			}
		}
	}
	printf("%d\n", max);
    return 0;
}
