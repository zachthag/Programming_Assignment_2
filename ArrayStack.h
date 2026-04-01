#ifndef ARRAYSTACK_H
#define ARRAYSTACK_H

#include <vector>
#include <stdexcept>

template <typename T>
class ArrayStack {
private:
    std::vector<T> data;

public:
    void push(const T& value) {
        data.push_back(value);
    }

    void pop() {
        if (data.empty()) {
            throw std::out_of_range("pop from empty stack");
        }
        data.pop_back();
    }

    T top() const {
        if (data.empty()) {
            throw std::out_of_range("top from empty stack");
        }
        return data.back();
    }

    bool empty() const {
        return data.size() == 0;
    }

    int size() const {
        return static_cast<int>(data.size());
    }
};

#endif
