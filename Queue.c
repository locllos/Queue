#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int ERROR = -1;
const int INF = (int)1e9 + 1;

int Min(const int value_a, const int value_b)
{
	return value_a < value_b ? value_a : value_b;
}

typedef struct Node
{
	int value_;
	int min_value_;

	struct Node* next_;
} Node;

typedef struct List
{
	size_t size_;
	int min_value_;

	Node* head_;

} List;

typedef struct Queue
{

	List* add_stack_;
	List* del_stack_;

	size_t size_;


} Queue;

//Prototypes of list functions
List* constructList();
void pushList(List* list, const int value);
int topList(const List* list);
int minList(const List* list);
void popList(List* list);
void destructList(List* list);


//Prototypes of queue functions
Queue* constructQueue();
void balanceQueue(Queue* queue);
void pushQueue(Queue* queue, const int value);
int popQueue(Queue* queue);
int frontQueue(Queue* queue);
int minQueue(const Queue* queue);
int getQueueSize(const Queue* queue);
void clearQueue(Queue* queue);
void destructQueue(Queue* queue);


//List functions
List* constructList()
{
	List* list = (List*)calloc(1, sizeof(List));
	list->size_ = 0;
	list->head_ = NULL;

	return list;
}

void pushList(List* list, const int value)
{
	Node* node = (Node*)calloc(1, sizeof(Node));

	node->value_ = value;
	node->min_value_ = (list->size_ > 0) ? Min(minList(list), value) : value;
	node->next_ = list->head_;

	list->min_value_ = node->min_value_;

	list->head_ = node;
	++list->size_;
}

int topList(const List* list)
{
	assert(list->size_ != 0);

	return list->head_->value_;
}

int minList(const List* list)
{
	assert(list->size_ != 0);

	return list->min_value_;
}

void popList(List* list)
{
	int min_value = INF;

	if (list->size_ == 0)
	{
		return;
	}
	else if (list->size_ > 1)
	{
		min_value = list->head_->next_->min_value_;
	}
	Node* node = list->head_;
	list->head_ = node->next_;
	node->next_ = NULL;

	free(node);
	--list->size_;

	list->min_value_ = min_value;
}

void destructList(List* list)
{
	while (list->size_)
	{
		popList(list);
	}

	free(list);
}

//Queue functions
Queue* constructQueue()
{
	Queue* queue = (Queue*)calloc(1, sizeof(Queue));

	queue->add_stack_ = constructList();
	queue->del_stack_ = constructList();

	queue->add_stack_->size_ = 0;
	queue->del_stack_->size_ = 0;

	queue->size_ = 0;

	return queue;
}

void balanceQueue(Queue* queue)
{
	const int add_stack_size = queue->add_stack_->size_;

	for (int i = 0; i < add_stack_size; ++i)
	{
		pushList(queue->del_stack_, topList(queue->add_stack_));

		popList(queue->add_stack_);
	}
}

void pushQueue(Queue* queue, const int value)
{
	pushList(queue->add_stack_, value);

	++queue->size_;
}

int popQueue(Queue* queue)
{
	if (queue->size_ == 0)
	{
        return ERROR;
	}

	if (queue->del_stack_->size_ == 0)
	{
		balanceQueue(queue);
	}

	const int get_popped = frontQueue(queue);
	popList(queue->del_stack_);
	--queue->size_;

	return get_popped;
}

int frontQueue(Queue* queue)
{
	if (queue->size_ == 0)
	{
		return ERROR;
	}

    if (queue->del_stack_->size_ == 0)
    {
		balanceQueue(queue);
    }

	return queue->del_stack_->head_->value_;
}

int minQueue(const Queue* queue)
{
	if (queue->size_ == 0)
	{
		return ERROR;
	}

	if (queue->del_stack_->size_ != 0 && queue->add_stack_->size_ != 0)
	{
		return Min(minList(queue->del_stack_), minList(queue->add_stack_));
	}

	else if (queue->del_stack_->size_ == 0 && queue->add_stack_->size_ != 0)
	{
		return minList(queue->add_stack_);
	}
	else if (queue->del_stack_->size_ != 0 && queue->add_stack_->size_ == 0)
	{
		return minList(queue->del_stack_);
	}

	return ERROR;
}

int getQueueSize(const Queue* queue)
{
	return queue->size_;
}

void clearQueue(Queue* queue)
{
	destructList(queue->add_stack_);
	destructList(queue->del_stack_);

	queue->add_stack_ = constructList();
	queue->del_stack_ = constructList();

	queue->size_ = 0;
}

void destructQueue(Queue* queue)
{
	destructList(queue->add_stack_);
	destructList(queue->del_stack_);

	free(queue);
}

int main()
{
	Queue* queue = constructQueue();

	char* input = (char*)calloc(10, sizeof(char));

	int n = 0;
	int number = 0;
	scanf("%d", &n);

	for (int i = 0; i < n; ++i)
	{
		scanf("%s", input);

		if (strcmp(input, "enqueue") == 0)
		{

			scanf("%d", &number);

			pushQueue(queue, number);

			printf("ok\n");

		}
		else if (strcmp(input, "dequeue") == 0)
		{
			number = popQueue(queue);

			if (number != ERROR)
			{
				printf("%d\n", number);
			}
			else
			{
				printf("error\n");
			}
		}
		else if (strcmp(input, "front") == 0)
		{
			number = frontQueue(queue);

			if (number != ERROR)
			{
				printf("%d\n", number);
			}
			else
			{
				printf("error\n");
			}
		}
		else if (strcmp(input, "size") == 0)
		{
			printf("%d\n", getQueueSize(queue));
	    }
		else if (strcmp(input, "clear") == 0)
		{
			clearQueue(queue);

			printf("ok\n");
		}
		else if (strcmp(input, "min") == 0)
		{
			number = minQueue(queue);

			if (number != ERROR)
			{
				printf("%d\n", number);
			}
			else
			{
				printf("error\n");
			}
		}
	}

	free(input);

	destructQueue(queue);

	return 0;
}
