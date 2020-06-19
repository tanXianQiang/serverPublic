#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
  红黑树定义
    1. 每个结点是红的或者黑的
	2. 根结点是黑的
	3. 每个叶子结点是黑的
	4. 如果一个结点是红的，则它的两个儿子都是黑的
	5. 对每个结点，从该结点到其子孙结点的所有路径上的
	    包含相同数目的黑结点





   红黑树的应用
		1. Linux进程调度 CFS
		2. Nginx Timer事件管理
		3. Epoll事件块的管理




案例一、服务器端高并发IO的keep alilve方案，满足一下几个需求
		1. 每个IO都是自己的时间戳
		2. 每个IO收到自己的beat后，重置自己的定时器
		3. 若IO定时没有收到beat，则执行IO的回调函数，并重置定时器
		4. 若再次没有收到beat，销毁IO，注销定时器	


案例二、设计一个线程或者进程的运行体R与运行体调度器S的结构体
		1. 运行体R：包含运行状态{新建，准备，挂起{IO等待读，IO等待写，
		睡眠，延时}， 退出}，运行体回调函数，回调参数
		2. 调度器S：包含栈指针，栈大小，当前运行体
		3. 调度器S：包含执行集合{就绪，延时，睡眠，等待}。

		

*/




#define RED				1
#define BLACK 			2






typedef int KEY_TYPE;

typedef struct _rbtree_node {
	unsigned char color;
	struct _rbtree_node *right;
	struct _rbtree_node *left;
	struct _rbtree_node *parent;
	KEY_TYPE key;
	void *value;
} rbtree_node;

typedef struct _rbtree {
	rbtree_node *root;
	rbtree_node *nil;
} rbtree;

rbtree_node *rbtree_mini(rbtree *T, rbtree_node *x) {
	while (x->left != T->nil) {
		x = x->left;
	}
	return x;
}

rbtree_node *rbtree_maxi(rbtree *T, rbtree_node *x) {
	while (x->right != T->nil) {
		x = x->right;
	}
	return x;
}

rbtree_node *rbtree_successor(rbtree *T, rbtree_node *x) {
	rbtree_node *y = x->parent;

	if (x->right != T->nil) {
		return rbtree_mini(T, x->right);
	}

	while ((y != T->nil) && (x == y->right)) {
		x = y;
		y = y->parent;
	}
	return y;
}


void rbtree_left_rotate(rbtree *T, rbtree_node *x) {

	rbtree_node *y = x->right;  // x  --> y  ,  y --> x,   right --> left,  left --> right

	x->right = y->left; //1 1
	if (y->left != T->nil) { //1 2
		y->left->parent = x;
	}

	y->parent = x->parent; //1 3
	if (x->parent == T->nil) { //1 4
		T->root = y;
	} else if (x == x->parent->left) {
		x->parent->left = y;
	} else {
		x->parent->right = y;
	}

	y->left = x; //1 5
	x->parent = y; //1 6
}
/*



--------------------------------rbtree_left_rotate-  带节点的左旋 ----- 
[O {R,R 0,L 0,P 0,0,NULL}z ]
24,25,13,35,23, 26,67,47,|38,98, 20,19,17,49,12, 21,9,18,14,15
											  [O {B,RNULL,LNULL,PNULL,0,NULL} T->nil]
											  [O {R,R 26,L 13,P 0,24,NULL} T->root]
			[O {B,R 0,L 23,P 24,13,NULL} ]								              [O {B,R 47,L 25,P 24,26,NULL}   ] 
						[O {R,R 0,L 0,P 13,23,NULL} ]	         	[O {B,R 0,L 0,P 26,25,NULL}  ]	              [O {R,R 67,L 35,P 26,47,NULL} z]  
															        						              [O {B,R 38,L 0,P 47,35,NULL} x]       [O {B,R 0,L 0,P 47,67,NULL}  ] 
																														[O {R,R 0,L 0,P 35,38,NULL}]



--------------------------------------------------------------------------------------

[O {R,R 0,L 0,P 0,0,NULL}z ]
24,25,13,35,23, 26,67,|47,38,98, 20,19,17,49,12, 21,9,18,14,15
								  [O {B,RNULL,LNULL,PNULL,0,NULL} T->nil]
								  [O {B,R 26,L 13,P 0,24,NULL} T->root]
			[O {B,R 0,L 23,P 24,13,NULL} ]								[O {R,R 47,L 25,P 24,26,NULL}   ] 
						[O {R,R 0,L 0,P 13,23,NULL} ]		[O {B,R 0,L 0,P 26,25,NULL}  ]		 
																					                     [O {B,R 67,L 35,P 26,47,NULL} y]  
																					              [O {R,R 0,L 0,P 47,35,NULL} x]             [O {R,R 0,L 0,P 47,67,NULL}  ] 









-----------------------------------------------------------------------

 [O {R,R 0,L 0,P 0,0,NULL}z ]
 24,25,13,35,23, |26,67,47,38,98, 20,19,17,49,12, 21,9,18,14,15
								   [O {B,RNULL,LNULL,PNULL,0,NULL} T->nil]
								   [O {B,R 26,L 13,P 0,24,NULL} T->root]
			 [O {B,R 0,L 23,P 24,13,NULL} ] 				             [O {B,R 35,L 25,P 24,26,NULL} y ] 
						 [O {R,R 0,L 0,P 13,23,NULL} ]		 [O {R,R 0,L 0,P 26,25,NULL} x]				 [O {R,R 0,L 0,P 26,35,NULL} ]	 
																						

 ------------------------------------------------------------------------------------------------
[O {R,R 0,L 0,P 0,0,NULL}z ]
24,25,13,35,23, |26,67,47,38,98, 20,19,17,49,12, 21,9,18,14,15
							      [O {B,RNULL,LNULL,PNULL,0,NULL} T->nil]
							      [O {B,R 26,L 13,P 0,24,NULL} T->root]
			[O {B,R 0,L 23,P 24,13,NULL} ]			      	 [O {R,R 0,L 0,P 26,25,NULL} x]
						[O {R,R 0,L 0,P 13,23,NULL} ]						[O {B,R 35,L 25,P 24,26,NULL} y ]	 	
																                           [O {R,R 0,L 0,P 26,35,NULL} ]

*/

void rbtree_right_rotate(rbtree *T, rbtree_node *y) {

	rbtree_node *x = y->left;

	y->left = x->right;
	if (x->right != T->nil) {
		x->right->parent = y;
	}

	x->parent = y->parent;
	if (y->parent == T->nil) {
		T->root = x;
	} else if (y == y->parent->right) {
		y->parent->right = x;
	} else {
		y->parent->left = x;
	}

	x->right = y;
	y->parent = x;
}
/*

[O {R,R 0,L 0,P 0,0,NULL}z ]
24,25,13,35,23, 26,67,|47,38,98, 20,19,17,49,12, 21,9,18,14,15
								  [O {B,RNULL,LNULL,PNULL,0,NULL} T->nil]
								  [O {B,R 26,L 13,P 0,24,NULL} T->root]
			[O {B,R 0,L 23,P 24,13,NULL} ]								[O {R,R 47,L 25,P 24,26,NULL}   ] 
						[O {R,R 0,L 0,P 13,23,NULL} ]		[O {B,R 0,L 0,P 26,25,NULL}  ]		 
																					                     [O {B,R 67,L 35,P 26,47,NULL} y]  
																					              [O {R,R 0,L 0,P 47,35,NULL} x]             [O {R,R 0,L 0,P 47,67,NULL}  ] 

------------------------------------------------------------------------------------------

[O {R,R 0,L 0,P 0,0,NULL}z ]
24,25,13,35,23, 26,67,|47,38,98, 20,19,17,49,12, 21,9,18,14,15
								  [O {B,RNULL,LNULL,PNULL,0,NULL} T->nil]
								  [O {B,R 26,L 13,P 0,24,NULL} T->root]
			[O {B,R 0,L 23,P 24,13,NULL} ]								[O {R,R 35,L 25,P 24,26,NULL}   ] 
						[O {R,R 0,L 0,P 13,23,NULL} ]		[O {B,R 0,L 0,P 26,25,NULL} y ]				   [O {B,R 47,L 0,P 26,35,NULL} ]
																					                                      [O {R,R 67,L 0,P 26,47,NULL} x]  
																					                                                 [O {R,R 0,L 0,P 47,67,NULL} y ] 
																					                     
                                                                                                   //理解这里的左右旋
																					                     

---------------------------------------------------------------------------------------------


[O {R,R 0,L 0,P 0,0,NULL}z ]
24,25,13,35,23, |26,67,47,38,98, 20,19,17,49,12, 21,9,18,14,15
							      [O {B,RNULL,LNULL,PNULL,0,NULL} T->nil,y]
							      [O {B,R 25,L 13,P 0,24,NULL} T->root]
			[O {B,R 0,L 23,P 24,13,NULL} ]			      	 [O {B,R 26,L 0,P 24,25,NULL} ]
						[O {R,R 0,L 0,P 13,23,NULL} ]						[O {R,R 35,L 0,P 25,26,NULL}x]	 	
																                           [O {R,R 0,L 0,P 26,35,NULL} y]

------------------------------------------------------------------------------------------------------
[O {R,R 0,L 0,P 0,0,NULL}z ]
24,25,13,35,23, |26,67,47,38,98, 20,19,17,49,12, 21,9,18,14,15
							      [O {B,RNULL,LNULL,PNULL,0,NULL} T->nil,y]
							      [O {B,R 25,L 13,P 0,24,NULL} T->root]
			[O {B,R 0,L 23,P 24,13,NULL} ]			      	 [O {B,R 26,L 0,P 24,25,NULL} ]
						[O {R,R 0,L 0,P 13,23,NULL} ]							 [O {R,R 0,L 26,P 26,35,NULL} y]	
																[O {R,R 35,L 0,P 25,26,NULL}x]

--------------------------------------------------------------------------

[O {R,R 0,L 0,P 0,0,NULL}z ]
24,25,13,35,23, |26,67,47,38,98, 20,19,17,49,12, 21,9,18,14,15
							      [O {B,RNULL,LNULL,PNULL,0,NULL} T->nil,y]
							      [O {B,R 25,L 13,P 0,24,NULL} T->root]
			[O {B,R 0,L 23,P 24,13,NULL} ]			      	 [O {B,R 35,L 0,P 24,25,NULL} ]
						[O {R,R 0,L 0,P 13,23,NULL} ]							 [O {R,R 0,L 0,P 25,35,NULL} y	
																[O {R,R 0,L 0,P 35,26,NULL}x]








*/









void rbtree_insert_fixup(rbtree *T, rbtree_node *z) {

	while (z->parent->color == RED) { //z ---> RED
		if (z->parent == z->parent->parent->left) {  /* 确定父节点是父父节点的左还是右 */
			rbtree_node *y = z->parent->parent->right;
			if (y->color == RED) {
				z->parent->color = BLACK;
				y->color = BLACK;
				z->parent->parent->color = RED;

				z = z->parent->parent; //z --> RED    /*父父节点变为红色， 还要进行判断他的父父节点的父节点是否为红色*/
			} else {

				if (z == z->parent->right) {
					z = z->parent;
					rbtree_left_rotate(T, z);
				}

				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				rbtree_right_rotate(T, z->parent->parent);
			}
		}else {
			rbtree_node *y = z->parent->parent->left;
			if (y->color == RED) {
				z->parent->color = BLACK;
				y->color = BLACK;
				z->parent->parent->color = RED;

				z = z->parent->parent; //z --> RED
			/*
				  改变Z的值去遍历上层。 
				  
			*/
			} else {
				if (z == z->parent->left) {
					z = z->parent;
					rbtree_right_rotate(T, z);
				}

				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				rbtree_left_rotate(T, z->parent->parent);
			}
		}
		
	}

	T->root->color = BLACK;
}





 /*   
                  
              



很多的项目，但是你们这个水平， 我敢介绍给你们搞没得嘛，我感觉这是忽悠我的吧
右转就是子节点和父节点位置交换， 子节点做父节点，是个小的数，父节点做子节点是个大数，父节点是子节点的右孩。  



理解：   rbtree_right_rotate(T, z->parent->parent);
     右旋.   将父节点放到右边去。 
     

*/

/*
  理解：  
          p判断该父节点是否为红色。 
          判断父节点是在父父节点的左边还是右边。  左边执行if    右边执行else     ；  牵连三层 。    
                   左边  ：    判断父节点的兄弟节点是红色还是黑色。   红色执行if	   黑色执行else 
                             红色  ： 把父节点和父节点兄弟节点变为黑色， 父父节点变为红色。  然后父父节点作为子节点，检查是否满足要求。 
                             黑色：  新节点在右边还是在左边还是右边   
                             				右边： 左转   右转
                             				左边： 右转
                                      
                   右边  ：    判断父节点的兄弟节点是红色还是黑色。   红色执行if	   黑色执行else 
                             红色  ： 把父节点和父节点兄弟节点变为黑色， 父父节点变为红色。  然后父父节点作为子节点，检查是否满足要求。 
                             黑色：  新节点在右边还是在左边还是右边   
                             				左边： 右转
                             				右边： 右转左转
                                      


*/

/*

--------------------------------rbtree_left_rotate-  带节点的左旋 ----- 
[O {R,R 0,L 0,P 0,0,NULL}z ]
24,25,13,35,23, 26,67,47,38,98,|20, 19,17,49,12, 21,9,18,14,15
													  [O {B,RNULL,LNULL,PNULL,0,NULL} T->nil]
													  [O {R,R 26,L 13,P 0,24,NULL} T->root]
					[O {B,R 23,L 0,P 24,13,NULL} ]								              [O {B,R 47,L 25,P 24,26,NULL}   ] 
		                        [O {R,R 0,L 20,P 13,23,NULL} ]	         	[O {B,R 0,L 0,P 26,25,NULL}  ]	              [O {R,R 67,L 35,P 26,47,NULL} z]  
														        					 						    [O {B,R 38,L 0,P 47,35,NULL} x]        [O {B,R 98,L 0,P 47,67,NULL}  ] 
						   													                              				[O {R,R 0,L 0,P 35,38,NULL} z]                   [O {R,R 0,L 0,P 67,98,NULL} z] 




[O {R,R 0,L 0,P 13,20,NULL} ]


----------------------------------------------------------------
[O {R,R 0,L 0,P 0,0,NULL}z ]
24,25,13,35,23, 26,67,|47,38,98, 20,19,17,49,12, 21,9,18,14,15
								  [O {B,RNULL,LNULL,PNULL,0,NULL} T->nil]
								  [O {B,R 26,L 13,P 0,24,NULL} T->root]
			[O {B,R 0,L 23,P 24,13,NULL} ]								[O {R,R 47,L 25,P 24,26,NULL}   ] 
						[O {R,R 0,L 0,P 13,23,NULL} ]		[O {B,R 0,L 0,P 26,25,NULL}  ]		   [O {B,R 67,L 35,P 26,47,NULL} y]  
																					              [O {R,R 0,L 0,P 47,35,NULL} x]             [O {R,R 0,L 0,P 47,67,NULL}  ] 







------------------------------------------------------------------
[O {R,R 0,L 0,P 0,0,NULL}z ]
24,25,13,35,23, 26,67,|47,38,98, 20,19,17,49,12, 21,9,18,14,15
								  [O {B,RNULL,LNULL,PNULL,0,NULL} T->nil]
								  [O {B,R 26,L 13,P 0,24,NULL} T->root]
			[O {B,R 0,L 23,P 24,13,NULL} ]								[O {R,R 35,L 25,P 24,26,NULL}   ] 
						[O {R,R 0,L 0,P 13,23,NULL} ]		[O {B,R 0,L 0,P 26,25,NULL} y ]				   [O {R,R 47,L 0,P 26,35,NULL} ]
																					                                      [O {B,R 67,L 0,P 26,47,NULL} ]  
																					                                                 [O {R,R 0,L 0,P 47,67,NULL} z ] 




--------------------------------------------------------------------------------------------
[O {R,R 0,L 0,P 0,0,NULL}z ]
24,25,13,35,23, 26,67,|47,38,98, 20,19,17,49,12, 21,9,18,14,15
								  [O {B,RNULL,LNULL,PNULL,0,NULL} T->nil]
								  [O {B,R 26,L 13,P 0,24,NULL} T->root]
			[O {B,R 0,L 23,P 24,13,NULL} ]								[O {R,R 35,L 25,P 24,26,NULL}   ] 
						[O {R,R 0,L 0,P 13,23,NULL} ]		[O {B,R 0,L 0,P 26,25,NULL} y ]				   [O {B,R 67,L 0,P 26,35,NULL} ]
																					                                       [O {R,R 0,L 47,P 35,67,NULL} z] 
																					                     [O {R,R 0,L 0,P 67,47,NULL} ]  
                                                                                                   //理解这里的左右旋
																					                     

--------------------------------------------

[O {R,R 0,L 0,P 0,0,NULL}z ]
24,25,13,35,23, |26,67,47,38,98, 20,19,17,49,12, 21,9,18,14,15
							      [O {B,RNULL,LNULL,PNULL,0,NULL} T->nil,y]
							      [O {B,R 25,L 13,P 0,24,NULL} T->root]
			[O {B,R 0,L 23,P 24,13,NULL} ]			      	 [O {R,R 26,L 0,P 24,25,NULL} ]
						[O {R,R 0,L 0,P 13,23,NULL} ]						[O {B,R 35,L 0,P 25,26,NULL}]	 	
																                           [O {R,R 0,L 0,P 26,35,NULL} ]

--------------------------------------------------------------------------
[O {R,R 0,L 0,P 0,0,NULL}z ]
	 24,25,13,35,23, |26,67,47,38,98, 20,19,17,49,12, 21,9,18,14,15
									      [O {B,RNULL,LNULL,PNULL,0,NULL} T->nil,y]
									      [O {B,R 25,L 13,P 0,24,NULL} T->root]
					[O {B,R 0,L 23,P 24,13,NULL} ]			      	 [O {B,R 35,L 0,P 24,25,NULL} ]
								[O {R,R 0,L 0,P 13,23,NULL} ]							 [O {R,R 0,L 26,P 25,35,NULL} z]	
																		[O {R,R 0,L 0,P 35,26,NULL}]

---------------------------------------------------------------------------
[O {R,R 0,L 0,P 0,0,NULL}z ]
	 24,25,13,35,23, |26,67,47,38,98, 20,19,17,49,12, 21,9,18,14,15
									      [O {B,RNULL,LNULL,PNULL,0,NULL} T->nil]
									      [O {B,R 25,L 13,P 0,24,NULL} T->root]
					[O {B,R 0,L 23,P 24,13,NULL} ]			      	 [O {B,R 35,L 0,P 24,25,NULL} ]
								[O {R,R 0,L 0,P 13,23,NULL} ]							 [O {R,R 0,L 0,P 25,35,NULL} ]	

-----------------------------------------------------------------------------
[O {R,R 0,L 0,P 0,0,NULL}z ]
	 24,25,13,|35,23, 26,67,47,38,98, 20,19,17,49,12, 21,9,18,14,15
				      [O {B,RNULL,LNULL,PNULL,0,NULL} T->nil]
				      [O {B,R 25,L 13,P 0,24,NULL} T->root]
[O {B,R 0,L 0,P 24,13,NULL} ]			      	 [O {B,R 35,L 0,P 24,25,NULL} ]
																 [O {R,R 0,L 0,P 25,35,NULL} ]	

--------------------------------------------------------------------------

[O {R,R 0,L 0,P 0,0,NULL}z ]
	 24,25,13,35,23, 26,67,47,38,98, 20,19,17,49,12, 21,9,18,14,15
	 
					 [O {B,RNULL,LNULL,PNULL,0,NULL} T->nil]

					 
				                                         [O {B,R 25,L 13,P 0,24,NULL} T->root,x,y]

[O {R,R 0,L 0,P 24,13,NULL}z ]//黑树下面是红树可以不用变化																 [O {R,R 0,L 0,P 24,25,NULL}z ]


----------------------------------------------------------------------------
[O {R,R 0,L 0,P 0,0,NULL}z ]
	 24,25,13,35,23, 26,67,47,38,98, 20,19,17,49,12, 21,9,18,14,15
	 
					 [O {B,RNULL,LNULL,PNULL,0,NULL} T->nil]

					 
   [O {B,R 25,L 0,P 0,24,NULL} T->root,x,y]

											 [O {R,R 0,L 0,P 24,25,NULL}z ]
   

-------------------------------------------------------------------------
	  24,25,13,35,23, 26,67,47,38,98, 20,19,17,49,12, 21,9,18,14,15
        
                        [O {B,NULL,NULL,NULL,0,NULL} T->nil,y,x]

                        
      [O {B,0,0,0,24,NULL}z, T->root]
																
*/		





void rbtree_insert(rbtree *T, rbtree_node *z) {

	rbtree_node *y = T->nil;
	rbtree_node *x = T->root;

     /* 找树的节点为nil */
	while (x != T->nil) {
		y = x;       /* 存取x的上一个节点 */
		if (z->key < x->key) {
			x = x->left;
		} else if (z->key > x->key) {
			x = x->right;
		} else { //Exist
			return ;
		}
	} 


	/* 树的nil节点插入节点 */
	z->parent = y;
	if (y == T->nil) {
		T->root = z;
	} else if (z->key < y->key) {
		y->left = z;
	} else {
		y->right = z;
	}

	z->left = T->nil;
	z->right = T->nil;
	z->color = RED;

	rbtree_insert_fixup(T, z);
}



/*


[O {R,R 0,L 0,P 0,0,NULL}z ]
	 24,25,13,35,23, |26,67,47,38,98, 20,19,17,49,12, 21,9,18,14,15
									      [O {B,RNULL,LNULL,PNULL,0,NULL} T->nil]
									      [O {B,R 25,L 13,P 0,24,NULL} T->root]
					[O {B,R 0,L 23,P 24,13,NULL} ]			      	 [O {B,R 35,L 0,P 24,25,NULL} ]
								[O {R,R 0,L 0,P 13,23,NULL} ]							 [O {R,R 0,L 26,P 25,35,NULL} ]	
																		[O {R,R 0,L 0,P 35,26,NULL}z]
																			

-----------------------------------------------------------------------------------
[O {R,R 0,L 0,P 0,0,NULL}z ]
	 24,25,13,35,23, |26,67,47,38,98, 20,19,17,49,12, 21,9,18,14,15
									      [O {B,RNULL,LNULL,PNULL,0,NULL} T->nil]
									      [O {B,R 25,L 13,P 0,24,NULL} T->root]
					[O {B,R 0,L 23,P 24,13,NULL} ]			      	 [O {B,R 35,L 0,P 24,25,NULL} ]
								[O {R,R 0,L 0,P 13,23,NULL} ]							 [O {R,R 0,L 26,P 25,35,NULL} ]	
																		[O {R,R 0,L 0,P 35,26,NULL}z]
																			

----------------------------------------------------------------
[O {R,R 0,L 0,P 0,0,NULL}z ]
	 24,25,13,35,|23, 26,67,47,38,98, 20,19,17,49,12, 21,9,18,14,15
									      [O {B,RNULL,LNULL,PNULL,0,NULL} T->nil]
									      [O {B,R 25,L 13,P 0,24,NULL} T->root]
					[O {B,R 0,L 23,P 24,13,NULL} ]			      	 [O {B,R 35,L 0,P 24,25,NULL} ]
								[O {R,R 0,L 0,P 13,23,NULL}z ]							 [O {R,R 0,L 0,P 25,35,NULL} ]	

------------------------------------------------------------
[O {R,R 0,L 0,P 0,0,NULL}z ]
	 24,25,13,|35,23, 26,67,47,38,98, 20,19,17,49,12, 21,9,18,14,15
	 
				 [O {B,RNULL,LNULL,PNULL,0,NULL} T->nil]

					 
				      [O {B,R 25,L 13,P 0,24,NULL} T->root,x,y]

[O {R,R 0,L 0,P 24,13,NULL} ]			      	 [O {R,R 0,L 0,P 24,25,NULL} ]

																 [O {R,R 0,L 0,P 25,35,NULL}z ]			






----------------------------------------------------------------------------------

[O {R,R 0,L 0,P 0,0,NULL}z ]
	 24,25,13,35,23, 26,67,47,38,98, 20,19,17,49,12, 21,9,18,14,15
	 
					 [O {B,RNULL,LNULL,PNULL,0,NULL} T->nil]

					 
				                         [O {B,R 25,L 13,P 0,24,NULL} T->root,x,y]

[O {R,R 0,L 0,P 24,13,NULL}z ]																 [O {R,R 0,L 0,P 24,25,NULL}z ]

---------------------------------------------------------------------------------
   [O {R,R 0,L 0,P 0,0,NULL}z ]
        24,25,13,35,23, 26,67,47,38,98, 20,19,17,49,12, 21,9,18,14,15
        
                        [O {B,RNULL,LNULL,PNULL,0,NULL} T->nil]

                        
      [O {B,R 25,L 0,P 0,24,NULL} T->root,x,y]

										        [O {R,R 0,L 0,P 24,25,NULL}z ]
      
*/








void rbtree_delete_fixup(rbtree *T, rbtree_node *x) {

	while ((x != T->root) && (x->color == BLACK)) {
		if (x == x->parent->left) {

			rbtree_node *w= x->parent->right;
			if (w->color == RED) {
				w->color = BLACK;
				x->parent->color = RED;

				rbtree_left_rotate(T, x->parent);
				w = x->parent->right;
			}

			if ((w->left->color == BLACK) && (w->right->color == BLACK)) {
				w->color = RED;
				x = x->parent;
			} else {

				if (w->right->color == BLACK) {
					w->left->color = BLACK;
					w->color = RED;
					rbtree_right_rotate(T, w);
					w = x->parent->right;
				}

				w->color = x->parent->color;
				x->parent->color = BLACK;
				w->right->color = BLACK;
				rbtree_left_rotate(T, x->parent);

				x = T->root;
			}

		} else {

			rbtree_node *w = x->parent->left;
			if (w->color == RED) {
				w->color = BLACK;
				x->parent->color = RED;
				rbtree_right_rotate(T, x->parent);
				w = x->parent->left;
			}

			if ((w->left->color == BLACK) && (w->right->color == BLACK)) {
				w->color = RED;
				x = x->parent;
			} else {

				if (w->left->color == BLACK) {
					w->right->color = BLACK;
					w->color = RED;
					rbtree_left_rotate(T, w);
					w = x->parent->left;
				}

				w->color = x->parent->color;
				x->parent->color = BLACK;
				w->left->color = BLACK;
				rbtree_right_rotate(T, x->parent);

				x = T->root;
			}

		}
	}

	x->color = BLACK;
}

rbtree_node *rbtree_delete(rbtree *T, rbtree_node *z) {

	rbtree_node *y = T->nil;
	rbtree_node *x = T->nil;

	if ((z->left == T->nil) || (z->right == T->nil)) {
		y = z;
	} else {
		y = rbtree_successor(T, z);
	}

	if (y->left != T->nil) {
		x = y->left;
	} else if (y->right != T->nil) {
		x = y->right;
	}

	x->parent = y->parent;
	if (y->parent == T->nil) {
		T->root = x;
	} else if (y == y->parent->left) {
		y->parent->left = x;
	} else {
		y->parent->right = x;
	}

	if (y != z) {
		z->key = y->key;
		z->value = y->value;
	}

	if (y->color == BLACK) {
		rbtree_delete_fixup(T, x);
	}

	return y;
}

/*



--------------------------------rbtree_left_rotate-  带节点的左旋 ----- 
[O {R,R 0,L 0,P 0,0,NULL}z ]
24,25,13,35,23, 26,67,47,38,98,|20, 19,17,49,12, 21,9,18,14,15
													  [O {B,RNULL,LNULL,PNULL,0,NULL} T->nil]
													  [O {R,R 26,L 13,P 0,24,NULL} T->root]
					[O {B,R 23,L 0,P 24,13,NULL} ]								              [O {B,R 47,L 25,P 24,26,NULL}   ] 
		                        [O {R,R 0,L 20,P 13,23,NULL} ]	         	[O {B,R 0,L 0,P 26,25,NULL}  ]	              [O {R,R 67,L 35,P 26,47,NULL} z]  
														        					 						    [O {B,R 38,L 0,P 47,35,NULL} x]        [O {B,R 98,L 0,P 47,67,NULL}  ] 
						   													                              				[O {R,R 0,L 0,P 35,38,NULL} z]                   [O {R,R 0,L 0,P 67,98,NULL} z] 




------------------------------------------------------------
    理解： 
    		  判断是否是子节点
    		          是			子节点     把z赋值给y 
    		          不是		
    		  用x节点去替换y节点      				将y从节点中删除  
    		  判断y的颜色   
    		            黑色



*/
rbtree_node *rbtree_search(rbtree *T, KEY_TYPE key) {

	rbtree_node *node = T->root;
	while (node != T->nil) {
		if (key < node->key) {
			node = node->left;
		} else if (key > node->key) {
			node = node->right;
		} else {
			return node;
		}	
	}
	return T->nil;
}


void rbtree_traversal(rbtree *T, rbtree_node *node) {
	if (node != T->nil) {
		rbtree_traversal(T, node->left);
		printf("key:%d, color:%d\n", node->key, node->color);
		rbtree_traversal(T, node->right);
	}
}

int main() {

	int keyArray[] = {24,25,13,35,23, 26,67,47,38,98,}; //  20,19,17,49,12, 21,9,18,14,15

	rbtree *T = (rbtree *)malloc(sizeof(rbtree));
	if (T == NULL) {
		printf("malloc failed\n");
		return -1;
	}
	
	T->nil = (rbtree_node*)malloc(sizeof(rbtree_node));
	T->nil->color = BLACK;
	T->root = T->nil;
    /* 这个节点只给了颜色，没有给键值 */
    /*所有叶子节点都是黑色的，并是隐藏      			可以知道T->nil 指向的是一个空间*/
/*
     O {B,NULL,NULL,NULL,0,NULL} T->nil, T->root
     O {0,NULL,NULL,NULL,24,NULL}
*/
	

	rbtree_node *node = T->nil;
	int i = 0;
	int size  = sizeof(keyArray)/sizeof(int);
	for (i = 0;i <size ;i ++) {
		node = (rbtree_node*)malloc(sizeof(rbtree_node));
		node->key = keyArray[i];
		node->value = NULL;     /* O {0,NULL,NULL,NULL,24,NULL} */

		rbtree_insert(T, node);
		
	}

	rbtree_traversal(T, T->root);
	printf("----------------------------------------\n");

	for (i = 0;i < size;i ++) {

		rbtree_node *node = rbtree_search(T, keyArray[i]);
		rbtree_node *cur = rbtree_delete(T, node);
		free(cur);

		rbtree_traversal(T, T->root);
		printf("----------------------------------------\n");
	}
	

	
}




/*

   理解 ： 
      插入到节点中                   rbtree_insert 
      然后变换位置                   rbtree_insert_fixup 
      是父父节点的左还是右               if (z->parent == z->parent->parent->left) 
      父节点的兄弟节点颜色      		   if (y->color == RED)    变换父节点和父节点的兄弟节点为黑色。  父父节点为红色。 



插入节点      节点旋转有些不了解。 


删除节点       删除节点有些复杂， 需要用节点去模拟，理顺编写流程过程。     




总结： 
      以后程序有理解不了的，用一组数据模拟， 每个分支都跑一遍， 然后理出思路。 
      该程序没有模拟完整，需要重新模拟。  





后续操作：       
          重新根据分支来选择模拟数据， 理出思路。                                         

*/

