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

	class iterator //�� ã�� �� �ʿ��� ��. PUSH POP �������. ���� ��ȸ�� �� �ִ� ������ ��ģ������ �Ѱ���.
	{
		friend class CList;
	private:
		Node* _cursor; //�����̺� ��� ����. Ŀ��. 
		//���ͷ����͵� ��ü�� ���� ����. 
	public:
		iterator(Node* node = nullptr)
		{
			//���ڷ� ���� Node �����͸� ����
			this->_cursor = node;
		}

		iterator operator ++(int) //����
		{
			//���� ��带 ���� ���� �̵�
			iterator temp(this->_cursor); //nullptr�ȵ���.
			this->_cursor = this->_cursor->_Next;

			return temp;
		}

		iterator& operator++() //���� @@@@
		{
			this->_cursor = this->_cursor->_Next;
			return *this;
		}

		iterator operator --(int)
		{
			iterator temp(this->_cursor); //nullptr�ȵ���.
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
			//���� ����� �����͸� ����
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
		//ù��° ������ ��带 ����Ű�� ���ͷ����� ����
		return iterator(this->_head._Next);

	}

	iterator end()
	{
		//Tail ��带 ����Ű��(�����Ͱ� ���� ��¥ ���� �� ���) ���ͷ����͸� ���� �Ǵ� ������ ������ �� �ִ� ���ͷ����͸� ����
		return iterator(&this->_tail);
	}

	void push_front(T data) //T�� ���ǵǾ� ���� �ʾƼ� �����Ϸ��� T�� ���ǵ��� �ʴ� �� �ڵ带 ������ �ʴ´�.
	{
		//����� NEXT�� INSERT

		//static int i; 
		//�Լ�ȣ���� �� �Ǹ� �� ����ƽ ���������� �����Ϳ����� �ö��� ����.
		//ȣ��ɶ��� �� ���� ����. �������ʹ� ������. �Ϲ� ��� ������ �������� ����. �Լ��� ���Ϲ����� �����ȿ��� ��� üũ��. �Լ��� ���ʷ� ȣ���ߴ°� ���� Ȯ���ϰ� ���� ȣ���̸� ���Ϲ޴��Լ� üũ. 2��°���� �Ȱ�����. 
		//���ο��� int�� ���� ����Ʈ/long�� ����Ʈ �ϳ� ����. �ڵ� ���θ������. int���� push�ϸ� ���� �ö󰡴µ� int���� push 5��. i +5�� . ���� ����Ʈ���� i�� 0��.


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
		//���ϳ�� PREV�� INSERT
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
		//����� NEXT ��� ����
		Node* removed;

		removed = this->_head._Next;
		this->_head._Next->_Next->Prev = &this->_head;
		this->_head._Next = removed->_Next;

		free(removed);

		this->_size--;
	}

	void pop_back()
	{
		//���ϳ�� PREV ��� ����
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
		//���ͷ������� �� ��带 ����.
		//�׸��� ���� ����� ���� ��带 ī��Ű�� ���ͷ����� ����
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

