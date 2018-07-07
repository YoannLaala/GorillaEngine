#include <windows.h>
#include <Viewer/Application.hpp>

class List
{
	struct Node
	{
		int value;
		Node* next;
	};

public:
	List()
		: head(NULL)
		, tail(NULL)
		, size(0)
	{
		
	}

	~List()
	{
		Node* pNodeToDelete = NULL;
		Node* pNode = head;
		while(pNode)
		{
			pNodeToDelete = pNode;
			pNode = pNode->next;
			delete pNodeToDelete;
		}	

		head = NULL; tail = NULL;
	}

	void push_back(int value)
	{
		Node* pNodeNew = new Node();
		pNodeNew->value = value;
		if(!head)
		{
			head = pNodeNew;
		}

		pNodeNew->next = tail;
		tail = pNodeNew;
		++size;
	}

	void pop_back()
	{
		if(tail)
		{
			Node* pNode = head;
			while(pNode->next != tail)
			{
				pNode = pNode->next;
			}
			pNode->next = NULL;
			delete tail;
			--size;
		}
	}

	void push_front(int value)
	{
		Node* pNodeNew = new Node();
		pNodeNew->value = value;
		if(!tail)
		{
			tail = pNodeNew;
		}

		pNodeNew->next = head;
		head = pNodeNew;
		++size;
	}

	void pop_front()
	{
		if(head)
		{
			Node* pNode = head->next;
			delete head;
			head = pNode;
			--size;
		}
	}

	void reverse()
	{
		Node *pPrevious, *pCurrent, *pNext; pCurrent = head; pPrevious = NULL;
		while(pCurrent)
		{
			pNext = pCurrent->next;
			pCurrent->next = pPrevious;
			pPrevious = pCurrent;
			pCurrent = pNext;
		}
		tail = head;
		head = pPrevious;
	}

	void insert_at(int pos, int value)
	{
		Node* pNodeNew = new Node();
		pNodeNew->value = value;
		if(!head || pos == 0)
		{
			pNodeNew->next = head;
			head = pNodeNew;
			++size;
			return;
		}

		Node* pNode = head;
		while(pNode->next && pos > 0)
		{
			pNode = pNode->next;
			--pos;
		}

		pNodeNew->next = pNode->next;
		pNode->next = pNodeNew;
		++size;
	}

	Node* head;
	Node* tail;
	int size;
};

class Stack
{
public:
	Stack()
		: size(0)
	{
	
	}

	~Stack()
	{
	
	}

	inline void push(int value) { values[size++] = value; }
	inline int	pop() { return values[size--]; }
	inline bool is_empty() { return size == 0; }

	void reverse()
	{
		int* pStart = &values[0];
		int* pEnd = &values[size-1];
		while(pStart < pEnd)
		{
			int temp = *pStart;
			*pStart = *pEnd;
			*pEnd = temp;

			++pStart; --pEnd;
		}
	}

private:
	int size;
	int values[10];
};

template <typename Type, typename size_t Size = 10> 
class Queue
{
public:
	Queue()
		: start(0)
		, end(0)
	{
	
	}

	~Queue()
	{
	
	}

	inline void enqueue(Type value) { if(!is_full()){ values[end++ % Size] = value; }; }
	inline Type	dequeue() { return is_empty() ? -1 : values[start++ % Size]; }
	inline bool is_empty() { return end == start; }
	inline bool is_full() { return end-start == Size; }

private:
	int start, end;
	Type values[Size];
};

template <typename KEY, typename VALUE>
class HashMap
{
private:
	struct Node
	{
		KEY _key;
		VALUE _value;
		Node* _pNext;
	};

private:
	Node** _ppMap;
	size_t _uiCapacity;

	size_t HashFunction(size_t uiKey) const { return uiKey; }

public:
	HashMap()
		: _uiCapacity(0)
		, _ppMap(NULL)
	{
		// Nothing to do
	}

	HashMap(size_t uiCapacity)
		: _uiCapacity(uiCapacity)
	{
		_ppMap = new Node*[uiCapacity] { 0 };
	}

	~HashMap()
	{
		delete _ppMap;
	}

	void insert(const KEY& key, const VALUE& value)
	{
		size_t uiIndex = HashFunction(key) % _uiCapacity;
		Node** ppNode = &_ppMap[uiIndex];
		while(*ppNode && (*ppNode)->_key != key)
		{
			ppNode = &((*ppNode)->_pNext);
		}

		if(!*ppNode)
		{
			*ppNode = new Node();
			(*ppNode)->_key = key;
		}
		(*ppNode)->_value = value;
		(*ppNode)->_pNext = NULL;
	}
};

/******************************************************************************
**	Entry Point
******************************************************************************/
int WINAPI wWinMain(HINSTANCE /*_hInstance*/, HINSTANCE /*_hPrevInst*/, LPWSTR /*_lpCmdLine*/, int /*_nCmdShow*/)
{	
	//Gorilla::Math::Vector3 heho = 2.0f * Gorilla::Math::Vector3::One;

	List list;
	list.push_back(1);
	list.insert_at(10, 2);
	list.insert_at(0, 3);
	list.pop_front();
	list.reverse();

	Stack stack;
	stack.push(1);
	stack.push(2);
	stack.push(3);
	stack.pop();
	stack.reverse();

	int value = -1;
	Queue<int, 3> queue;
	bool test = queue.is_empty();
	queue.enqueue(1);
	queue.enqueue(2);
	queue.enqueue(3);
	queue.enqueue(4);
	queue.enqueue(5);
	test = queue.is_full();
	value = queue.dequeue();
	value = queue.dequeue();
	value = queue.dequeue();
	value = queue.dequeue();
	test = queue.is_full();
	test = queue.is_empty();
	queue.enqueue(18);
	queue.enqueue(19);
	queue.enqueue(20);
	queue.enqueue(21);
	queue.enqueue(22);
	test = queue.is_full();
	value = queue.dequeue();
	value = queue.dequeue();
	value = queue.dequeue();
	value = queue.dequeue();
	
	// Launch application
	Gorilla::Viewer::Application kApplication;
	kApplication.Run();

    return 0;
}