#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>

using namespace std;

class String {
public:
	//迭代器的实现:
	//迭代器: 一种访问容器元素的机制,体现封装的特性
	//不需要关注容器的细节,就可以直接访问(可读可写)元素
	//使用方式:类似于指针的操作
	//        1.解引用 --> 获取指向位置的内容
	//        2.位置移动 --> 指向其它位置

	typedef char* iterator;
	typedef const char* const_iterator;

	iterator begin() {
		//第一个元素的位置
		return _str;
	}
	iterator end() {
		//最后一个元素的下一个位置
		return _str + _size;
	}

	const_iterator begin()const {
		return _str;
	}

	const_iterator end()const {
		return _str + _size;
	}


	//无参构造函数
	String()
		:_str(new char[16])
		, _size(0)
		, _capacity(0)
	{
		_str[_size] = '\0';
	}
	//构造函数
	String(const char* str) {
		_size = strlen(str);
		_str = new char[_size + 1];
		strcpy(_str, str);
		_capacity = _size;
	}
	////拷贝构造函数
	//String(const String& str)
	//	:_str(new char[str._capacity+1])
	//	,_size(str._size)
	//	,_capacity(str._capacity)
	//{
	//	//深拷贝:资源拷贝
	//	strcpy(_str, str._str);
	//}
	//拷贝构造函数的现代写法:建议这样写
	String(const String& str)
		:_str(nullptr)
		, _size(0)
		, _capacity(0)
	{
		//调用构造函数:代码复用
		String tmp(str._str);
		Swap(tmp);
	}
	void Swap(String& str) {
		swap(_str, str._str);
		swap(_size, str._size);
		swap(_capacity, str._capacity);
	}
	//深拷贝交换:效率低
	/*void Swap(String& str) {
		//拷贝构造
		String tmp = str;
		//赋值运算符
		str = *this;
		//赋值运算符
		*this = tmp;
	}*/
	//赋值运算符
	//String& operator=(const String& str) {
	//	if (this != &str) {		
	//		//开空间
	//		char* tmp = new char[str._capacity + 1];
	//		//内容拷贝
	//		strcmp(tmp, str._str);
	//		//原有空间释放
	//		delete[] _str;
	//		_str = tmp;
	//		_size = str._size;
	//		_capacity = str._capacity;
	//	}
	//	return *this;
	//}
	//赋值运算符现代写法
	//代码复用:拷贝构造 (传值时进行拷贝构造)
	String& operator=(String str) {
		Swap(str);
		return *this;
	}

	const char* c_str()const {
		return _str;
	}
	//析构函数
	~String() {
		if (_str) {
			delete[]_str;
			_size = _capacity = 0;
			_str = nullptr;
		}
	}

	char& operator[](size_t pos) {
		if (pos < _size)
			return _str[pos];
	}
	const char& operator[](size_t pos)const {
		if (pos < _size)
			return _str[pos];
	}
	size_t size() const {
		return _size;
	}

	void pushback(const char& ch) {
		//判断是否需要增容
		if (_size == _capacity) {
			size_t newC = _capacity == 0 ? 1 : 2 * _capacity;
			reverse(newC);
		}
		//尾插入字符
		_str[_size] = ch;
		//更新_size
		++_size;
		//不要忘记更新'\0'
		_str[_size] = '\0';
		//insert(_size,ch);
	}
	void reverse(size_t n) {
		if (n > _capacity) {
			//开空间 : +1 用来存放'\0'
			char* tmp = new char[n + 1];
			//拷贝内容
			strcpy(tmp, _str);
			//释放原有空间
			delete[] _str;
			_str = tmp;
			//更新容量
			_capacity = n;
		}
	}

	void Append(const char* str) {
		//检查容量
		int len = strlen(str);
		if (_size + len > _capacity) {
			reverse(_size + len);
		}
		//拷贝内容
		strcpy(_str + _size, str);
		//更新_size
		_size += len;
		//insert(_size,str)
	}

	String& operator+=(const char& ch) {
		pushback(ch);
		return *this;
	}
	String& operator+=(const char* str) {
		Append(str);
		return *this;
	}

	void insert(size_t pos, const char& ch) {
		if (pos > _size)
			return;
		//判断是否需要增容  
		if (_size == _capacity) {
			size_t newC = _capacity == 0 ? 1 : 2 * _capacity;
			reverse(newC);
		}
		size_t end = _size + 1;
		//元素向后移动
		while (end > pos) {
			_str[end] = _str[end - 1];
			--end;
		}
		//插入元素
		_str[pos] = ch;
		//更新
		++_size;
	}

	void insert(size_t pos, const char* str) {
		if (pos > _size)
			return;
		//判断是否需要增容
		size_t len = strlen(str);
		if (_size + len > _capacity) {
			reverse(_size + len);
		}

		size_t end = _size + len;
		while (end > pos + len - 1) {
			_str[end] = _str[end - len];
			--end;
		}
		//插入字符串		
		for (int i = 0; i < len; ++i) {
			_str[pos + i] = str[i];
		}
		//更新_size
		_size += len;
	}
	//调整_size
	void resize(size_t n,const char ch =  '\0') {
		if (n > _capacity) {
			//增容
			reverse(n);
		 }
		if (n > _size) {
			memset(_str + _size, ch, n - _size);
		}
		_size = n;
		_str[_size] = '\0';
	}
	void popback() {
		earse(_size - 1, 1);
	}
	
	void earse(size_t pos, size_t len) {
		if (pos < _size) {
			if (pos + len >= _size) {
				_size = pos;
				_str[_size] = '\0';
			}
			else {
				//移动元素[pos+len,_size] --> [pos,_size-len]
				for (int i = pos + len; i > pos; --i) {
					_str[pos++] = _str[i];
				}
				_size -= len;
			}
		}
	}
	//字符串查找,返回索引
	size_t find(const char* str) {
		char* ptr = strstr(_str, str);  //strstr找到子串会返回指针
		if (ptr) { 
			return ptr - _str;
		}
		else
			return npos;
	}
private:
	char* _str;
	size_t _size;
	size_t _capacity;
public:
	static const size_t npos;
};
//静态成员变量类外初始化
const size_t String::npos = -1;
//加法运算符重载:不改变加数
String operator+(const String& s, const String& str) {
	String ret(s); // 拷贝构造
	ret += str.c_str();
	return ret;
}
String operator+(const String& s, const char* str) {
	String ret(s);
	ret += str;
	return ret;
}
String operator+(const String& s, const char& ch) {
	String ret(s);
	ret += ch;
	return ret;
}
ostream& operator<<(ostream& cout, const String& str) {
	cout << str.c_str() << endl;
	return cout;
}

void test() {
	String str("abcdefgh");

	cout << str.c_str() << endl;

	String::iterator it = str.begin();
	while (it != str.end()) {
		cout << *it << " ";
		++it;
	}
	cout << endl;

	//范围for :底层利用迭代器实现
	for (const char& ch : str) {
		cout << ch << " ";
	}
	cout << endl;

	//[]遍历
	for (int i = 0; i < str.size(); ++i) {
		cout << str[i] << " ";
	}
	cout << endl;
}