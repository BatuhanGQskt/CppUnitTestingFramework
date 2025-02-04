#pragma once
#include<vector>
#include<concepts>
#include<iostream>
#include <type_traits>
#include<functional>
#include "../FunctionWrapper/FunctionWrapper.h"

// Concept definition for checking if T has operator==
template <typename U>
concept EqualityComparable = requires(U a, U b) {
    { a == b } -> std::convertible_to<bool>;
};

// Concept definition for checking if T has operator!=
template <typename U>
concept EqualityUncomparable = requires(U a, U b) {
    { a != b } -> std::convertible_to<bool>;
};

// Concept definition for checking if T is castable to boolean
template <typename U>
concept BooleanConvertible = requires(U a) {
    { static_cast<bool>(a) } -> std::convertible_to<bool>;
};

template <typename U>
concept HasConstIterator = requires { typename U::const_iterator; };


template <typename T>
class UnitTest {
private:
    static UnitTest<T>* instance;
    static int instanceCounter;

public: //change to private when solved storing assertion function fully required
    std::vector<std::function<bool()>> assertions;

private:
    // Private copy constructor and assignment operator to prevent copying
    UnitTest(const UnitTest<T>&) = delete;
    UnitTest<T>& operator=(const UnitTest<T>&) = delete;

protected:
    UnitTest() {

    }
    
    void printResult(bool passed, const T& testObject, const T& trueObject, const std::string& functionName = "");

public:
    void runTests();
    static UnitTest<T>& getInstance();
    
    // Add assertion to the list (wrap it as a lambda)
    template <typename Func, typename... Args>
    void addAssertion(Func func,  Args... args);

    bool assertEqual(const T& a, const T& b) requires EqualityComparable<T>;

    bool assertNotEqual(const T& a, const T& b) requires EqualityUncomparable<T>;

    bool assertTrue(const T& a) requires BooleanConvertible<T>;
    bool assertFalse(const T& a) requires BooleanConvertible<T>;

    bool assertIs(const T& a, const T& b, const std::string& functionName = "");
    bool assertIsNot(const T& a, const T& b, const std::string& functionName = "");

    bool assertIsNULL(const T& a, const std::string& functionName = "");
    bool assertIsNotNULL(const T& a, const std::string& functionName = "");

    bool assertIsNullptr(const T* a, const std::string& functionName = "");
    bool assertIsNotNullptr(const T* a, const std::string& functionName = "");

    template <HasConstIterator Container>
    bool assertIn(const T& a, const Container& c, const std::string& functionName = "");

    template <HasConstIterator Container>
    bool assertNotIn(const T& a, const Container& c, const std::string& functionName = "");

    template <typename U>
    bool assertIsInstance(const T& a, const U& b=NULL, const std::string& functionName="");
    template <typename U>
    bool assertIsNotInstance(const T& a, const U& b = NULL, const std::string& functionName = "");
};

// Initialize static members
template <typename T>
UnitTest<T>* UnitTest<T>::instance = nullptr;
template <typename T>
int UnitTest<T>::instanceCounter = 0;


template <typename T>
UnitTest<T>& UnitTest<T>::getInstance() {
    static UnitTest<T> instance;
    UnitTest<T>::instanceCounter++;

    std::string classType = typeid(T).name();
    std::cout << "Current instance of the type " << classType << ": instances are " << UnitTest<T>::instanceCounter << std::endl;
    return instance;
}



// Run all stored assertions
template <typename T>
void UnitTest<T>::runTests() {
    for (const auto& assertion : this->assertions) {
        assertion();  // Call each stored assertion
    }
}

template <typename T>
template <typename Func, typename... Args>
void UnitTest<T>::addAssertion(Func func, Args... args) {
    this->assertions.push_back([=]() -> bool {
        if constexpr (std::is_member_function_pointer_v<Func>) {
            return (this->*func)((*args)...);
        }
        else {
            // If func is not a member function pointer, you might call it directly.
            // (Assuming it takes Args... as parameters.)
            return func(args...);
        }
        });
}


template<typename T>
void UnitTest<T>::printResult(bool passed, const T& testObject, const T& trueObject, const std::string& functionName) {
    constexpr bool isStreamable = requires(std::ostream & os, const T & obj) { os << obj; };

    std::string message = (passed ? "[PASS] " : "[FAIL] ");
    std::string className = typeid(T).name();  // Get class name dynamically

    message += "[" + className+ "::" + functionName + "] ";

    if constexpr (isStreamable) {
        std::cout << message << testObject << " "
            << (passed ? "==" : "!=")
            << " " << trueObject << std::endl;
    }
    else {
        std::cout << message << (passed ? "Objects are equal." : "Objects are not equal.") << std::endl;
    }
}

template<typename T>
bool UnitTest<T>::assertEqual(const T& testObject, const T& trueObject) requires EqualityComparable<T> {
    bool result = (testObject == trueObject);
    printResult(result, testObject, trueObject, "assertEqual");
    return result;
}

template<typename T>
bool UnitTest<T>::assertNotEqual(const T& testObject, const T& trueObject) requires EqualityUncomparable<T> {
    bool result = (testObject != trueObject);
    printResult(result, testObject, trueObject, "assertNotEqual");
    return result;
}


template <typename T>
bool UnitTest<T>::assertTrue(const T& testObject) requires BooleanConvertible<T> {
    bool result = static_cast<bool>(testObject);
    std::cout << (result ? "[PASS] Value is true." : "[FAIL] Value is false.") << std::endl;
    return result;
}

template <typename T>
bool UnitTest<T>::assertFalse(const T& testObject) requires BooleanConvertible<T> {
    bool result = not static_cast<bool>(testObject);
    std::cout << (result ? "[PASS] Value is false." : "[FAIL] Value is true.") << std::endl;
    return result;
}

// 📌 `assertIs`: Checks if `a` and `b` refer to the same object in memory
template <typename T>
bool UnitTest<T>::assertIs(const T& testObject, const T& trueObject, const std::string& functionName) {
    bool result = (&testObject == &trueObject);
    printResult(result, testObject, trueObject, "assertIs");
    return result;
}

// 📌 `assertIsNot`: Checks if `a` and `b` refer to different objects in memory
template <typename T>
bool UnitTest<T>::assertIsNot(const T& testObject, const T& trueObject, const std::string& functionName) {
    bool result = (&testObject != &trueObject);
    printResult(result, testObject, trueObject, "assertIsNot");
    return result;
}

template <typename T>
bool UnitTest<T>::assertIsNULL(const T& testObject, const std::string& functionName) {
    bool result = (testObject == NULL);
    printResult(result, testObject, NULL, functionName.empty() ? "assertIsNULL" : functionName);
    return result;
}

template <typename T>
bool UnitTest<T>::assertIsNotNULL(const T& testObject, const std::string& functionName) {
    bool result = not (testObject == NULL);
    printResult(result, testObject, NULL, functionName.empty() ? "assertIsNotNULL" : functionName);
    return result;
}

template <typename T>
bool UnitTest<T>::assertIsNullptr(const T* testObject, const std::string& functionName) {
    bool result = (testObject == nullptr);
    printResult(result, T{}, T{}, functionName.empty() ? "assertIsNullptr" : functionName);
    return result;
}

template <typename T>
bool UnitTest<T>::assertIsNotNullptr(const T* testObject, const std::string& functionName) {
    bool result = not (testObject == nullptr);
    printResult(result, T{}, T{}, functionName.empty() ? "assertIsNotNullptr" : functionName);
    return result;
}

template <typename T>
template <HasConstIterator Container>
bool UnitTest<T>::assertIn(const T& testObject, const Container& c, const std::string& functionName) {
    for (const auto& item : c) {
        if (item == testObject) {
            printResult(true, T{}, T{}, functionName.empty() ? "assertIn" : functionName);
            return true;
        }
    }
    printResult(false, T{}, T{}, functionName.empty() ? "assertIn" : functionName);
    return false;
}


template <typename T>
template <HasConstIterator Container>
bool UnitTest<T>::assertNotIn(const T& testObject, const Container& c, const std::string& functionName) {
    for (const auto& item : c) {
        if (item == testObject) {
            printResult(false, T{}, T{}, functionName.empty() ? "assertIn" : functionName);
            return false;
        }
    }
    printResult(true, T{}, T{}, functionName.empty() ? "assertIn" : functionName);
    return true;
}


template <typename T>
template <typename U>
bool UnitTest<T>::assertIsInstance(const T& testObject, const U& b, const std::string& functionName) {
    bool result = std::is_base_of<T, U>::value;
    printResult(result, testObject, testObject, functionName.empty() ? "assertIsInstance" : functionName);
    return result;
}

template <typename T>
template <typename U>
bool UnitTest<T>::assertIsNotInstance(const T& testObject, const U& b, const std::string& functionName) {
    bool result = not std::is_base_of<T, U>::value;
    printResult(result, testObject, testObject, functionName.empty() ? "assertIsNotInstance" : functionName);
    return result;
}