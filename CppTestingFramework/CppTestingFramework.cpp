// CppTestingFramework.cpp : Defines the entry point for the application.
//

#include "CppTestingFramework.h"
#include "./UnitTest/UnitTest.h"


#include <vector>
#include <list>
#include <deque>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <array>
#include <string>


using namespace std;


class FunctionHolder {
private:
    std::vector<std::function<void ()>> funcH;
public:
    template <typename Func>
    void addFuncToHolder(Func&& func) {
        funcH.emplace_back(std::forward<Func>(func));
    }

    void execute() {
        for (const auto& func : funcH)
        {
            std::cout << "EXECUTING FUNCTIONS;" << std::endl;
            func();
        }
    }

    template <typename Func, typename... Args>
    auto lambdafy(Func&& f, Args... args) {
        // The lambda captures all arguments by copy.
        return [=]() {
            // Determine the return type of f(args...)
            using Ret = std::invoke_result_t<Func, Args...>;

            if constexpr (std::is_void_v<Ret>) {
                // If the function returns void, just call it.
                f(args...);
            }
            else {
                // Otherwise, capture the result and print it.
                Ret res = f(args...);
                std::cout << "Result of the function is " << res << std::endl;
            }
            };
    }
};

class OverLoadedClass {
private:
    int value;
public:
    OverLoadedClass(int v = 0) : value(v) {}

    int add(int a, int b) {
        std::cout << "executing int add(int, int)" << std::endl;
        return a + b;
    }

    int add(int a) {
        std::cout << "executing int add(int)" << std::endl;
        return this->value + a;
    }
};


class TestClass : public UnitTest<TestClass> {
public:
    int value;

    TestClass(int val) : value(val) {}

    // Equality operator
    bool operator==(const TestClass& other) const {
        return value == other.value;
    }

    // Inequality operator
    bool operator!=(const TestClass& other) const {
        return value != other.value;
    }

    // Boolean conversion (e.g., 0 is false, nonzero is true)
    explicit operator bool() const {
        return value != 0;
    }

    // Stream output operator (to allow printing in printResult)
    friend std::ostream& operator<<(std::ostream& os, const TestClass& obj) {
        return os << "TestClass(" << obj.value << ")";
    }

    // Implementing the runTests function
    void runTests() {
        UnitTest<TestClass>& unittest = UnitTest<TestClass>::getInstance();

        TestClass myTestClass1 = 10;
        TestClass myTestClass2 = 10;

        UnitTest<int>& intunittest = UnitTest<int>::getInstance();
        intunittest.assertNotEqual(4, myTestClass1.value);


        std::function<bool(const TestClass*, const TestClass*)> test =
            [&unittest](const TestClass* a, const TestClass* b) -> bool {
                return unittest.assertEqual(*a, *b);
            };


        unittest.addAssertion(
            static_cast<bool (UnitTest<TestClass>::*)(const TestClass&, const TestClass&)>(&UnitTest<TestClass>::assertEqual),
            &myTestClass1, 
            &myTestClass2);

        
        unittest.addAssertion([&unittest](const TestClass* a, const TestClass* b) -> bool {
            return unittest.assertEqual(*a, *b);
            }, &myTestClass1, &myTestClass2);
        
        unittest.addAssertion(
            &UnitTest<TestClass>::assertEqual,
            new TestClass(3), 
            new TestClass(3)
        );


        // Run the assertions
        unittest.runTests();  // Calling the base class method
    }

};

struct Base {};
struct Derived : Base {};
struct Unrelated {};


struct TestStruct {
    int value;

    // Equality operator
    bool operator==(const TestStruct& other) const {
        return value == other.value;
    }

    // Inequality operator
    bool operator!=(const TestStruct& other) const {
        return value != other.value;
    }

    // Boolean conversion (e.g., 0 is false, nonzero is true)
    explicit operator bool() const {
        return value != 0;
    }

    // Stream output operator (to allow printing in printResult)
    friend std::ostream& operator<<(std::ostream& os, const TestStruct& obj) {
        return os << "TestStruct(" << obj.value << ")";
    }
};

void TestStructTest() {
    UnitTest<TestStruct>& test = UnitTest<TestStruct>::getInstance();
    
    TestStruct obj1{ 10 };
    TestStruct obj2{ 10 };
    TestStruct obj3{ 5 };
    TestStruct objFalse{ 0 };  // Should behave as `false` in `assertTrue`
    std::vector<TestStruct> testList = { TestStruct(1), TestStruct(5), TestStruct(10) };


    std::cout << "\n=== Running Unit Tests on TestClass ===\n";

    // Equality Tests
    test.assertEqual(obj1, obj2);  // Pass
    test.assertEqual(obj1, obj3);  // Fail
    //test.assertEqual(TestStruct(5), testList);  //  Pass if TestClass(5) is in `testingList`

    // Inequality Tests
    test.assertNotEqual(obj1, obj3);  //  Pass
    test.assertNotEqual(obj1, obj2);  //  Fail

    // Boolean Tests
    test.assertTrue(obj1);  //  Pass
    test.assertFalse(objFalse); //  Pass
    test.assertFalse(obj1); //  Fail


    std::shared_ptr<TestStruct> testObj = std::make_shared<TestStruct>(200);
    std::shared_ptr<TestStruct> testObj2 = testObj;

    //UnitTest<TestStruct>& testClassTest = UnitTest<TestStruct>::getInstance();
    test.assertIs(*testObj, *testObj2);

    std::cout << "\n=== Finishing Unit Tests on TestClass ===\n";
}

void assertIsTest() {
    UnitTest<TestStruct>& test = UnitTest<TestStruct>::getInstance();
    TestStruct obj1{ 10 };
    TestStruct obj2{ 10 };

    TestStruct& objRef = obj1;  // objRef refers to obj1
    test.assertIs(obj1, objRef);  // Should pass (same reference)
    test.assertIs(obj1, obj2);    // Should fail (different objects)
}

void IntClassTest() {
    UnitTest<int>& testInt = UnitTest<int>::getInstance();
    int a = 5;
    int b = 7;
    int c = 5;
    vector<int> testingList = { 1, 2, 3, 5, 7 };

    std::cout << "\n=== Running Unit Tests on int primitive ===\n";

    testInt.assertEqual(a, c); // Pass
    testInt.assertEqual(a, b); // Fail
    //testInt.assertEqual(5, testingList); // Pass
    //testInt.assertEqual(10, testingList); // Fail
    testInt.assertNotEqual(3, a);

    testInt.assertEqual(4, int(4.3));

    std::cout << "\n=== Finishing Unit Tests on int primitive ===\n";

}


void noneTest() {
    UnitTest<int>& testNone = UnitTest<int>::getInstance();
    std::cout << "\n=== Running Unit Tests on int NULL primitive ===\n";

    testNone.assertIsNULL(NULL);
    testNone.assertIsNotNULL(5);
    testNone.assertIsNullptr(nullptr);
    testNone.assertIsNotNullptr(new int(5));

    TestClass myClass = NULL;
    UnitTest<TestClass>& testClassTest = UnitTest<TestClass>::getInstance();

    testClassTest.assertIsNULL(myClass);

    std::cout << "\n=== Finishing Unit Tests on int NULL primitive ===\n";
}

void assertInTest() {
    UnitTest<int>& intTest = UnitTest<int>::getInstance();;
    UnitTest<std::string>& stringTest = UnitTest<std::string>::getInstance();;

    std::cout << "\n===== Testing std::vector<int> =====" << std::endl;
    std::vector<int> vec = { 1, 2, 3, 4, 5 };
    intTest.assertIn(3, vec, "Vector Test");
    intTest.assertIn(10, vec, "Vector Test");

    std::cout << "\n===== Testing std::list<int> =====" << std::endl;
    list<int> lst = { 1, 2, 3, 4, 5 };
    intTest.assertIn(2, lst, "List Test");
    intTest.assertNotIn(10, lst, "List Test");

    std::cout << "\n===== Testing std::deque<int> =====" << std::endl;
    std::deque<int> dq = { 1, 2, 3, 4, 5 };
    intTest.assertIn(4, dq, "Deque Test");
    intTest.assertIn(10, dq, "Deque Test");

    std::cout << "\n===== Testing std::set<int> =====" << std::endl;
    std::set<int> s = { 1, 2, 3, 4, 5 };
    intTest.assertIn(5, s, "Set Test");
    intTest.assertIn(10, s, "Set Test");

    std::cout << "\n===== Testing std::unordered_set<int> =====" << std::endl;
    std::unordered_set<int> us = { 1, 2, 3, 4, 5 };
    intTest.assertIn(1, us, "Unordered Set Test");
    intTest.assertIn(10, us, "Unordered Set Test");

    //std::cout << "\n===== Testing std::map<int, std::string> =====" << std::endl;
    //std::map<int, std::string> mp = { {1, "one"}, {2, "two"}, {3, "three"} };
    //intTest.assertIn(2, mp, "Map Test"); // Checking key existence
    //intTest.assertIn(10, mp, "Map Test");

    //std::cout << "\n===== Testing std::unordered_map<int, std::string> =====" << std::endl;
    //std::unordered_map<int, std::string> ump = { {1, "one"}, {2, "two"}, {3, "three"} };
    //intTest.assertIn(3, ump, "Unordered Map Test");
    //intTest.assertIn(10, ump, "Unordered Map Test");

    //std::cout << "\n===== Testing std::string =====" << std::endl;
    //std::string str = "hello";
    //stringTest.assertIn(std::string(1, 'e'), str, "String Test");
    //stringTest.assertIn(std::string(1, 'z'), str, "String Test");

    //std::cout << "\n===== Testing std::array<int, 5> =====" << std::endl;
    //std::array<int, 5> arr = { 1, 2, 3, 4, 5 };
    //intTest.assertIn(3, arr, "Array Test");
    //intTest.assertIn(10, arr, "Array Test");
}

void testAssertInstance() {
    Base b;
    Derived d;
    Unrelated ur;

    std::cout << "TESTING ASSERTISINSTANCE METHOD" << std::endl;

    UnitTest<Base>& unittestInstance = UnitTest<Base>::getInstance();

    unittestInstance.assertIsNotInstance(b, ur);

}

void printHellow() {
    std::cout << "Hellow" << std::endl;
}

int main()
{
    /*
    OverLoadedClass oInstance(10);

    FunctionHolder holder;

    holder.addFuncToHolder(holder.lambdafy((oInstance.*pAdd), 2));


    holder.execute();
    */
    

    
    TestClass myTest = 5;

    myTest.runTests();
    

    //TestStructTest();
    
    //IntClassTest();
    
    //assertIsTest();

    //noneTest();

    //assertInTest();

    //testAssertInstance();

	return 0;
}
