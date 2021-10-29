#include<iostream>
#include<cstring>
using namespace std;
const int MAX_LEN = 256;
class String
{
    private:
        const int m_max;
        char* m_str;
        size_t m_len;

    public:
        // def ctor
        String();
        // ctor
        String(const char* str);
        // copy ctor
        String(const String& str);
        // destor
        ~String();

        // ops
        String& operator=(const String& str);
        String& operator+(const String& str);
        String& operator=(const char* str);

        // I/O
        friend ostream& operator<<(ostream& os, const String& str);
        friend istream& operator>>(istream& is, String& str);
};

String::String() : m_max(MAX_LEN)
{
    m_len = 0;
    m_str = new char[1];
    m_str[0] = '\0';
}

String::~String()
{
    cout << "string: " << m_str << " at addr: " << (void*) m_str  << " freed " <<endl;
    delete [] m_str;
}

String::String(const char* str) : m_max(MAX_LEN)
{
    if (!str || !strlen(str) || strlen(str) > m_max)
    {
        m_str = new char[1];
        m_str[0] = '\0';
        m_len = 0;
        return;
    }

    m_len = strlen(str);
    m_str = new char[m_len + 1];
    strncpy(m_str, str, m_len);
    m_str[m_len] = '\0';
}

String::String(const String& str) : m_max(MAX_LEN)
{
    if (!str.m_len)
    {
        m_str = new char[1];
        m_str[0] = '\0';
        m_len = 0;
        return;
    }

    m_len = str.m_len;
    m_str = new char[m_len + 1];
    strncpy(m_str, str.m_str, m_len);
    m_str[m_len] = '\0';
}

String& String::operator=(const String& str)
{
    if (this == &str)
        return *this;

    cout << "string: " << m_str << " at addr: " << (void*) m_str  << " freed " <<endl;
    delete [] m_str;

    if (!str.m_len)
    {
        m_str = new char[1];
        m_str[0] = '\0';
        m_len = 0;
        return *this;
    }

    m_len = str.m_len;
    m_str = new char[m_len + 1];
    strncpy(m_str, str.m_str, m_len);
    m_str[m_len] = '\0';

    return *this;
}

String& String::operator=(const char* str)
{
    if (!str || !strlen(str) || strlen(str) > m_max)
    {
        m_str = new char[1];
        m_str[0] = '\0';
        m_len = 0;
        return *this;
    }

    cout << "string: " << m_str << " at addr: " << (void*) m_str  << " freed " <<endl;
    delete [] m_str;

    m_len = strlen(str);
    m_str = new char[m_len + 1];
    strncpy(m_str, str, m_len);
    m_str[m_len] = '\0';

    return *this;
}

ostream& operator<<(ostream& os, const String& str)
{
    os << str.m_str;
    return os;
}

istream& operator>>(istream& is, String& str)
{
    is.clear();

    char buf[MAX_LEN+1];
    memset(buf, 0, MAX_LEN+1);
    if (is.getline(buf, MAX_LEN))
    {
        str = buf;
    }

    return is;
}

void callme1(String & rsb) {
    cout << "String passed by reference:"<<endl;
    cout << rsb << endl;
}

void callme2(String sb) {
    cout << "String passed by value:\n";
    cout << sb << endl;
}

int main()
{
    String a = String("ahooooooooooooooooooooo");
    String b("wahoooooooooooooooooo");
    String c(a);
    cout << a << " - " << b << " - " << c  << " - " <<endl;
    // c.operator=("") 
    c = "mahoooooo";
    cout << a << " - " << b << " - " << c  << " - " <<endl;
    // c.operator=(a.operator=(b))
    c = a = b;
    cout << a << " - " << b << " - " << c  << " - " <<endl;
    c = c;

    cout << "what saying:" << endl;
    cin >> c;
    cout << "said: " << c << endl;

    cout << "what saying again:" << endl;
    cin >> c;
    cout << "said: " << c << endl;

    callme1(c);
    callme2(c);

    return 0;
}
