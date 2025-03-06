#include <iostream>
#include <memory>

class MyClass {
    public:
        MyClass(int val) : value(val) { 
            std::cout << "Constructor: " << value << std::endl; 
        }
        ~MyClass() { 
            std::cout << "Destructor: " << value << std::endl; 
        }
        void show() { 
            std::cout << "Value: " << value << std::endl; 
        }
    private:
        int value;
    };

class MyClass_Unique {
    public:
        MyClass_Unique(int val) : value(val) { 
            std::cout << "Constructor: " << value << std::endl; 
        }
        ~MyClass_Unique() { 
            std::cout << "Destructor: " << value << std::endl; 
        }
        void show() { 
            std::cout << "Value: " << value << std::endl; 
        }
    private:
        int value;
    };

    
int main() {

    std::cout << "Share ptr test: " << std::endl; // 

    std::shared_ptr<MyClass> ptr1 = std::make_shared<MyClass>(10);
    std::shared_ptr<MyClass> ptr2 = ptr1; // 共享所有权
    std::shared_ptr<MyClass> ptr3 = ptr2; // 共享所有权

    std::cout << "Reference Count: " << ptr1.use_count() << std::endl; // 3
    ptr2.reset(); // 释放 ptr2
    std::cout << "Reference Count after ptr2 reset: " << ptr1.use_count() << std::endl; // 2
    ptr3.reset(); // 释放 ptr3
    std::cout << "Reference Count after ptr3 reset: " << ptr1.use_count() << std::endl; // 1


    std::cout << "Unique ptr test: " << std::endl; // 

    std::unique_ptr<MyClass_Unique> uniq_ptr1 = std::make_unique<MyClass_Unique>(20);
    // std::unique_ptr<MyClass_Unique> uniq_ptr2 = ptr1; // ❌错误：不能复制 unique_ptr
    std::unique_ptr<MyClass_Unique> uniq_ptr2 = std::move(uniq_ptr1); // ✅ 只能通过 move 转移所有权
    if (!uniq_ptr1) {
        std::cout << "ptr1 is now empty!\n";
    }
    uniq_ptr2->show();
    
    std::cout << "\nPress Enter to exit...";
    std::cin.get();
    return 0; 
}

