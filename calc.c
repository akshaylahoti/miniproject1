#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
const int INT_MIN = -1; /*error condition*/
struct node{
        int data;
        struct node *next;
};
double fact(int n){
	double fact = 1;
	int i;
	for(i = 1; i <= n; i++)
		fact = fact * i;
	return fact;
} 	
double powerfunc(double i, int j) {
	double final = 1;
	int sign = (j > 0 ? 1 : -1);
	if(sign == -1)
		j = -j;
	while(j > 0) {
		if(j % 3 == 2)
			final = final * i * i;
		if(j % 3 == 1)
			final = final * i;
		i = i * i * i;
		j = j / 3;
	}
	if (sign == -1)
		return (1.0 / final);
	else
		return final;
}
struct node *createstack(){ /*function to create stack*/
        return NULL;
}
void push(struct node **head, int value){ /*function to add node at top of stack*/
	struct node *tmp; /*new node*/
	tmp = (struct node*)malloc(sizeof(struct node));
	if(tmp == NULL){
		printf("error\n");
	}
	else{
	tmp->data = value;
	tmp->next = *head;
	*head = tmp;
	}
}
int emptystack(struct node *head){
	return(head == NULL);
}
int pop(struct node **head){
	if(emptystack(*head)){
		printf("error : stack underflown\n");
		return INT_MIN;
	}
	else{
	struct node *top = *head;
	int value = top->data;
	*head = top->next;
	free(top);
	return value;
	}
}
int top(struct node *head){ /*function to return data of top of stack*/
	if(emptystack(head))
		return INT_MIN;
	return head->data;
}
void deletestack(struct node **head){ /*function to delete stack*/
	if(emptystack(*head)){
		return;
	}
	else{
		struct node *temp, *p;
		p = *head;
		while(p->next){
			temp = p->next;
			p->next = temp->next;
			free(temp);
		}
		free(p);
	}
}
void printstack(struct node *node){/*function to print stack from top to bottom*/
	printf("\nlist is");
	while(node != NULL){
		printf("%d", node->data);
		node = node->next;
	}
}
int hasprecedence(int op1, int op2){ /*checking operator precedence with one another*/
	if(op2 == '(' || op2 == ')')
		return 0;
	if((op1 == '*' || op1 == '/')&&(op2 == '+' || op2 == '-'))
		return 0;
	else 
		return 1;
}
int applyop(int op, int b, int a){
	switch(op){
		case'+':
			return a + b;
		case'-':
			return a - b;
		case'*':
			return a * b;
		case'/':
			return a / b;
	}
	return INT_MIN;
}
int evaluateinfix(char *exp){
	struct node *values = createstack();/*stack for operands*/
	struct node *ops = createstack();/*stack for operator*/
	int i = 0, tmp, tmp1;
	int a, b, op;
	while(exp[i] != 0){
		if(exp[i] == ' '){
			i++;
			continue;
		}
		if(exp[i] >= '0' && exp[i] <= '9'){
			push(&values, exp[i]-48);
			while(exp[i+1] >= '0' && exp[i+1] <= '9'){
				i++;
				tmp1 = exp[i]-48;
				tmp = pop(&values);
				tmp = tmp*10 + tmp1;
				push(&values, tmp);
			}
		}
		else if(exp[i] == '(')
			push(&ops, exp[i]);
		else if(exp[i] == ')'){ 
			while(top(ops) != '('){
				a = pop(&values);
				b = pop(&values);
				op = pop(&ops);
				if(op == INT_MIN || a == INT_MIN || b == INT_MIN){
					deletestack(&values);
					deletestack(&ops);
					return INT_MIN;
				}
				tmp = applyop(op, a, b);
				if(tmp == INT_MIN){
					deletestack(&values);
					deletestack(&ops);
					return INT_MIN;
				}
				push(&values, tmp);
			}
			pop(&ops);
		}
		else if(exp[i] == '+' || exp[i] == '-' || exp[i] == '*' || exp[i] == '/'){
			while(!emptystack(ops) && hasprecedence(exp[i], top(ops))){
				a = pop(&values);
				b = pop(&values);
				op = pop(&ops);
				if(op == INT_MIN || a == INT_MIN || b == INT_MIN){
					deletestack(&values);
					deletestack(&ops);
					return INT_MIN;
				}
				tmp = applyop(op, a, b);
				if(tmp == INT_MIN){
					deletestack(&values);
					deletestack(&ops);
					return INT_MIN;
				}
				push(&values, tmp);
			}
 			push(&ops, exp[i]);
		}
		else{
			deletestack(&values);
			deletestack(&ops);
			return INT_MIN;
		}
		i++;
	}
/*all expression parsed , now will apply ops to remaining values*/
	while(!emptystack(ops)){
		a = pop(&values);
		b = pop(&values);
		op = pop(&ops);
		if(op == INT_MIN || a == INT_MIN || b == INT_MIN){
			deletestack(&values);
			deletestack(&ops);
			return INT_MIN;
		}
		tmp = applyop(op, a, b);
		if(tmp == INT_MIN){
			deletestack(&values);
			deletestack(&ops);
			return INT_MIN;
		}
		push(&values, tmp);
	}
	int output = pop(&values);
	if(!emptystack(values)){
		deletestack(&values);
		deletestack(&ops);
		return INT_MIN;
	}
	deletestack(&values);
	deletestack(&ops);
	return output;
}
int main(){
	char *input;
	input = (char *)malloc(sizeof(char));
	printf("Enter the expression to be evaluated\n");
	scanf("%[^\n]s", input);
	int output = evaluateinfix(input);
	if(output != INT_MIN){
		printf("\ninfix:%s result:%d\n", input, output);
	}
	else{
	printf("wrong input\n");
	}
	return 0;
}
