#pragma once

template <typename T> 
class CList 
{
public:
	struct Node 
	{
		T _data;
		Node* _Prev;
		Node* _Next;
	};

	class iterator //값 찾을 때 필요한 거. PUSH POP 상관없음. 값을 순회할 수 있는 권한을 이친구에게 넘겨줌.
	{
		friend class CList;
	private:
		Node* _cursor; //프라이빗 멤버 변수. 커서. 
		//이터레이터도 객체로 생성 가능. 
	public:
		iterator(Node* node = nullptr)
		{
			//인자로 들어온 Node 포인터를 저장
			this->_cursor = node;
		}

		iterator operator ++(int) //후위
		{
			//현재 노드를 다음 노드로 이동
			iterator temp(this->_cursor); //nullptr안들어가게.
			this->_cursor = this->_cursor->_Next;

			return temp;
		}

		iterator& operator++() //전위 @@@@
		{
			this->_cursor = this->_cursor->_Next;
			return *this;
		}

		iterator operator --(int)
		{
			iterator temp(this->_cursor); //nullptr안들어가게.
			this->_cursor = this->_cursor->_Prev;

			return temp;
		}

		iterator& operator--()
		{

			this->_cursor = this->_cursor->_Prev;
			return *this;
		}

		T& operator *()
		{
			//현재 노드의 데이터를 뽑음
			return this->_cursor->_data;

		}

		bool operator ==(const iterator& other)
		{
			return (this->_cursor == other._cursor);
		}

		bool operator !=(const iterator& other)
		{
			return (this->_cursor != other._cursor);
		}
	};

public:
	CList()
	{
		this->_head._Next = &this->_tail;
		this->_tail._Prev = &this->_head;
	}

	~CList()
	{
		clear();
	}

	iterator begin()
	{
		//첫번째 데이터 노드를 가리키는 이터레이터 리턴
		return iterator(this->_head._Next);

	}

	iterator end()
	{
		//Tail 노드를 가리키는(데이터가 없는 진짜 더미 끝 노드) 이터레이터를 리턴 또는 끝으로 인지할 수 있는 이터레이터를 리턴
		return iterator(&this->_tail);
	}

	void push_front(T data) //T가 정의되어 있지 않아서 컴파일러가 T가 정의되지 않는 한 코드를 만들지 않는다.
	{
		//헤드노드 NEXT에 INSERT

		//static int i; 
		//함수호출이 안 되면 이 스태틱 지역변수는 데이터영역에 올라가지 않음.
		//호출될때만 이 문장 실행. 다음부터는 무시함. 일반 상수 넣으면 전역으로 잡힘. 함수를 리턴받으면 지역안에서 계속 체크함. 함수를 최초로 호출했는가 여부 확인하고 최초 호출이면 리턴받는함수 체크. 2번째부턴 안가져옴. 
		//메인에서 int형 지역 리스트/long형 리스트 하나 만듬. 코드 따로만들어짐. int에서 push하면 저게 올라가는데 int에서 push 5번. i +5됨 . 롱형 리스트에선 i가 0임.


		Node* new_node = (Node*)malloc(sizeof(Node));
		new_node->_data = data;

		new_node->_Prev = &this->_head;
		new_node->_Next = this->_head._Next;

		this->_head._Next->_Prev = new_node;
		this->_head._Next = new_node;

		this->_size++;

	}

	void push_back(T data)
	{
		//테일노드 PREV에 INSERT
		Node* new_node = (Node*)malloc(sizeof(Node));
		new_node->_data = data;

		new_node->_Prev = this->_tail._Prev;
		new_node->_Next = &this->_tail;

		this->_tail._Prev->_Next = new_node;
		this->_tail._Prev = new_node;

		this->_size++;
	}

	void pop_front()
	{
		//헤드노드 NEXT 노드 제거
		Node* removed;

		removed = this->_head._Next;
		this->_head._Next->_Next->Prev = &this->_head;
		this->_head._Next = removed->_Next;

		free(removed);

		this->_size--;
	}

	void pop_back()
	{
		//테일노드 PREV 노드 제거
		Node* removed;

		removed = this->_tail._Prev;
		this->_tail._Prev->_Prev->_Next = &this->_tail;
		this->_tail._Prev = removed->_Prev;

		free(removed);

		this->_size--;
	}

	void clear()
	{
		iterator iter;

		for (iter = begin(); iter != end();)
		{
			iter = erase(iter);
		}
	}

	int size()
	{
		return _size;
	};

	bool is_empty()
	{
		return (_size == 0);
	};

	iterator erase(iterator iter)
	{
		//이터레이터의 그 노드를 지움.
		//그리고 지운 노드의 다음 노드를 카리키는 이터레이터 리턴
		Node* remove = iter._cursor;

		iter._cursor->_Prev->_Next = iter._cursor->_Next;
		iter._cursor->_Next->_Prev = iter._cursor->_Prev;
		iter._cursor = iter._cursor->_Next;

		free(remove);
		this->_size--;
		return iter;
	}

	void remove(T Data)
	{
		iterator iter;

		for (iter = begin(); iter != end();)
		{
			if (*iter == Data)
				iter = erase(iter);
			else
				++iter;
		}
	}

private:
	int _size = 0;
	Node _head;
	Node _tail;
};

