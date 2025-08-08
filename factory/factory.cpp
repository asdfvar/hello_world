#include <iostream>
#include <memory> // For std::unique_ptr

// Product Interface
class Product {
   public:
      virtual void use () const = 0;
      virtual ~Product () = default;
};

// Concrete Product A
class ConcreteProductA : public Product {
   public:
      void use () const override {
         std::cout << "Using Concrete Product A." << std::endl;
      }
};

// Concrete Product B
class ConcreteProductB : public Product {
   public:
      void use () const override {
         std::cout << "Using Concrete Product B." << std::endl;
      }
};

// Creator Interface (Factory)
class Creator {
   public:
      // The factory method
      virtual std::unique_ptr<Product> createProduct() const = 0;
      virtual ~Creator () = default;

      // An operation that uses the product
      void someOperation () const {
         std::unique_ptr<Product> product = this->createProduct ();
         product->use ();
      }
};

// Concrete Creator A
class ConcreteCreatorA : public Creator {
   public:
      std::unique_ptr<Product> createProduct () const override {
         return std::make_unique<ConcreteProductA> ();
      }
};

// Concrete Creator B
class ConcreteCreatorB : public Creator {
   public:
      std::unique_ptr<Product> createProduct () const override {
         return std::make_unique<ConcreteProductB> ();
      }
};

int main () {
   std::cout << "Creating product using ConcreteCreatorA:" << std::endl;
   std::unique_ptr<Creator> creatorA = std::make_unique<ConcreteCreatorA> ();
   creatorA->someOperation();

   std::cout << std::endl;

   std::cout << "Creating product using ConcreteCreatorB:" << std::endl;
   std::unique_ptr<Creator> creatorB = std::make_unique<ConcreteCreatorB> ();
   creatorB->someOperation ();

   return 0;
}
