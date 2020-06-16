

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_ARRAY_LENGTH		12


int shell_sort(int *data, int length) {
    /* 步长 */
	int gap = 0;
	int i = 0, j = 0;
	int temp;

	for (gap = length / 2;gap >= 1; gap /= 2) {

		for (i = gap;i < length;i ++) {

			temp = data[i];
			for (j = i-gap;j >= 0 && temp < data[j];j = j - gap) {

				data[j+gap] = data[j];
				
			}
			data[j+gap] = temp;
		}

	}

	return 0;
}

/*   希尔
	   用笔演算
	   0		1		2		3		 4		 5	 	  6	   	 7	   	 8		 9		 10		 11
	   23		64		24		12		 9	 	16	  	 53	  	 57		 71		 79		 87		 97
gap 6	   

												       temp gap 

												          i ------------------------------------->|
												          
		j--------------------------------------->|	

		0		 1		 2		 3		  4 	  5 	   6	  7 	  8 	  9 	  10	  11
		23		 57 	 24 	 12 	  9 	 16 	  53	  64	  71	  79	  87	  97



gap  3                            i--------------------------------------------------------------->|
       j----------------------------------------------------------------->|


		0		 1		 2		 3		  4 	  5 	   6	  7 	  8 	  9 	  10	  11
		12		 9  	 16 	 23 	  57 	  24 	  53	  64	  71	  79	  87	  97



 gap  2 			    i------------------------------------------------------------------------->|
		j----------------------------------------------------------------------->|
 
 
		 0		  1 	  2 	  3 	   4	   5		6	   7	   8	   9	   10	   11
		 12 	  9 	  16	  23	   53	   24	   57	   64	   71	   79	   87	   97


 gap  1 	    i----------------------------------------------------------------------------------->|
		j---------------------------------------------------------------------------------->|
 
 
		 0		  1 	  2 	  3 	   4	   5		6	   7	   8	   9	   10	   11
		 9   	  12 	  16	  23	   24	   53	   57	   64	   71	   79	   87	   97

		


       
 条件j>0  和 temp < data[j]   是比较的判断条件 。    
 temp 记录当前gap索引的值。  然后与gap前一个索引比较， 后一个比前一个小（temp < data[j] 前面一个填充后面一个值）;
 然后再用temp去比较前面的值，当边界小于0（j<0）,将记录的值插入到边界加gap索引的位置;
 当temp的值大于前一个值，那么就跳出循环，在后索引位置插入temp值。 

*/





/*
gdb调试方法
打断点到 b shell_sort
打断点到 b  20
打印变量的值    p   temp  
打印变量的直p         i
打印data  数组的长度p       /d  *data@12
打印多个变量  p {j,i,length} 


b  23     b  26 
info b    查看断点 
d  3      删除第3个断点 




II gdb 调试  
打断点到 b 21 
p  {i,j,gap,temp,data[j],j-gap}
p *data@12



III  看
打印局部信息            info locals
必须理解， 一步步调试来进行理解。 
n    运行后，终端显示出来的c代码，是将需要执行的语句。 
n    在主函数将要调用函数前，可以按info          		locals 可以查看局部变量，
     没有执行的局部变量也会加载，比如temp 
s    当看到有跟踪的函数在屏幕出现时，按s跟进函数 
     
watch gap==3    定位gap等于3 ，让程序断下来。 


准备推演一把。 
把我想法推演出来， ok    



*/



void merge(int *data, int *temp, int start, int middle, int end) {

	int i = start, j = middle+1, k = start;

	while (i <= middle && j <= end) {
		if (data[i] > data[j]) {
			temp[k++] = data[j++];
		} else {
			temp[k++] = data[i++];
		}
	}

	while (i <= middle) {
		temp[k++] = data[i++];
	}

	while (j <= end) {
		temp[k++] = data[j++];
	}

	for (i = start;i <= end;i ++) {
		data[i] = temp[i];
	}
}


int merge_sort(int *data, int *temp, int start, int end) {

	int middle;
	
	if (start < end) {
		middle = start + (end - start) / 2;

		merge_sort(data, temp, start, middle);
		merge_sort(data, temp, middle+1, end);

		merge(data, temp, start, middle, end);
	}

} 




/*归并
					   用笔演算
									   
									   0		1		2		3		 4		 5		  6 	 7		 8		 9		 10 	 11
									   23		64		24		12		 9		16		 53 	 57 	 71 	 79 	 87 	 97


第一次进入merge_sort 					   

					   用笔演算
					   
					   0		1		2		3		 4		 5	 	  6	   	 7	   	 8		 9		 10		 11
					   23		64		24		12		 9	 	16	  	 53	  	 57		 71		 79		 87		 97
 
					   			                                i									             
					   			                                                          j
    
                       start   									middle											 end


          merge        i	----------------------------------->|	     j-------------------------------------->|
                       k

                      前索引i数据 和  后索引j数据  ，tmp中存取索引位置小的数据
                      
          temp         23      53      57      64      24    12   9   16         
                             拷贝数据到数组后面                                               71  79 87   97



第二次进入merge_sort 


					   用笔演算
					   
					   0		1		2		3		 4		 5	 	  6	   	 7	   	 8		 9		 10		 11
					   23		64		24		12		 9	 	16	  	 53	  	 57		 71		 79		 87		 97
 
					   			                                i									             
					   			                                                          j
 
                      start   			middle  				end		start			  middle				 end


          merge        i	----------------------------------->|	     j-------------------------------------->|
                       k

                      前索引i数据 和  后索引j数据  ，tmp中存取索引位置小的数据
                      
          temp         23      53      57      64      24    12   9   16         
                             拷贝数据到数组后面                                               71  79 87   97








不对    

      应当是先压栈，先压栈然后再出栈     先压左边， 然后压右边  然后和并起来的。 


      
第一次进入merge_sort 					   

					   用笔演算
					   
					   0		1		2		3		 4		 5	 	  6	   	 7	   	 8		 9		 10		 11
					   23		64		24		12		 9	 	16	  	 53	  	 57		 71		 79		 87		 97
 
					   			                                i									             
					   			                                                          
    
                       start 0  							  middle 5		
第一次进入merge_sort        i	----------------------------------->|	         
 压栈1


					 start 0		   middle  2
第二次进入merge_sort 	   i  ------------>| 	
压栈2

                      
				    start0    middle 1
第三次进入merge_sort  	  i	------>|	  
压栈3


					 start 0    middle 0
第四次进入merge_sort 0	      不成立出栈  	
压栈4   出4


退回
				     start 0  middle 1
第三次进入merge_sort  	  i	------>|
                                  2  2
                                  j->|    调用 右merge_sort   那么又要调用左merge_sort  到不成立   
                                           然后调用merge（，，0，1，2）
                                   0		1		2		3		 4		 5	 	  6	   	 7	   	 8		 9		 10		 11
				            	   23	   24		64		12		 9	 	16	  	 53	  	 57		 71		 79		 87		 97
				            	   
 
出栈3


				   start 0		   middle 2
第二次进入merge_sort 	 i	------------>| 
                                       
										3                    5
                                        j------------------->|        调用 右merge_sort   那么又要调用左merge_sort  到不成立 
                                                                              merge_sortL(,,3,4)                                merge_sortR(,,5,5)  
                                                                        进3  出 3
                                                                  merge_sortL(,,3,3)   merge_sortR (,,4,4)
                                                                  进4 出4
                                                                             0		1		2		3		 4		 5	 	  6	   	 7	   	 8		 9		 10		 11
				            	                                             23	   24		64		9		 12	 	16	  	 53	  	 57		 71		 79		 87		 97
																			 9	   12       16      23       24     64
																			 出2
                      0											5
					 start									  middle	  
第一次进入merge_sort 	   i  ----------------------------------->| 									   

   																6                                               11
																j---------------------------------------------->|

																merge_sortL(,,6,8)           merge_sortR(,,9,11)   
                                                                    
					                                      merge_sortL(,,6,7)    merge_sortR(,,8,8)  
					                                      
                                                        merge_sortL(,,6,6) merge_sortR(,,7,7)   

                                                     前面   6 7 索引比较    ，  6 7 8 比较                     后面           9 10 比较    9  10 11 比较
                                                       然后前后合并
                                                                       53  57 71                  79 87 97
					  0 	1		2		3		 4		 5		  6 	 7		 8		 9		 10 	 11
					  9	   12		16		23		 24 	64		 53 	 57 	 71 	 79 	 87 	 97
					 
                      9    12       16      23       24     53       57      64      71 	 79 	 87 	 97


                      
                         
                      前索引i数据 和  后索引j数据  ，tmp中存取索引位置小的数据
                      
          temp         23      53      57      64      24    12   9   16         
                             拷贝数据到数组后面                                               71  79 87   97
                        



将数组分成两边， 先压左边，左边压完返回后， 在压右边， 右边返回后， 再两边和并出栈。 
这个方法有类似二叉树。 


栈到底有多深：  gdb      b  if middle=0       但是middle是个临时变量入栈函数是重新分配middle变量的。 

*/







void sort(int *data, int left, int right) {

	if (left >= right) return ;

	int i = left;
	int j = right;
	int key = data[left];

	while (i < j) {

		while (i < j && key <= data[j]) {
			j --;
		}
		data[i] = data[j];

		while (i < j && key >= data[i]) {
			i ++;
		}
		data[j] = data[i];
	}

	data[i] = key;

	sort(data, left, i - 1);
	sort(data, i + 1, right);

}




/*

  key = data[left];  23 

							0		 1		 2		 3		  4 	  5 	   6	  7 	  8 	  9 	  10	  11
							23		 64 	 24 	 12 	  9 	 16 	  53	  57	  71	  79	  87	  97

第一次 sort（，0，11）             i 										 j <-------------------------------------------------
    
	                        0		 1		 2		 3		  4 	  5 	   6	  7 	  8 	  9 	  10	  11
							16		 9  	 12 	 23 	  24 	 64 	  53	  57	  71	  79	  87	  97

                                                      j <--------------------
																		
							------------------------>i

key = 16                             
第二次入栈 sortL（0，2） 【        sortR（4，11）】   
							 0		1		2		3		 4		 5		  6 	 7		 8		 9		 10 	 11
						    12		9		16		23		 24 	64		 53 	 57 	 71 	 79 	 87 	 97
 
										    j <--------------------
																	   
				     -                ----->i




key = 12                             
第三次入栈sortL（0，2） 
							 0		 1		 2		 3		  4 	  5 	   6	  7 	  8 	  9 	  10	  11
							 9		 12  	 16 	 23 	  24 	 64 	  53	  57	  71	  79	  87	  97

                                     j <---------
																		
		 			              -->i		
		   sortL（0，0） 【     sortR（2，2）】			              


key = 9                             
第四次入栈sortL（0，0）
							
          第三次出栈
 
																					
key = 24                             
第二次入栈【sortL（0，2）】           sortR（4，11）
							 0		 1		 2		 3		  4 	  5 	   6	  7 	  8 	  9 	  10	  11
							 9		 12  	 16 	 23 	  24 	 64 	  53	  57	  71	  79	  87	  97

                                                              j <-----
																		
					              			                -->i	
					              			               【sortL（4，3）】           sortR（5，11）  
					        0		 1		 2		 3		  4 	  5 	   6	  7 	  8 	  9 	  10	  11
							 9		 12  	 16 	 23 	  24 	 64 	  53	  57	  71	  79	  87	  97

                                                                        j <-----
															 			
					              			                           -->i	
                                                                 
							                                            【sortL（5，4）】           sortR（6，11）
							                                            

	归并和快排都像二叉树排序。 				              
							
*/
int quick_sort(int *data, int length) {

	sort(data, 0, length-1);

}

int main() {

	int data[DATA_ARRAY_LENGTH] = {23, 64, 24, 12, 9, 16, 53, 57, 71, 79, 87, 97};
	int i = 0;

	
    printf("shell_sort:");
	shell_sort(data, DATA_ARRAY_LENGTH);
	for (i = 0;i < DATA_ARRAY_LENGTH;i ++) {
		printf("%4d", data[i]);
	}
	printf("\n");


    printf("merge_sort:");
	int temp[DATA_ARRAY_LENGTH] = {0};

	merge_sort(data, temp, 0, DATA_ARRAY_LENGTH-1);
	for (i = 0;i < DATA_ARRAY_LENGTH;i ++) {
		printf("%4d", data[i]);
	}
	printf("\n");


    printf("quick_sort:");
	quick_sort(data, DATA_ARRAY_LENGTH);
	for (i = 0;i < DATA_ARRAY_LENGTH;i ++) {
		printf("%4d", data[i]);
	}
	printf("\n");


	

}


