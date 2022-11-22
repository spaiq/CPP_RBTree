#include <iostream>
#include <sstream>
#include <stack>

template<typename T>
class RedBlackTree {
public:
    struct Node {
        T data;
        Node* parent;
        Node* left;
        Node* right;
        bool color; //0 for red, 1 for black
        unsigned index;

        Node(const T& data, Node* parent = nullptr, unsigned index = 0, bool color = 0, Node* left = nullptr, Node* right = nullptr)
            : data(data), parent(parent), index(index), color(color), left(left), right(right){
        }

        Node(T&& data, Node* parent = nullptr, unsigned index = 0, bool color = 0, Node* left = nullptr, Node* right = nullptr)
            : data(data), parent(parent), index(index), color(color), left(left), right(right){
        }

        ~Node() {
            parent = nullptr;
            left = nullptr;
            right = nullptr;
            color = NULL;
            index = NULL;
        }
    };

private:
    Node* mRoot;
    unsigned mLength;
    unsigned mNewIndex;

    std::string preOrderHelper(Node* node) const {
        std::ostringstream stream;
        if (node == nullptr) return stream.str();
        stream << node->data << " "
            << preOrderHelper(node->left)
            << preOrderHelper(node->right);

        return stream.str();
    }

    std::string inOrderHelper(Node* node) const {
        std::ostringstream stream;
        if (node == nullptr) return stream.str();
        stream << inOrderHelper(node->left)
            << node->data << " "
            << inOrderHelper(node->right);

        return stream.str();
    }

    unsigned treeHeightHelper(Node* node) const {
        if (node == nullptr) return 0;
        int left_height = treeHeightHelper(node->left);
        int right_height = treeHeightHelper(node->right);

        return std::max(left_height, right_height) + 1;
    }

    void appendFix(Node* node) {
        Node* temp;
        while (node->parent->color == 0 && node->parent != mRoot) {
            if (node->parent == node->parent->parent->right) {
                temp = node->parent->parent->left;
                if (temp && temp->color == 0) {
                    temp->color = 1;
                    node->parent->color = 1;
                    node->parent->parent->color = 0;
                    node = node->parent->parent;
                }
                else {
                    if (node == node->parent->left) {
                        node = node->parent;
                        rightRotate(node);
                    }
                    node->parent->color = 1;
                    node->parent->parent->color = 0;
                    leftRotate(node->parent->parent);
                }
            }
            else {
                temp = node->parent->parent->right;

                if (temp && temp->color == 0) {
                    temp->color = 1;
                    node->parent->color = 1;
                    node->parent->parent->color = 0;
                    node = node->parent->parent;
                }
                else {
                    if (node == node->parent->right) {
                        node = node->parent;
                        leftRotate(node);
                    }
                    node->parent->color = 1;
                    node->parent->parent->color = 0;
                    rightRotate(node->parent->parent);
                }
            }
            if (node == mRoot) {
                break;
            }
        }

        mRoot->color = 1;
        return;
    }

    //h)
    void leftRotate(Node* node) {
        Node* temp = node->right;
        node->right = temp->left;
        if (temp->left != nullptr) {
            temp->left->parent = node;
        }
        temp->parent = node->parent;
        if (node->parent == nullptr) {
            this->mRoot = temp;
        }
        else if (node == node->parent->left) {
            node->parent->left = temp;
        }
        else {
            node->parent->right = temp;
        }
        temp->left = node;
        node->parent = temp;
    }

    //i)
    void rightRotate(Node* node) {
        Node* temp = node->left;
        node->left = temp->right;
        if (temp->right != nullptr) {
            temp->right->parent = node;
        }
        temp->parent = node->parent;
        if (node->parent == nullptr) {
            this->mRoot = temp;
        }
        else if (node == node->parent->right) {
            node->parent->right = temp;
        }
        else {
            node->parent->left = temp;
        }
        temp->right = node;
        node->parent = temp;
    }

    std::string printGraphHelper(const std::string& prefix, const Node* node, bool isLeft)
    {
        std::ostringstream stream;
        if (node != nullptr)
        {
            stream << prefix << (isLeft ? "|--" : "L--") << node->data << " ";
            if (node->color == 0) stream << "r\n";
            else stream << "b\n";
            stream << printGraphHelper(prefix + (isLeft ? "|   " : "    "), node->left, true);
            stream << printGraphHelper(prefix + (isLeft ? "|   " : "    "), node->right, false);
            return stream.str();
        }
        return stream.str();
    }

public:
    RedBlackTree() noexcept
        : mRoot(nullptr), mLength(0), mNewIndex(0) {
    }

    ~RedBlackTree() {
        deleteTree();
    }

    Node* getRoot() {
        return mRoot;
    }

    //a)
    template <typename Comp>
    Node* findElement(const T& el, Comp comp_less) const {
        auto temp = mRoot;
        while (!(el == temp->data)) {
            if (comp_less(el, temp->data)) {
                if (!temp->left) return nullptr;
                temp = temp->left;
            }
            else {
                if (!temp->right) return nullptr;
                temp = temp->right;
            }
        }
        return temp;
    }

    //b)
    std::string preOrder() const {
        return preOrderHelper(this->mRoot);
    }

    //c)
    std::string inOrder() const {
        return inOrderHelper(this->mRoot);
    }

    //d)
    void deleteTree() {
        if (mRoot == nullptr) return;
        auto temp = mRoot;
        Node* del;
        while (temp) {
            while (temp->left)
                temp = temp->left;
            if (temp->right)
                temp = temp->right;
            else if (temp == mRoot) break;
            else {
                del = temp;
                temp = temp->parent;
                if (del == temp->left) temp->left = nullptr;
                else temp->right = nullptr;
                delete del;
                del = nullptr;
            }
        }
        delete mRoot;
        mRoot = nullptr;
        mLength = 0;
        return;
    }

    //e)
    unsigned treeHeight() const {
        return treeHeightHelper(this->mRoot);
    }

    //f)
    template <typename Comp>
    void append(const T& data, Comp comp_less) {
        if (!mRoot) {
            mRoot = new Node(data, nullptr, mNewIndex, 1);
            mNewIndex++;
            mLength++;
            return;
        }
        if (data == mRoot->data) {
            mRoot->data = data;
            return;
        }
        Node* new_value;
        auto temp = mRoot;
        while (temp->left || temp->right) {
            if (temp->left)
                if (data == temp->left->data) {
                    temp->left->data = data;
                    return;
                }
            if (temp->right)
                if (data == temp->right->data) {
                    temp->right->data = data;
                    return;
                }
            if (comp_less(data, temp->data)) {
                if (temp->left) {
                    temp = temp->left;
                }
                else {
                    new_value = new Node(data, temp, mNewIndex);
                    temp->left = new_value;
                    mNewIndex++;
                    mLength++;

                    appendFix(new_value);
                    return;
                }
            }
            else {
                if (temp->right) {
                    temp = temp->right;
                }
                else {
                    new_value = new Node(data, temp, mNewIndex);
                    temp->right = new_value;
                    mNewIndex++;
                    mLength++;

                    appendFix(new_value);
                    return;
                }
            }
        }
        if (comp_less(data, temp->data)) {
            new_value = new Node(data, temp, mNewIndex);
            temp->left = new_value;
            mNewIndex++;
            mLength++;
        }
        else {
            new_value = new Node(data, temp, mNewIndex);
            temp->right = new_value;
            mNewIndex++;
            mLength++;
        }

        appendFix(new_value);
        return;
    }

    //g)
    std::string toString() {
        if (mRoot == NULL)
            return "Tree is empty";
        std::stack<Node*> nodeStack;
        nodeStack.push(mRoot);
        std::ostringstream stream;
        stream << "size: " << mLength
            << "\nheight: " << treeHeight()
            << "\n\nindex\tcolor\tparent\tleft\tright\tdata\n";
        if (mLength > 10) {
            for (unsigned i = 0; i < 10; i++) {
                Node* node = nodeStack.top();
                stream << node->index << "\t";
                if (node->color == 0) stream << "red" << "\t";
                else stream << "black" << "\t";

                if (node->parent) stream << node->parent->index << "\t";
                else stream << "NULL" << "\t";

                if (node->left) stream << node->left->index << "\t";
                else stream << "NULL" << "\t";

                if (node->right) stream << node->right->index << "\t";
                else stream << "NULL" << "\t";

                stream << node->data << "\n";
                nodeStack.pop();

                if (node->right)
                    nodeStack.push(node->right);
                if (node->left)
                    nodeStack.push(node->left);
            }
            return stream.str();
        }
        while (nodeStack.empty() == false) {
            Node* node = nodeStack.top();
            stream << node->index << "\t" << node->color << "\t";

            if (node->parent) stream << node->parent->index << "\t";
            else stream << "NULL" << "\t";

            if (node->left) stream << node->left->index << "\t";
            else stream << "NULL" << "\t";

            if (node->right) stream << node->right->index << "\t";
            else stream << "NULL" << "\t";

            stream << node->data << "\n";
            nodeStack.pop();

            if (node->right)
                nodeStack.push(node->right);
            if (node->left)
                nodeStack.push(node->left);
        }
        return stream.str();
    }

    std::string printGraph(){
        return printGraphHelper("", this->mRoot, false);
    }
};

#include <map>
#include <vector>
#include <numeric>
#include <algorithm>
void test()
{
    std::vector<int> v(10);
    std::iota(v.begin(), v.end(), 0);
    std::map<int, int> heights, root_data;
    std::map<int, int>::iterator itr;
    do
    {
        RedBlackTree<int> t;
        for (int i : v)
            t.append(i, std::less<int>{});
        ++heights[t.treeHeight()];
        ++root_data[t.getRoot()->data];

    } while (std::next_permutation(v.begin(), v.end()));

    for (itr = heights.begin(); itr != heights.end(); ++itr) {
        std::cout << '\t' << itr->first << '\t' << itr->second
            << '\n';
    }

    std::cout << "***" << std::endl;
    for (itr = root_data.begin(); itr != root_data.end(); ++itr) {
        std::cout << '\t' << itr->first << '\t' << itr->second
            << '\n';
    }
}

struct SomeClass {
    int some_int;
    bool operator== (const SomeClass& obj) const {
        return some_int == obj.some_int;
    };
};

std::ostream& operator<< (std::ostream& out, const SomeClass& obj) {
    out << obj.some_int;
    return out;
}


int main()
{
    auto comp_less = [](const SomeClass& a, const SomeClass& b) {
        return a.some_int < b.some_int;
    };

    test();
    system("PAUSE");
    return 0;
    
    int random;
    double mstimediff;
    const int MAX_ORDER = 7; // maksymalny rzad wielkosci dodawanych danych
    RedBlackTree<SomeClass>* rbt = new RedBlackTree<SomeClass>(); // stworzenie drzewa
    for (int o = 1; o <= MAX_ORDER; o++)
    {
        const int n = pow(10, o); // rozmiar danych
        // dodawanie do drzewa
        clock_t t1 = clock();
        for (int i = 0; i < n; i++)
        {
            random = rand() % 10001;
            SomeClass so = SomeClass{ random }; // losowe dane
                rbt->append(so, comp_less); // dodanie ( drugi argument to wskaznik na komparator )
        }
        clock_t t2 = clock();
        mstimediff = 1000 * (t2 - t1) / (double)CLOCKS_PER_SEC;
        std::cout << rbt->toString()
            << "\nPomiar 1 (dodawanie), rzedu " << o
            << "\nCzas calkowity: " << mstimediff
            << "ms\nCzas zamortyzowany: " << mstimediff / n << "ms"
            << "\nWysokosc drzewa: " << rbt->treeHeight() << "\n\n"; // wypis na ekran aktualnej postaci drzewa ( skrotowej ) wraz z wysokoscia oraz pomiarow czasowych
        // wyszukiwanie
        const int m = pow(10, 4); // liczba prob wyszukiwania
        int hits = 0; // liczba trafien
        t1 = clock();
        for (int i = 0; i < m; i++)
        {
            random = rand() % 10001;
            SomeClass *so = new SomeClass{ random }; // losowe dane jako wzorzec do wyszukiwania ( obiekt chwilowy )
            if (rbt->findElement(*so, comp_less) != NULL)
                hits++;
            delete so;
        }
        t2 = clock();
        std::cout << "\nPomiar 2 (dodawanie), rzedu " << o
            << "\nCzas calkowity: " << mstimediff
            << "ms\nCzas zamortyzowany: " << mstimediff / n << "ms"
            << "\nTrafienia: " << hits << "\n\n"; // wypis na ekran pomiarow czasowych i liczby trafien
            rbt->deleteTree(); // czyszczenie drzewa wraz z uwalnianiem pamieci danych
    }
    delete rbt;
    return 0;


    RedBlackTree<SomeClass>* tree = new RedBlackTree<SomeClass>;

    tree->append(SomeClass{ 10 }, comp_less);
    tree->append(SomeClass{ 2 }, comp_less);
    tree->append(SomeClass{ 32 }, comp_less);
    tree->append(SomeClass{ 41 }, comp_less);
    tree->append(SomeClass{ 13 }, comp_less);
    tree->append(SomeClass{ 7 }, comp_less);
    tree->append(SomeClass{ 5 }, comp_less);
    tree->append(SomeClass{ 2 }, comp_less);
    tree->append(SomeClass{ 32 }, comp_less);
    tree->append(SomeClass{ 13 }, comp_less);
    tree->append(SomeClass{ 41 }, comp_less);
    for (int i = 0; i < 20; i++)
        tree->append(SomeClass{ i }, comp_less);

    std::cout << tree->preOrder() << "\n";
    std::cout << tree->inOrder() << "\n";
    std::cout << tree->treeHeight() << "\n";
    std::cout << "\n" << tree->printGraph() << "\n";
    std::cout << tree->toString() << "\n";

    tree->deleteTree();
    std::cout << tree->inOrder() << "\n";
    delete tree;

    return 0;
}