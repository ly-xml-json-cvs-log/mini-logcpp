/*************************************************************************
  > File Name: test_delete.cpp
  > Author: tiankonguse(skyyuan)
  > Mail: i@tiankonguse.com 
  > Created Time: 2015年05月30日 星期六 14时29分46秒
***********************************************************************/

#include<iostream>
#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<string>
#include<queue>
#include<map>
#include<cmath>
#include<stack>
#include<algorithm>
#include<functional>
#include<stdarg.h>
using namespace std;
#ifdef __int64
typedef __int64 LL;
#else
typedef long long LL;
#endif
int main() {
    int*p = new int(2);
    printf("p=%p %d\n",p,*p);
    delete p;
    printf("p=%p %d\n",p,*p);

    return 0;
}
