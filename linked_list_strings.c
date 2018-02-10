/*
Author: Dhiveshan Chetty
The program doesn't completely fill the list with all the correct values
the member function is in the program but not implemented fully
I have a New Node function that creates a new node
I had a fatal error that I couldn't seem to fix
the head and tail are both stored in a struct
and they point to the next and prev of the list_node_s
the print function seems to induce infinite recursion
or produce a segmentation fault
My code handles all the cases in logic, however it seems to have syntactical errors


Please give me partial credit for somewhat functioning code
I can't afford to drop this class because of 1 project
I've also been working on this project for 35 hours so far and I couldn't seem to figure 
out what was wrong, even with the tutors/professor's help.
Sorry for any inconvenience I may have caused

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct list_node_s
{
	char* data;
	struct list_node_s* next_p;
	struct list_node_s* prev_p;
	
};
struct list_s
{
	struct list_node_s* head_p;
	struct list_node_s* tail_p;
};

struct list_node_s* New_Node(char* val) {
	struct list_node_s* temp;
   char val2[strlen(val)];
   strcpy(val2,val);
   printf("%s\n",val);
   printf("Copied string: %s\n", val2 );
   temp =malloc(1*sizeof(struct list_s));
   temp -> data = val2;
   printf("%s\n",val2 );
   temp -> next_p = NULL;
   temp -> prev_p = NULL;
   return temp;
}  /* Create_node */

void print(struct list_node_s* head_p );

void insert(char *insertion, struct list_s *list)
{
	struct list_node_s* curr_p= list->head_p;
	struct list_node_s* temp;
	int count =0;
	while(curr_p !=NULL)
	{
		printf("current data: %s\n",curr_p->data );
		printf("insertion: %s\n",insertion );
		printf("string compare value: %d\n", strcmp(curr_p->data,insertion));
		if(strcmp(curr_p ->data, insertion)  <0)
		{
			printf("-1\n" );
			curr_p = curr_p->next_p;
			count++;

		}
		else if(strcmp(curr_p -> data,insertion) >0 )
		{

				printf("+1\n" );
			
			break;
		}
		else if(strcmp(curr_p -> data,insertion) ==0)
		{
			return;
		}
	}
	temp = malloc(sizeof(struct list_node_s));
	temp-> data=malloc(sizeof(char)*strlen(insertion));
	strcpy(temp -> data, insertion);
	if(curr_p == NULL)
	{

		list-> tail_p = temp;		
		list->head_p = temp;
		// list->tail_p->prev_p=NULL;
		// list->head_p->next_p=NULL;


		// temp =New_Node(insertion);
		printf("current data: %s\n",temp->data );
		return;
	}
	else
	{
		// if(count==0)
		// {
		// 	curr_p->next_p = list->head_p;
		// 	list->head_p->prev_p= curr_p;
		// 	curr_p-> prev_p = NULL;
		// 	list-> head_p = curr_p;
		// }
		temp->prev_p = curr_p;
        temp->next_p = curr_p->next_p;
        curr_p->next_p->prev_p = temp;
        curr_p->next_p = temp;
		// temp = curr_p;
		// temp-> prev_p = curr_p -> prev_p;
		// curr_p-> prev_p = temp;
		// temp ->next_p =curr_p;
		// temp->prev_p->next_p = temp;
		return;
	}
void member(struct list_s* list, char *insertion)
{
	struct list_node_s* curr2 = list->head_p;
	
	while(strcmp(curr2->data,insertion) != 0 )
	{

		curr2 = curr2->next_p;
		
	}
	if(strcmp(curr2->data,insertion) ==0)
	{
		printf("found value");
	}
	else
	{
		printf("not found");
	}

}


}
void print(struct list_node_s* head_p )
{
	struct list_node_s* temp2 = head_p;
	while(temp2 != NULL)
	{
		printf("%s\n", temp2 ->data );
		temp2= temp2->next_p;
	}
	
}
int main(void)
{

	struct list_s list;
	struct list_node_s* temp1;
	list.head_p = NULL;
	list.tail_p = NULL;
	temp1 = NULL;
	//head_p = NULL;
	//tail_p =NULL;
	char insertion[100];
	char *i ="i";
	char input[1];
	printf("Please type the action you would like to perform(i,m,p,d,f)\n");
	scanf(" %c",&input);
	printf("%d\n", strncmp(input,i,1));
	printf("%c\n",input[0] );
	if(strncmp(input,i,1)==0)
	{
		printf("Please input the string to add to the list\n");
		scanf(" %s", &insertion);
		insert(insertion, &list);
		// if (list.head_p==NULL)
		// {
		// 	printf("yes\n");
		// }
		// else
		// {
		// 	printf("Head data: %s\n", list.head_p -> data);
		// }
		int i;
		for (i = 0; i < 2; ++i)
		{
		printf("Please input the string to add to the list\n");
		scanf(" %s", &insertion);
		insert(insertion, &list);
		
		}
		print(list.head_p);
	}
	return 0;
}