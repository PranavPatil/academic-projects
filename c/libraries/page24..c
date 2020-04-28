#include<stdio.h>
#include<stdlib.h>
int evaluate(int matches_num,int player,int *take)
{
	int i,value,max_take;
	if(matches_num<=0)
		return player;
	max_take=min(3,matches_num);
	value=-player;
	for(i=1;i<=max_take;++i)
		if(evaluate(matches_num-i,-player,&i)==player)
		{
			value=player;
			if(player==1)
				*take=i;
		}
	return value;
}
int main(void)
{
	int num,take=1,score,p2;
	printf("Player 1:Enter the number of mathches\n");
	scanf("%d",&num);
	printf("The winner is player %d\n",score=evaluate(num,1,&take)==1?1:2);
	printf("Player 1 take %d matches\n",take);
	while((num-=take)>1)
	{
		printf("There are %d matches left\n",num);
		printf("player 2 : how much do you take?\n");
		scanf("%d",&p2);
		score=evaluate(num-=p2,1,&take);
		printf("Player 1 take %d matches\nplayer %d is winning\n ",take,
score == 1?1:2);
	}
	printf("Game Over\n");
	return 0;
}
