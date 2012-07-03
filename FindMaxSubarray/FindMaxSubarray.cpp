// FindMaxSubarray.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <map>
#include <iostream>

using namespace std;

typedef struct ReturnValue_tag
{
	int low;
	int high;
	int maxValue;
} RetrunValue;

RetrunValue find_maximum_subarray_brute_force(int* begin, int length)
{
	int max = 0;
	int low = 0;
	int high = 0;

	for(int i=0; i<length-1; i++)
	{
		int sum = 0;
		for( int j=i; j<length; j++ )
		{
			sum += *(begin+j);
			if(sum > max)
			{
				low = i;
				high = j;
				max = sum;
			}
		}
	}

	RetrunValue result;
	result.low = low;
	result.high = high;
	result.maxValue = max;
	return result;
}

RetrunValue find_max_crossing_subarray(int* array, int begin, int mid, int end)
{
	RetrunValue result;	

	int left = 0x80000000;
	int sum = 0;
	for(int i=mid; i>=begin; i--)
	{
		sum += *(array+i);
		if( sum > left )
		{
			left = sum;
			result.low = i;
		}
	}

	int right = 0x80000000;
	int sumRight = 0;
	for(int j=mid+1; j<=end; j++)
	{
		sumRight += *(array+j);
		if(sumRight>right)
		{
			right = sumRight;
			result.high = j;
		}
	}
	result.maxValue = left + right;
	return result;
}

RetrunValue find_maximum_subarray(int* array, int start, int end)
{
	RetrunValue result;
	if(start==end)
	{
		result.low = start;
		result.high = end;
		result.maxValue = *(array+start);
		return result;
	}
	int mid = (start+end) / 2;
	RetrunValue leftResult = find_maximum_subarray(array, start, mid);
	RetrunValue rightResult = find_maximum_subarray(array, mid+1, end);
	RetrunValue crossResult = find_max_crossing_subarray(array, start, mid, end);

	if(leftResult.maxValue > rightResult.maxValue && leftResult.maxValue > crossResult.maxValue)
		return leftResult;
	else if( rightResult.maxValue > crossResult.maxValue )
		return rightResult;
	else
		return crossResult;
}

RetrunValue find_maximum_subarray_liner(int* array, int start, int end)
{
	RetrunValue result;
	result.maxValue = 0x80000000;
	result.low = start;
	result.high = start;

	int max_start = start;
	int sum = *(array+start);
	for(int i = start+1; i<=end; i++)
	{
		sum+=array[i];
		if( array[i] > sum)
		{   
			max_start = i;
			sum = array[i];
		}
		if(sum>result.maxValue)
		{   
			result.low = max_start;
			result.maxValue = sum;
			result.high = i;    
		}
	}
	return result;
}

int _tmain(int argc, _TCHAR* argv[])
{
	//int array[16] = { 13, -3, -25, 20, -3, -16, -23, 18, 20, -7, 12, -5, -22, 15, -4, 7 };
	//int length = 16;

	std::map<int, int> nums;
	int length = 20000000;
	int* array = new int[length];
	srand(time(NULL));
	for(int i=0; i<length; i++)
	{
		*(array+i) = rand() % 20000 - 10000;
		//printf("%d\n", *(array+i));
		//Sleep(1);

		//std::pair<std::map<int,int>::iterator, bool> ret = nums.insert(std::pair<int,int>(*(array+i), 0));
		//if(!ret.second)
		//{
		//	//printf("\n");
		//}
	}

	LARGE_INTEGER litmp;   
	double dfFreq;   
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;//   获得计数器的时钟频率

	LARGE_INTEGER start, stop1, stop2;
	QueryPerformanceCounter(&start);
	RetrunValue result = find_maximum_subarray_liner(array, 0, length-1);
	//RetrunValue result = find_maximum_subarray_brute_force(array, length);
	printf("brute-force max: %d, from %d to %d\n", result.maxValue, result.low, result.high);

	QueryPerformanceCounter(&stop1);

	result = find_maximum_subarray(array, 0, length-1);
	printf("divide-and-conquer max: %d, from %d to %d\n", result.maxValue, result.low, result.high);

	QueryPerformanceCounter(&stop2);

	LONGLONG QPart1 = start.QuadPart;
	LONGLONG QPart2 = stop1.QuadPart;
	LONGLONG QPart3 = stop2.QuadPart;

	double duration1,duration2;   
	duration1 = (double)(QPart2 - QPart1);
	duration2 = (double)(QPart3 - QPart2);

	double dfMinus1 = duration1/dfFreq;
	double dfMinus2 = duration2/dfFreq;

	cout << "find By bute force: " << dfMinus1 << endl;
	cout << "find By divide-and-conquer: " << dfMinus2 << endl;

	delete[] array;
	getchar();
	return 0;
}

