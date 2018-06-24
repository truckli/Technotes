# include <iostream>
# include <memory>
# include <cstring>
using namespace std;


class MyString {
private:
	char *m_data;
public:
	MyString();
	MyString(const char* ptr);
	MyString(const MyString& rhs);
	~MyString();
	MyString& operator=(const MyString& rhs);
	MyString operator+(const MyString& rhs);
	char operator[](const unsigned int index);
	bool operator==(const MyString& rhs);
	friend ostream& operator<<(ostream& output, const MyString &rhs);
};
//默认的构造函数
&nbsp;MyString::MyString() {
	m_data = new char[1];
	*m_data = '\0';
}
//使用const char* 来初始化
 MyString::MyString(const char* ptr) {
	if (NULL == ptr) {
		m_data = new char[1];
		*m_data = '\0';
	} else {
		int len = strlen(ptr);
		m_data = new char[len + 1];
		strcpy(m_data, ptr);
	}
}
//拷贝构造函数
&nbsp;MyString::MyString(const MyString& rhs) {
	int len = strlen(rhs.m_data);
	m_data = new char[len + 1];
	strcpy(m_data, rhs.m_data);
}
bool MyString::operator ==(const MyString& rhs) {
	int result = strcmp(m_data, rhs.m_data);
	if (0 == result)
		return true;
	else
		return false;
}
//赋值操作符
&nbsp;MyString& MyString::operator =(const MyString& rhs) {
	if (this != &rhs) {
		delete[] m_data;
		m_data = new char[strlen(rhs.m_data) + 1];
		strcpy(m_data, rhs.m_data);
	}
	return *this;
}
//重载运算符+
&nbsp;MyString MyString::operator+(const MyString &rhs) {
	MyString newString;
	if (!rhs.m_data)
		newString = *this;
	else if (!m_data)
		newString = rhs;
	else {
		newString.m_data = new char[strlen(m_data) + strlen(rhs.m_data) + 1];
		strcpy(newString.m_data, m_data);
		strcat(newString.m_data, rhs.m_data);
	}
	return newString;
}
//重载下标运算符
 char MyString::operator [](const unsigned int index) {
	return m_data[index];
}
//析构函数
&nbsp;MyString::~MyString() {
	delete[] m_data;
}
//重载<<
&nbsp;ostream& operator<<(ostream& output, const MyString &rhs) {
	output << rhs.m_data;
	return output;
}
int main() {
	const char* p = "hello,world";
	MyString s0 = "hello,world";
	MyString s1(p);
	MyString s2 = s1;
	MyString s3;
	s3 = s1;
	MyString s4 = s3 + s1;
	bool flag(s1 == s2);
	cout << s0 << endl;
	cout << s1 << endl;
	cout << s2 << endl;
	cout << s3 << endl;
	cout << flag << endl;
	char result = s3[1];
	cout << result << endl;
	cout << s4 << endl;
	return 0;
}

