#include <iostream>

class MyClass {
   public:
      struct MyThing {
         int x;
      };

      int y;
};

template<class T> void func (T& myClass, typename T::MyThing myThing) {
   myClass.y = myThing.x;
}

int main () {

   MyClass myClass;
   MyClass::MyThing myThing = { 87 };

   func (myClass, myThing);

   std::cout << "myClass.y = " << myClass.y << std::endl;

   return 0;
}
