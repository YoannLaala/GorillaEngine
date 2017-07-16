#include <windows.h>
#include <Viewer/Application.hpp>

//namespace Gorilla
//{
//	class DynamicPointLight : public Gorilla::Engine::Component
//	{
//	public:
//		COMPONENT_DECLARE(DynamicPointLight);
//
//		virtual void Update() override
//		{
//			/*Gorilla::Component::Node* pCpnNode = GetOrCreate<Gorilla::Component::Node>();
//			float32 fTime = GetTime()->GetToltalTime();
//			pCpnNode->SetPosition(cosf(fTime) * 0.7f, sinf(fTime) * 0.7f, sinf(fTime) * 0.7f);*/
//			//pCpnNode->RotateY(GetTime()->GetDeltaTime());
//		}
//	};
//	COMPONENT_REGISTER(DynamicPointLight);
//
//	class GlobalIlluminationScene : public Gorilla::Engine::Component
//	{
//	public:
//		COMPONENT_DECLARE(GlobalIlluminationScene);
//
//		virtual void Update() override
//		{
//			//GetRenderer()->GetGizmo()->PushQuad();
//		}
//	};
//	COMPONENT_REGISTER(GlobalIlluminationScene);
//}

//namespace Gorilla
//{
//	class DebuggerCo : public Gorilla::Engine::Component
//	{
//	public:
//		COMPONENT_DECLARE(DebuggerCo);
//
//		template <typename T> 
//		void WriteValue(const Debugger::Item* _pItem, JsonWriter& _kStream) 
//		{ 
//			T kValue; 
//			_pItem->GetValue(kValue); 
//			_kStream.Write(_pItem->GetName().GetBuffer(), kValue);
//		}
//
//		void WriteItem(const Debugger::Item* _pItem, JsonWriter& _kStream)
//		{
//			const uint32 uiItemCount = _pItem->GetItemCount();
//			for(uint32 uiItem = 0; uiItem < uiItemCount; ++uiItem)
//			{
//				const Debugger::Item* pItemChild = _pItem->GetItem(uiItem);
//				switch(pItemChild->GetType())
//				{
//					case Debugger::Item::Object:
//					{
//						_kStream.Write(pItemChild->GetName().GetBuffer());
//						_kStream.StartObject();
//							WriteItem(pItemChild, _kStream);
//						_kStream.EndObject();
//						break;
//					}
//
//					case Debugger::Item::Boolean: { WriteValue<bool>(pItemChild, _kStream); break; }
//					case Debugger::Item::Float32: { WriteValue<float32>(pItemChild, _kStream); break; }
//				}
//			}
//		}
//
//		//!	@brief		OnCategorySelected
//		//!	@date		2015-04-04
//		//void OnCategorySelected(void* _pData, const Web::WebValueList* _pValueList, Web::WebValue* /*_pValueOut*/)
//		//{
//		//	DebuggerCo* pHandler = static_cast<DebuggerCo*>(_pData);
//		//	
//		//	// Category
//		//	String sCategoryName;
//		//	_pValueList->Get(0)->Read(sCategoryName);
//		//	//Debugger::GetInstance()->SetFilter(sCategoryName.GetBuffer());
//		//}
//
//		virtual void Start() override
//		{
//			Debugger::Create();
//		}
//
//		virtual void Update() override
//		{
//			// Activate / Deactivate the debugger
//			//if(GetInput()->IsKeyPressed(Keyboard::F12))
//			//{
//			//	if(!Debugger::IsCreated())
//			//	{
//			//		Debugger::Create();
//			//	}
//			//	else
//			//	{
//			//		Debugger::Destroy();
//			//	}
//
//			//	/*bool bActivate = !Debugger::GetInstance()->IsActivated();
//			//	if(bActivate)
//			//	{
//			//		Gorilla::Component::WebView* pView = GetOrCreate<Gorilla::Component::WebView>();
//			//		pView->Url = "F:/projects/GorillaEngine/Projects/MyGame/Asset/GUI/Profiler.html";
//			//		pView->GetPage()->GetRoot()->CreateFunction("onCategorySelected", &OnCategorySelected, this);
//			//	}
//			//	else
//			//	{
//			//		Debugger::GetInstance()->Deactivate();
//			//		Remove<Component::WebView>();
//			//	}*/
//			//}
//
//			// Update the content id activated
//			if(Debugger::IsCreated())
//			{
//				JsonWriter kStream;
//				kStream.StartObject();
//				WriteItem(&Debugger::GetInstance()->Root, kStream);
//				kStream.EndObject();
//
//				LOG_PRINT(kStream.GetBuffer());
//
//				/*const Debugger::Item* pItem = Debugger::GetInstance()->GetItem(aName[ECategory::Performance]);
//				if(pItem)
//				{
//					Gorilla::Component::WebView* pView = GetOrCreate<Gorilla::Component::WebView>();
//					pView->GetPage()->GetRoot()->ExecuteFunction("onUpdate");
//				}*/
//			}
//		}
//
//		virtual void Stop() override
//		{
//			//Debugger::GetInstance()->Deactivate();
//		}
//	};
//	COMPONENT_REGISTER(DebuggerCo);
//}
//
//#define BIG_TEST(...) { Gorilla::String sValue; PP_MACRO_CALL(BIG_TEST, sValue, __VA_ARGS__) }
//#define BIG_TEST_1(_string1_) printf(_string1_.GetBuffer());
//#define BIG_TEST_2(_string1_, _string2_) BIG_TEST_1(_string1_.Append(_string2_))
//#define BIG_TEST_3(_string1_, _string2_, _string3_) BIG_TEST_2(_string1_.Append(_string2_), _string3_)
//#define BIG_TEST_4(_string1_, _string2_, _string3_, _string4_) BIG_TEST_3(_string1_.Append(_string2_), _string3_, _string4_)
//#define BIG_TEST_5(_string1_, _string2_, _string3_, _string4_, _string5_) BIG_TEST_4(_string1_.Append(_string2_), _string3_, _string4_, _string5_)
//#define BIG_TEST_6(_string1_, _string2_, _string3_, _string4_, _string5_, _string6_) BIG_TEST_5(_string1_.Append(_string2_), _string3_, _string4_, _string5_, _string6_)

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

	inline void enqueue(Type value) { if(!is_full()){ values[end++] = value; }; }
	inline Type	dequeue() { return is_empty() ? -1 : values[start++]; }
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

	Queue<int, 20> queue;
	bool test = queue.is_empty();
	queue.enqueue(1);
	queue.enqueue(2);
	queue.enqueue(3);
	queue.enqueue(4);
	queue.enqueue(5);
	test = queue.is_full();
	queue.dequeue();
	queue.dequeue();
	queue.dequeue();
	queue.dequeue();
	
	// Launch application
	Gorilla::Viewer::Application kApplication;
	kApplication.Run();

    return 0;
}