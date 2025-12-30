#include "mystack.h"

Mystack::Mystack() {
    top = -1;
}

bool Mystack::empty() {
    return (top == -1);
}

bool Mystack::full() {
    return top == 99;
}

int Mystack::size() {
    return top+1;
}

bool Mystack::push(string p) {
    if(top+1 != 100) {
        data[++top] = p;
        return true;
    }
    return false;
}
bool Mystack::pop() {
    if(empty()) {
        return false;
    }
    top--;
    return true;
}
string Mystack::getTop() {
    if(empty()) {
        return "";
    }
    else {
        return data[top];
    }
}
