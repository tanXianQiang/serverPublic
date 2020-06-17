

#include <stdio.h>
#include <stdlib.h>
#include <string.h>




#if 0

/*
参考文档
  https://blog.csdn.net/v_JULY_v/article/details/7041827?utm_medium=distribute.pc_relevant_right.none-task-blog-BlogCommendFromMachineLearnPai2-2.nonecase&depth_1-utm_source=distribute.pc_relevant_right.none-task-blog-BlogCommendFromMachineLearnPai2-2.nonecase
*/


int ViolentMatch(char* s, char* p)
{
	int sLen = strlen(s);
	int pLen = strlen(p);
 
	int i = 0;
	int j = 0;
	while (i < sLen && j < pLen)
	{
		if (s[i] == p[j])
		{
			//①如果当前字符匹配成功（即S[i] == P[j]），则i++，j++    
			i++;
			j++;
		}
		else
		{
			//②如果失配（即S[i]! = P[j]），令i = i - (j - 1)，j = 0    
			i = i - j + 1;
			j = 0;
		}
	}
	//匹配成功，返回模式串p在文本串s中的位置，否则返回-1
	if (j == pLen)
		return i - j;
	else
		return -1;




	
}

/*
s

		0  1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  27  28  29  30  31  32  
		a  b   a   b   x   b   a   b   a   b   a   b   a   b   c   d   a   b   a   b   c   a   b   d   d   c   a   d   f   d   s   s   s
																																	   sLen
		       i																														       
																				
p		
		0	1	2	3	4	5
		a	b	c	a	b	d
		j
							pLen	


 i  和  j  是同步索引s和p, 当不等时候i-j 回到同步位置， 加1从下面一个位置进行匹配。 
																																		

*/



int KmpSearch(char* s, char* p)
{
	int i = 0;
	int j = 0;
	int sLen = strlen(s);
	int pLen = strlen(p);
	while (i < sLen && j < pLen)
	{
		//①如果j = -1，或者当前字符匹配成功（即S[i] == P[j]），都令i++，j++    
		if (j == -1 || s[i] == p[j])
		{
			i++;
			j++;
		}
		else
		{
			//②如果j != -1，且当前字符匹配失败（即S[i] != P[j]），则令 i 不变，j = next[j]    
			//next[j]即为j所对应的next值      
			j = next[j];
		}
	}
	if (j == pLen)
		return i - j;
	else
		return -1;
}





/*
   不等时候，i没有向后移动。 如next初值可会向后移动。 

*/





void GetNext(char* p,int next[])
{
	int pLen = strlen(p);
	next[0] = -1;
	int k = -1;
	int j = 0;
	while (j < pLen - 1)
	{
		//p[k]表示前缀，p[j]表示后缀
		if (k == -1 || p[j] == p[k]) 
		{
			++k;
			++j;
			next[j] = k;
		}
		else 
		{
			k = next[k];
		}
	}
}




/*
p
-1	0	1	2	3	4	5	6 7
	A	B	C	D	A	B	D E

                            j
k

next	
	-1   0	 0  0   0   1    2

*/











#endif











void make_next(const char *pattern, int *next) {

	int q, k;
	int m = strlen(pattern);

	next[0] = 0;
	for (q = 1,k = 0;q < m; q ++) {

		while (k > 0 && pattern[q] != pattern[k])
			k = next[k-1];

		if (pattern[q] == pattern[k]) {
			k ++;
		}

		next[q] = k;

	}

     
/*
  	 0	1	2	3	4	5	6	7	8	9	10	11	12	13	14	15	16	17	18	19	20	21	22	23	24	25	26	27	28	29	30	31	32	
     a	b	a	b	x	b	a	b	a	b	a	b	a	b	c	d	a	b	a	b	c	a	b	d	d	c	a	d	f	d	s	s	s

 	 0	1	2	3	4	5
	 a	b	c	a	b	d

     
     	                q
            k
					    m


    next[1]  = 0
    next[2]  = 0
    next[3]  = 1
    next[4]  = 2
    

*/



	// next[0] = 0;
	// q=1, k=0, pattern[q]:pattern[k] = b:a, next[1] = 0;
	// q=2, k=0, pattern[q]:pattern[k] = c:a, next[2] = 0;
	// q=3, k=0, pattern[q]:pattern[k] = a:a, k++, next[3] = 1;
	// q=4, k=1, pattern[q]:pattern[k] = b:b, k++, next[4] = 2;
	// q=5, k=2, pattern[q]:pattern[k] = c:c, k++, next[5] = 3;
	// q=6, k=3, pattern[q]:pattern[k] = d:a, k=next[k-1] -> k=0; next[6] = 0;

}


int kmp(const char *text, const char *pattern, int *next) {

	int n = strlen(text);
	int m = strlen(pattern);

	make_next(pattern, next);
	
	int i, q;
	for (i = 0, q = 0;i < n;i ++) {

         // q > 0 表明有匹配数据，不回溯，根据next的前后缀，匹配下一个。 
		while (q > 0 && pattern[q] != text[i]) {
			q = next[q-1];
		}

		if (pattern[q] == text[i]) {
			q ++;
		}

		if (q == m) {
			//printf("Pattern occurs with shift: %d\n", (i-m+1));
			break;
		}
	}

	return i-q+1;
}

/*
text
		0  1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  27  28  29  30  31  32  
		c  b   a   b   x   b   a   b   a   b   a   b   a   b   c   d   a   b   a   b   c   a   b   d   d   c   a   d   f   d   s   s   s
																																       n 
																				
pattern		
		0	1	2	3	4	5
		a	b	c	a	b	d
							m	
next   
		0	1	2	3
		0	0	1	2					
        i
        q
																																		

*/




int main() {

	int i;
	int next[20] = {0};

	char *text = "cbabxbababababcdababcabddcadfdsss";
	char *pattern = "abcabd";

	int idx = kmp(text, pattern, next);
	printf("match pattern : %d\n", idx);

	for (i = 0;i < strlen(pattern);i ++) {
		printf("%4d", next[i]);
	}
	printf("\n");

	return 0;

}



