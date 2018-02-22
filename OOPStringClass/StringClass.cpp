// Name: Garrett McDonnell
// String Class for Object Oriented Programming

#include <iostream>

using std::cout;
using std::endl;
using std::ostream;


struct myString
{
    char *phrase;
    int len;

    myString();
    myString(const char* msg);
    myString(myString& obj);
    void operator = (const char* msg);
    void operator = ( myString& obj);
    bool operator == ( myString& obj);
    bool operator == (const char* msg);
    bool operator != ( myString& obj);
    bool operator != ( const char* msg);
    bool operator < ( myString& obj);
    bool operator < (const char* msg);
    friend ostream& operator << (ostream& obj, myString& msg);
};

int main()
{
    // Tests for myString class
    cout << "Hello world!" << endl;
    myString x;
    x="howdy";
    cout << x << endl;
    myString y=x;
    cout << y << endl;
    y="hey";
    cout << y << endl << x << endl;
    myString z{"bro"};
    cout << z << endl;
    y=z;
    if (y==z)
        cout << "sup" << endl;
    if (x!=y)
        cout << "cat" << endl;
    if (z=="bro")
        cout << "space" << endl;
    if (x!="bro")
        cout << "time" << endl;
    x="bros";
    y="bro";
    if (x<y)
        cout << "true" << endl;
    else
        cout << "false" << endl;
    if (x<"bro")
        cout << "dumb";
    else
        cout << "dumb dumb" << endl;
    cout << x << ' ' << y << ' ' << z;
}

myString::myString() : phrase(nullptr), len(0)
{}

myString::myString(const char* msg)
{
    int i = 0;
    const char *t = msg;
    while(*msg++)
    {
        i++;
    }
    len=i;
    phrase = new char[len];
    int x = 0;
    for(x;*t;t++)
    {
        phrase[x] = *t;
        x++;
    }
}

myString::myString(myString& obj)
{
    const char *t=obj.phrase;
    int x=0;
    phrase=new char[obj.len];
    len=obj.len;
    for (x; *t; t++)
    {
        phrase[x]=*t;
        x++;
    }
}

void myString::operator = (const char* msg)
{
    int i=0;
    const char *t=msg;
    while (*msg++)
    {
        i++;
    }
    len=i;
    phrase=new char[len];
    int x=0;
    for (x; *t; t++)
    {
        phrase[x]=*t;
        x++;
    }
}


void myString::operator = ( myString& obj)
{
    const char *t=obj.phrase;
    phrase=new char[obj.len];
    len=obj.len;
    int x=0;
    for (x; *t; t++)
    {
        phrase[x]=*t;
        x++;
    }
}

bool myString::operator == ( myString& obj)
{
    int tally=0;
    if (len==obj.len)
    {
        int x=0;
        while (x<len)
        {
            if (phrase[x]==obj.phrase[x])
                ++tally;
            ++x;
        }
        if (tally==len)
            return true;
    }
    return false;
}

bool myString::operator == (const char* msg)
{
    int i=0;
    const char *t=msg;
    while (*msg++)
    {
        i++;
    }
    int tally=0;
    if (len==i)
    {
        int x=0;
        while (x<len)
        {
            if (phrase[x]==t[x])
                ++tally;
            ++x;
        }
        if (tally==len)
            return true;
    }
    return false;
}

bool myString::operator != ( myString& obj)
{
    int tally=0;
    if (len==obj.len)
    {
        int x=0;
        while (x<len)
        {
            if (phrase[x]==obj.phrase[x])
                ++tally;
            ++x;
        }
        if (tally!=len)
            return true;
        else
            return false;
    }
    else
        return true;
}

bool myString::operator != (const char* msg)
{
    int i=0;
    const char *t=msg;
    while (*msg++)
    {
        i++;
    }
    int tally=0;
    if (len==i)
    {
        int x=0;
        while (x<len)
        {
            if (phrase[x]==t[x])
                ++tally;
            ++x;
        }
        if (tally!=len)
            return true;

        else
            return false;
    }
    else
        return true;
}

bool myString::operator < ( myString& obj)
{
    int x=0;
    while (x<len)
    {
        if (phrase[x]<obj.phrase[x])
            return true;
        else if (phrase[x]>obj.phrase[x])
            return false;
        else
            ++x;
    }
    return false;
}

bool myString::operator < (const char* msg)
{
    int i=0;
    const char *t=msg;
    while (*msg++)
    {
        i++;
    }
    int x=0;
    while (x<i)
    {
        if (phrase[x]<t[x])
            return true;
        else if (phrase[x]>t[x])
            return false;
        else
            ++x;
    }
    return false;
}

ostream& operator << (ostream& obj,  myString& msg)
{
    obj << msg.phrase;
    return obj;
}
