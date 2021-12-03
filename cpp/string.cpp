#include<iostream>
#include<cstring>
using namespace std;
const int MAX_LEN = 256;
class String
{
    private:
        const size_t m_max;
        char* m_str;
        size_t m_len;

    public:
        // def ctor
        String(const char*, const size_t);
        // copy ctor
        String(const String &);
        // detor
        ~String();

        // ops
        String& operator=(const String&);
        String& operator=(const char*);
        String& operator+(const String&);

        // I/O
        friend bool operator==(const String&, const String&);
        friend ostream& operator<<(ostream&, const String &);
        friend istream& operator>>(istream&, const String &);
};

// def ctor
String::String(const char* s = NULL, const size_t max = MAX_LEN): m_max(max)
{
    if (!s)
        m_len = 0;
    else 
        m_len = strlen(s);

    if (m_len > m_max)
    {
        cerr << "String len exceeds the max len of: " << m_max << endl;
        return;
    }

    m_str = new char[m_len + 1];
    strncpy(m_str, s, m_len);
    m_str[m_len] = '\0';
    cout << "ctor\t ---- string created for :'" << m_str << "' at addr: " << (void*)m_str << endl;
}

String::~String()
{
    cout << "detor\t ---- string freed   for :'" << m_str << "' at addr: " << (void*)m_str << endl;
    if (m_str)
        delete [] m_str;
}

// cp ctor
String::String(const String& s): m_max(s.m_max)
{
    m_len = s.m_len;
    m_str = new char[m_len + 1];
    strncpy(m_str, s.m_str, m_len);
    m_str[m_len] = '\0';
    cout << "cp ctor\t ---- string created for :'" << m_str << "' at addr: " << (void*)m_str << endl;
}

String& String::operator=(const String& s)
{
    if (this == &s)
        return *this;

    if (s.m_len > m_max)
        return *this;

    cout << "op =\t ---- string freed   for :'" << m_str << "' at addr: " << (void*)m_str << endl;
    delete [] m_str;

    m_len = s.m_len;
    m_str = new char[m_len + 1];
    strncpy(m_str, s.m_str, m_len);
    m_str[m_len] = '\0';
    cout << "op =\t ---- string created for :'" << m_str << "' at addr: " << (void*)m_str << endl;
    return *this;
}

String& String::operator=(const char* s)
{
    if (!s)
        return *this;

    cout << "op =\t ---- string will be created for :'" << s << "'" << endl;
    *this = String(s);
    return *this;
}

ostream& operator<<(ostream & os, const String & s)
{
    os << s.m_str;
    return os;
}

istream& operator>>(istream& is, String& s)
{
    char * buf = new char[MAX_LEN];
    memset(buf, 0, MAX_LEN);

    is.clear();
    if (is.getline(buf, MAX_LEN))
        s = buf;

    is.clear();

    return is;
}

int main()
{
    // ctor
    String a = String("ahooooooooooooooooooooo A");
    // ctor
    String b("ahooooooooooooooooooooo B");
    // ctor
    String c = "ahooooooooooooooooooooo C";
    // def ctor
    String d;
    // copy ctor
    String e(a);
    // op = ctor
    e = b;
    // op = ctor
    e = "ahooooooooooooooooooooo E";
    // no op
    e = e;

    cout << endl;
    operator<<(cout, a) << endl << endl;

    cout << "what saying:" << endl;
    cin >> d;
    cout << "said: " << d << endl;

    return 0;
}
