#include <stdio.h>
#include <stdlib.h>

/* +---------+----------+   +---------+----------+          +---------+----------+   
 * |   val   |   next --+-->|   val   |   next --+--> … … |   val   |   next --+--> NULL
 * +---------+----------+   +---------+----------+          +---------+----------+   
 */
typedef struct s_node {
	int val;
	struct s_node * next;
} NODE;

void ll_append(NODE* head, int val)
{
	NODE* p = (NODE*)malloc(sizeof(NODE));
	p->val = val;
	p->next = 0;

	NODE* q;
	for(q = head; q->next != 0; q = q->next) {}
	q->next = p;
}

void ll_free(NODE* head)
{
	NODE* p = head;
	NODE* q = head;
	while (q) {
		p = q;
		q = q->next;
		free(p);
	}
}

void ll_print(NODE* head)
{
	NODE* p = head;
	while (p) {
		printf("%d -> ", p->val);
		p = p->next;
	}
	printf("NULL\n");
}

NODE * llr(NODE* src)
{
	static NODE* prev;

	NODE* reversed;
	if (src->next) {
		reversed = llr(src->next);
		prev->next = src;
		src->next = 0;
		prev = src;
	}
	else {
		reversed = prev = src;
	}

	return reversed;
}

int main(void)
{
	NODE* head = (NODE*)malloc(sizeof(NODE));
	head->val = 0;
	head->next = 0;

	ll_append(head, 1);
	ll_append(head, 2);
	ll_append(head, 3);
	ll_append(head, 4);
	ll_append(head, 5);
	ll_append(head, 6);
	ll_append(head, 7);
	ll_append(head, 8);

	ll_print(head);

	head = llr(head);
	ll_print(head);

	ll_free(head);

        return 0;
}
