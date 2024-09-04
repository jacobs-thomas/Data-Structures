#ifndef JACOBS_GENERIC_LINKED_LIST
#define JACOBS_GENERIC_LINKED_LIST

#include <vector>
#include <iostream>
#include <assert.h>

namespace generic {

    template<typename Type>
    class List {

    private:
        // Structures:
        struct Node {
            /**
            * @brief Structure representing a node in the doubly linked list.
            */

        public:
            // Instance Attributes:
            Node* left = nullptr;
            Node* right = nullptr;
            Type value = Type();

            // Constructor:
            Node(Type p_value, Node* p_left = nullptr, Node* p_right = nullptr)
                : left(p_left), right(p_right), value(p_value) {}
        };

        class Iterator {
            /**
            * @brief Iterator for traversing the list.
            */

        private:
            Node* node;
        public:
            Iterator(Node* n) : node(n) {}

            Type& operator*() { return node->value; }
            Iterator& operator++() { node = node->right; return *this; }
            bool operator!=(const Iterator& other) const { return node != other.node; }
        };

        // Constants:
        static const unsigned int DEFAULT_SIZE = 0;

        // Instance Attributes:
        Node* root = nullptr;
        Node* back = nullptr;
        unsigned int size = DEFAULT_SIZE;

    public:
        // Constructors:
        List() { }

        List(const std::vector<Type>& elements) {
            for (const auto& element : elements) {
                push_back(element);
            }
        }

        List(List&& other) noexcept
            : root(other.root), back(other.back), size(other.size) {
            other.root = nullptr;
            other.back = nullptr;
            other.size = DEFAULT_SIZE;
        }

        // Methods:
        void push_back(const Type& value) {
            /**
            * @brief Adds a new element to the end of the list.
            * @param value Value to be added to the list.
            */


            // If the list is empty, set the root node.
            if (back == nullptr) {
                root = back = new Node(value);
                ++size;
                return;
            }

            // Otherwise, add the new node to the right of the current node.
            back->right = new Node(value, back);
            back = back->right;
            ++size;
        }

        void insert_at(const Type& value, const unsigned int index) {
            /**
            * @brief Inserts a new element at a specified position.
            * @param value Value to be inserted.
            * @param index Position at which to insert the element.
            */


            assert(index <= size && "Index is out of bounds");

            // Handle insertion at the beginning of the list
            if (index == 0) {
                Node* new_node = new Node(value, nullptr, root);
                if (root != nullptr) {
                    root->left = new_node;
                }
                root = new_node;
                ++size;
                return;
            }

            // Handle insertion at the end of the list
            if (index == size) {
                push_back(value); // Reuse the push_back method
                return;
            }

            // Handle insertion in the middle of the list
            Node* node = root;
            for (unsigned int i = 0; i < index - 1; ++i) {
                node = node->right;
            }

            Node* new_node = new Node(value, node, node->right);
            if (node->right != nullptr) {
                node->right->left = new_node;
            }
            node->right = new_node;

            ++size;
        }

        unsigned int length() const {
            /**
            * @brief Returns the number of elements in the list.
            * @return Number of elements in the list.
            */

            return size;
        }

        Type pop_back() {
            /**
            * @brief Removes and returns the last element of the list.
            * @return The value of the removed element.
            */


            assert(size > 0 && "Cannot pop from an empty list");

            Type value = back->value;
            Node* temp = back;
            back = back->left;

            if (back != nullptr) {
                back->right = nullptr;
            }
            else {
                root = nullptr;
            }

            delete temp;
            --size;

            return value;
        }

        Iterator begin() { return Iterator(root); }

        Iterator end() { return Iterator(nullptr); }


        // Operators:
        const Type& operator[](const unsigned int index) const {
            /**
            * @brief Accesses the element at a specified index in the list.
            *
            * This method allows direct access to list elements using the index operator.
            * It performs a linear search from the beginning of the list to the specified
            * index and returns a reference to the element at that index.
            *
            * @param index The zero-based index of the element to access.
            * @return A reference to the element at the specified index.
            * */


            assert(index < size && "Index is out of bounds");

            Node* node = root;
            for (unsigned int i = 0; i < index; ++i) {
                node = node->right;
            }

            return node->value;
        }

        List& operator=(const List& other) {
            /**
            * @brief Assignment operator for copying another list.
            * @param other List to copy from.
            * @return Reference to this list.
            */


            if (this != &other) {
                // Clear the current list
                while (size > 0) {
                    pop_back();
                }

                // Copy the other list
                Node* current = other.root;
                while (current != nullptr) {
                    push_back(current->value);
                    current = current->right;
                }
            }

            return *this;
        }

        // Destructor:
        ~List() {
            /**
            * @brief Destructor that frees allocated memory.
            */


            while (root != nullptr) {
                Node* temp = root;
                root = root->right;
                delete temp;
            }
        }
    };
}

#endif // !JACOBS_GENERIC_LINKED_LIST