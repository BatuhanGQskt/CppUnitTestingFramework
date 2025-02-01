#pragma once
#include<vector>
#include<concepts>
#include<iostream>
#include <type_traits>

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
    void printResult(bool passed, const T& testObject, const T& trueObject, const std::string& functionName = "");

public:
    bool assertEqual(const T& a, const T& b) requires EqualityComparable<T>;
    bool assertEqual(const T& obj, const std::vector<T>& list) requires EqualityComparable<T>;

    bool assertNotEqual(const T& a, const T& b) requires EqualityUncomparable<T>;
    bool assertNotEqual(const T& obj, const std::vector<T>& list) requires EqualityUncomparable<T>;

    bool assertTrue(const T& a) requires BooleanConvertible<T>;
    bool assertFalse(const T& a) requires BooleanConvertible<T>;

    bool assertIs(const T& a, const T& b, const std::string& functionName = "");
    bool assertIsNot(const T& a, const T& b, const std::string& functionName = ""); 
    bool assertIs(const std::shared_ptr<T>& a, const std::shared_ptr<T>& b, const std::string& functionName = "");
    bool assertIsNot(const std::shared_ptr<T>& a, const std::shared_ptr<T>& b, const std::string& functionName = "");

    bool assertIsNULL(const T& a, const std::string& functionName = "");
    bool assertIsNotNULL(const T& a, const std::string& functionName = "");

    bool assertIsNullptr(const T* a, const std::string& functionName = "");
    bool assertIsNotNullptr(const T* a, const std::string& functionName = "");

    template <HasConstIterator Container>
    bool assertIn(const T& a, const Container& c, const std::string& functionName = "");
    template <HasConstIterator Container>
    bool assertNotIn(const T& a, const Container& c, const std::string& functionName = "");
};


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

template <typename T>
bool UnitTest<T>::assertEqual(const T& testObject, const std::vector<T>& list) requires EqualityComparable<T> {
    for (const auto& item : list) {
        if (testObject == item) {
            printResult(true, testObject, item, "assertEqual");
            return true;
        }
    }
    printResult(false, testObject, T{});
    return false;
}

template<typename T>
bool UnitTest<T>::assertNotEqual(const T& testObject, const T& trueObject) requires EqualityUncomparable<T> {
    bool result = (testObject != trueObject);
    printResult(result, testObject, trueObject, "assertNotEqual");
    return result;
}

template <typename T>
bool UnitTest<T>::assertNotEqual(const T& testObject, const std::vector<T>& list) requires EqualityUncomparable<T> {
    for (const auto& item : list) {
        if (testObject == item) {
            printResult(false, testObject, item, "assertNotEqual");
            return false;
        }
    }
    printResult(true, testObject, T{});
    return true;
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

// 📌 `assertIs`: Checks if `a` and `b` refer to the same shared object
template <typename T>
bool UnitTest<T>::assertIs(const std::shared_ptr<T>& testObject, const std::shared_ptr<T>& trueObject, const std::string& functionName) {
    bool result = (testObject.get() == trueObject.get());  // Compare stored raw pointers
    printResult(result, *testObject, *trueObject, "assertIs");
    return result;
}

// 📌 `assertIsNot`: Checks if `a` and `b` refer to different shared objects
template <typename T>
bool UnitTest<T>::assertIsNot(const std::shared_ptr<T>& testObject, const std::shared_ptr<T>& trueObject, const std::string& functionName) {
    bool result = (testObject.get() != trueObject.get());  // Compare stored raw pointers
    printResult(result, *testObject, *trueObject, "assertIsNot");
    return result;
}


template <typename T>
bool UnitTest<T>::assertIsNULL(const T& testObject, const std::string& functionName) {
    bool result = (testObject == NULL);
    printResult(result, testObject, T{}, functionName.empty() ? "assertIsNULL" : functionName);
    return result;
}

template <typename T>
bool UnitTest<T>::assertIsNotNULL(const T& testObject, const std::string& functionName) {
    bool result = not (testObject == NULL);
    printResult(result, testObject, T{}, functionName.empty() ? "assertIsNotNULL" : functionName);
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
