﻿// CppTestingFramework.cpp : Defines the entry point for the application.
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


struct TestClass {
    int value;

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
};

void TestClassTest() {
    UnitTest<TestClass> test;
    TestClass obj1{ 10 };
    TestClass obj2{ 10 };
    TestClass obj3{ 5 };
    TestClass objFalse{ 0 };  // Should behave as `false` in `assertTrue`
    std::vector<TestClass> testList = { TestClass(1), TestClass(5), TestClass(10) };


    std::cout << "\n=== Running Unit Tests on TestClass ===\n";

    // Equality Tests
    test.assertEqual(obj1, obj2);  // Pass
    test.assertEqual(obj1, obj3);  // Fail
    test.assertEqual(TestClass(5), testList);  //  Pass if TestClass(5) is in `testingList`

    // Inequality Tests
    test.assertNotEqual(obj1, obj3);  //  Pass
    test.assertNotEqual(obj1, obj2);  //  Fail

    // Boolean Tests
    test.assertTrue(obj1);  //  Pass
    test.assertFalse(objFalse); //  Pass
    test.assertFalse(obj1); //  Fail


    std::shared_ptr<TestClass> testObj = std::make_shared<TestClass>(200);
    std::shared_ptr<TestClass> testObj2 = testObj;

    UnitTest<TestClass> testClassTest;
    testClassTest.assertIs(testObj, testObj2);

    std::cout << "\n=== Finishing Unit Tests on TestClass ===\n";
}

void assertIsTest() {
    UnitTest<TestClass> test;
    TestClass obj1{ 10 };
    TestClass obj2{ 10 };

    TestClass& objRef = obj1;  // objRef refers to obj1
    test.assertIs(obj1, objRef);  // Should pass (same reference)
    test.assertIs(obj1, obj2);    // Should fail (different objects)
}

void IntClassTest() {
    UnitTest<int> testInt;
    int a = 5;
    int b = 7;
    int c = 5;
    vector<int> testingList = { 1, 2, 3, 5, 7 };

    std::cout << "\n=== Running Unit Tests on int primitive ===\n";

    testInt.assertEqual(a, c); // Pass
    testInt.assertEqual(a, b); // Fail
    testInt.assertEqual(5, testingList); // Pass
    testInt.assertEqual(10, testingList); // Fail
    testInt.assertNotEqual(3, a);

    testInt.assertEqual(4, int(4.3));

    std::cout << "\n=== Finishing Unit Tests on int primitive ===\n";

}

void noneTest() {
    UnitTest<int> testNone;
    std::cout << "\n=== Running Unit Tests on int NULL primitive ===\n";

    testNone.assertIsNULL(NULL);
    testNone.assertIsNotNULL(5);
    testNone.assertIsNullptr(nullptr);
    testNone.assertIsNotNullptr(new int(5));
    std::cout << "\n=== Finishing Unit Tests on int NULL primitive ===\n";
}

void assertInTest() {
    UnitTest<int> intTest;
    UnitTest<std::string> stringTest;

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

int main()
{
    TestClassTest();
    
    IntClassTest();
    
    assertIsTest();

    noneTest();

    assertInTest();

	return 0;
}
