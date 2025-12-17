#pragma once
#include <stdexcept>

template <typename T>
class OneList
{
private:
    struct Node
    {
        T data;
        Node* next;
        Node(const T& d, Node* n = nullptr) : data(d), next(n) {}
    };

    Node* head;
    Node* tail;
    int count;

public:
    class Iterator
    {
        friend class OneList;
        OneList* owner;
        Node* cur;

        Iterator(OneList* o, Node* c) : owner(o), cur(c) {}

    public:
        Iterator() : owner(nullptr), cur(nullptr) {}

        T& operator*()
        {
            if (!cur) throw std::out_of_range("Iterator dereference");
            return cur->data;
        }

        bool operator!=(const Iterator& other) const
        {
            return cur != other.cur;
        }

        Iterator& operator++()
        {
            if (cur) cur = cur->next;
            return *this;
        }

        Iterator& operator--()
        {
            if (!owner) throw std::runtime_error("Iterator owner is null");

            if (cur == nullptr) // end() -> last
            {
                cur = owner->tail;
                return *this;
            }

            if (cur == owner->head) // begin -> end()
            {
                cur = nullptr;
                return *this;
            }

            Node* p = owner->head;
            while (p && p->next != cur)
                p = p->next;

            cur = p;
            return *this;
        }
    };

    OneList() : head(nullptr), tail(nullptr), count(0) {}

    OneList(const OneList& other) : head(nullptr), tail(nullptr), count(0)
    {
        Node* p = other.head;
        while (p)
        {
            push_back(p->data);
            p = p->next;
        }
    }

    ~OneList()
    {
        clear();
    }

    bool empty() const { return count == 0; }
    int size() const { return count; }

    T& front()
    {
        if (empty()) throw std::out_of_range("front on empty");
        return head->data;
    }

    T& back()
    {
        if (empty()) throw std::out_of_range("back on empty");
        return tail->data;
    }

    T& operator[](int index)
    {
        if (index < 0 || index >= count)
            throw std::out_of_range("index");

        Node* p = head;
        for (int i = 0; i < index; ++i)
            p = p->next;

        return p->data;
    }

    void push_front(const T& value)
    {
        Node* n = new Node(value, head);
        head = n;
        if (!tail) tail = head;
        ++count;
    }

    void push_back(const T& value)
    {
        Node* n = new Node(value);
        if (!tail)
            head = tail = n;
        else
        {
            tail->next = n;
            tail = n;
        }
        ++count;
    }

    void insert(int pos, const T& value)
    {
        if (pos < 0 || pos > count)
            throw std::out_of_range("insert");

        if (pos == 0) { push_front(value); return; }
        if (pos == count) { push_back(value); return; }

        Node* p = head;
        for (int i = 0; i < pos - 1; ++i)
            p = p->next;

        p->next = new Node(value, p->next);
        ++count;
    }

    void pop_front()
    {
        if (empty()) throw std::out_of_range("pop_front");

        Node* old = head;
        head = head->next;
        delete old;
        --count;

        if (!head) tail = nullptr;
    }

    void pop_back()
    {
        if (empty()) throw std::out_of_range("pop_back");

        if (head == tail)
        {
            delete head;
            head = tail = nullptr;
            count = 0;
            return;
        }

        Node* p = head;
        while (p->next != tail)
            p = p->next;

        delete tail;
        tail = p;
        tail->next = nullptr;
        --count;
    }

    void remove(const T& value)
    {
        while (head && head->data == value)
            pop_front();

        if (!head) return;

        Node* prev = head;
        Node* cur = head->next;

        while (cur)
        {
            if (cur->data == value)
            {
                prev->next = cur->next;
                if (cur == tail) tail = prev;
                delete cur;
                cur = prev->next;
                --count;
            }
            else
            {
                prev = cur;
                cur = cur->next;
            }
        }
    }

    void clear()
    {
        while (!empty())
            pop_front();
    }

    void sort_exchange()
    {
        if (count < 2) return;

        bool swapped;
        do
        {
            swapped = false;
            Node* cur = head;
            while (cur && cur->next)
            {
                if (cur->data > cur->next->data)
                {
                    T tmp = cur->data;
                    cur->data = cur->next->data;
                    cur->next->data = tmp;
                    swapped = true;
                }
                cur = cur->next;
            }
        } while (swapped);
    }

    void sort_selection()
    {
        if (count < 2) return;

        for (Node* i = head; i != nullptr; i = i->next)
        {
            Node* minNode = i;
            for (Node* j = i->next; j != nullptr; j = j->next)
            {
                if (j->data < minNode->data)
                    minNode = j;
            }
            if (minNode != i)
            {
                T tmp = i->data;
                i->data = minNode->data;
                minNode->data = tmp;
            }
        }
    }

    Iterator begin() { return Iterator(this, head); }
    Iterator end() { return Iterator(this, nullptr); }
};
