// A structure to store functions and their arguments
template <typename... Args>
struct FunctionWrapper {
    std::function<void(Args...)> func;
    std::tuple<Args...> arguments;

    // Constructor to store the function and its arguments
    FunctionWrapper(std::function<void(Args...)> f, Args... args)
        : func(f), arguments(std::make_tuple(args...)) {
    }

    // Call the stored function with the stored arguments
    void call() {
        callHelper(std::index_sequence_for<Args...>{});
    }

private:
    // Helper function to unpack the arguments tuple and call the function
    template <std::size_t... I>
    void callHelper(std::index_sequence<I...>) {
        func(std::get<I>(arguments)...);
    }
};
