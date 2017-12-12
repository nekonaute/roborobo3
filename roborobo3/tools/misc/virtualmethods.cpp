// Quick notes on inheritance and virtual functions
// cf. https://stackoverflow.com/questions/47773930/access-overriden-parent-virtual-method-in-c

#include <iostream>

using namespace std;

class Base
{
    public:
    virtual void f(){ cout << "Base::f()" << endl; }
    virtual void g(){ cout << "Base::g()" << endl; }
    void h(){ cout << "Base::h()" << endl; }
};

class Derived : public Base
{
    public:
    void f(){ cout << "Derived::f()" << endl; }
    override void g(){ cout << "Derived::g()" << endl; }
    void h(){ cout << "Derived::h()" << endl; }
};

int main()
{
    Base *pBase = new Derived;
    pBase->f();         // "Derived::f()"
    pBase->g();         // "Derived::g()"
    pBase->h();         // "Base::h()"
    pBase->Base::g();   // "Base::g()"
    
    Derived *pDerived = new Derived;
    pDerived->f();      // "Derived::f()"
    pDerived->g();      // "Derived::g()"
    pDerived->h();      // "Derived::h()"
    
    return 0;
}

/*
OUTPUT:
 Derived::f()
 Derived::g()
 Base::h()
 Base::g()
 Derived::f()
 Derived::g()
 Derived::h()
*/
