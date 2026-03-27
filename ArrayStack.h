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
        // TODO
    }

    void pop() {
        // TODO
    }

    T top() const {
        // TODO
        return T();
    }

    bool empty() const {
        // TODO
        return true;
    }

    int size() const {
        // TODO
        return 0;
    }
};

#endif
