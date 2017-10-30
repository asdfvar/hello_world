#include<iostream>
using namespace std;
  
class Base
{
public:
    virtual void show() { cout<<" In Base \n"; }
};
  
class Derived: public Base
{
public:
    void show() { cout<<"In Derived \n"; }
};
  
int main(void)
{
    cout << "------" << endl;

    Base bp;
    bp.show();  // RUN-TIME POLYMORPHISM

    cout << "------" << endl;

    Derived dr;
    dr.show();

    return 0;
}
