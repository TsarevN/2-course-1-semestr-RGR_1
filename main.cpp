#include <iostream>

using namespace std;

struct Node
{
    int data1;
    int data2;
    Node* next;
};

class BoolMatrix
{
private:
    Node* head;
    int maxR;
    int maxC;
public:
    BoolMatrix()
    {
        head = NULL;
        maxR = 0;
        maxC = 0;
    }
    BoolMatrix(const BoolMatrix& other)
    {
        head = NULL;
        Node* otherTemp = other.head;
        while (otherTemp != NULL)
        {
            add(otherTemp->data1, otherTemp->data2);
            otherTemp = otherTemp->next;
        }
        maxR = other.maxR;
        maxC = other.maxC;
    }
    ~BoolMatrix()
    {
        Node* temp = head;
        while (temp != NULL)
        {
            Node* next = temp->next;
            delete temp;
            temp = next;
        }
        head = NULL;
    }
    void add(int value1, int value2) {
        Node* node = new Node;
        node->data1 = value1;
        node->data2 = value2;
        node->next = NULL;

        if (head == NULL)
            head = node;
        else
        {
            Node* temp = head;
            while (temp->next != NULL)
                temp = temp->next;
            temp->next = node;
        }
    }
    bool get(int row, int col) const
    {
        Node* temp = head;
        while (temp != NULL)
        {
            if (temp->data1 == row && temp->data2 == col)
                return true;
            temp = temp->next;
        }
        return false;
    }
    BoolMatrix inversion() {

        int maxRow = -1;
        int maxCol = -1;
        Node* node = head;
        while (node != NULL) {
            if (node->data1 > maxRow)
                maxRow = node->data1;
            if (node->data2 > maxCol)
                maxCol = node->data2;
            node = node->next;
        }
        maxRow = maxR;
        maxCol = maxC;
        maxRow--;
        maxCol--;


        BoolMatrix invertedMatrix;
        invertedMatrix.maxR = maxRow;
        invertedMatrix.maxC = maxCol;


        for (int row = 0; row <= maxRow; row++) {
            for (int col = 0; col <= maxCol; col++) {
                bool isOne = false;


                Node* node = head;
                while (node != NULL) {
                    if (node->data1 == row && node->data2 == col) {
                        isOne = true;
                        break;
                    }
                    node = node->next;
                }


                if (isOne)
                    continue;
                else
                    invertedMatrix.add(row, col);
            }
        }
        invertedMatrix.maxR++;
        invertedMatrix.maxC++;
        *this = invertedMatrix;


        return *this;
    }
    BoolMatrix submatrix(int rowStart, int rowEnd, int colStart, int colEnd)
    {
        BoolMatrix result;
        Node* temp = head;
        while (temp != NULL)
        {
            if (temp->data1 >= rowStart && temp->data1 <= rowEnd && temp->data2 >= colStart && temp->data2 <= colEnd)
                result.add(temp->data1 - rowStart, temp->data2 - colStart);
            temp = temp->next;
        }
        *this = result;
        maxC = colEnd - colStart;
        maxR = rowEnd - rowStart;
        maxR++;
        maxC++;
        return *this;
    }
    BoolMatrix transpose()
    {
        BoolMatrix transposedMatrix;
        Node* temp = head;
        while (temp != NULL)
        {
            transposedMatrix.add(temp->data2, temp->data1);
            temp = temp->next;
        }
        transposedMatrix.maxC = maxC;
        transposedMatrix.maxR = maxR;
        *this = transposedMatrix;
        maxR = transposedMatrix.maxC;
        maxC = transposedMatrix.maxC;
        return *this;
    }
    BoolMatrix iteration()
    {
        BoolMatrix temp = *this;
        int maxr = maxR;
        int maxc = maxC;
        BoolMatrix res;
        res.maxR = maxR;
        res.maxC = maxC;

        BoolMatrix one;
        one.maxR = maxR;
        one.maxC = maxC;
        for (size_t i = 0; i < maxr; i++)
        {
            for (size_t j = 0; j < maxr; j++)
            {
                if (i == j)
                    one.add(i, j);

            }
        }
        BoolMatrix a;
        a.maxC = maxC;
        a.maxR = maxR;
        res = one | temp;
        while (res != a)
        {
            temp = temp * temp;
            a = res;
            res = res | temp;

        }
        maxR = res.maxR;
        maxC = res.maxC;
        *this = res;

        return *this;
    }

    BoolMatrix& operator=(const BoolMatrix& other)
    {
        if (this == &other)
            return *this;



        Node* temp = head;
        while (temp != NULL)
        {
            Node* next = temp->next;
            delete temp;
            temp = next;
        }
        head = NULL;


        Node* otherTemp = other.head;
        while (otherTemp != NULL)
        {
            add(otherTemp->data1, otherTemp->data2);
            otherTemp = otherTemp->next;
        }
        maxR = other.maxR;
        maxC = other.maxC;


        return *this;
    }
    BoolMatrix operator&(const BoolMatrix& other) const
    {
        BoolMatrix result;
        Node* temp1 = head;
        Node* temp2 = other.head;

        while (temp1 != NULL && temp2 != NULL)
        {
            if (temp1->data1 == temp2->data1 && temp1->data2 == temp2->data2)
            {
                result.add(temp1->data1, temp1->data2);
                temp1 = temp1->next;
                temp2 = temp2->next;
            }
            else if (temp1->data1 < temp2->data1 || (temp1->data1 == temp2->data1 && temp1->data2 < temp2->data2))
                temp1 = temp1->next;

            else
                temp2 = temp2->next;

        }
        result.maxC = maxC;
        result.maxR = maxR;
        return result;
    }
    BoolMatrix operator|(const BoolMatrix& other) const {
        BoolMatrix result(*this);

        Node* temp = other.head;
        while (temp != NULL) {
            bool found = false;
            Node* resultTemp = result.head;
            while (resultTemp != NULL) {
                if (resultTemp->data1 == temp->data1 && resultTemp->data2 == temp->data2) {
                    found = true;
                    break;
                }
                resultTemp = resultTemp->next;
            }
            if (!found)
                result.add(temp->data1, temp->data2);

            temp = temp->next;
        }

        result.maxC = maxC;
        result.maxR = maxR;
        return result;
    }
    BoolMatrix operator*(const BoolMatrix& other)
    {
        BoolMatrix res;
        Node* temp = head;
        Node* temp2 = other.head;
        int rows = 0, cols = 0;
        int rows2 = 0, cols2 = 0;

        while (temp != NULL)
        {
            rows = max(rows, temp->data1);
            cols = max(cols, temp->data2);

            temp = temp->next;
        }
        rows++;
        cols++;
        while (temp2 != NULL)
        {
            rows2 = max(rows2, temp2->data1);
            cols2 = max(cols2, temp2->data2);

            temp2 = temp2->next;
        }
        rows2++;
        cols2++;
        rows = maxR;
        cols = maxC;
        rows2 = other.maxR;
        cols2 = other.maxC;
        int** arr = new int* [rows];
        for (int i = 0; i < rows; i++)
        {
            arr[i] = new int[cols];
            for (int j = 0; j < cols; j++)
                arr[i][j] = 0;
        }

        int** arr2 = new int* [rows2];
        for (int i = 0; i < rows2; i++)
        {
            arr2[i] = new int[cols2];
            for (int j = 0; j < cols2; j++)
                arr2[i][j] = 0;

        }
        temp = head;
        while (temp != NULL)
        {
            arr[temp->data1][temp->data2] = 1;
            temp = temp->next;
        }
        temp2 = other.head;
        while (temp2 != NULL)
        {
            arr2[temp2->data1][temp2->data2] = 1;
            temp2 = temp2->next;
        }

        if (cols != rows2)
        {
            cout << "error cols != rows2" << endl;
            exit(0);
        }
        int** result = new int* [rows];
        for (int i = 0; i < rows; i++)
        {
            result[i] = new int[cols];
        }
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols2; j++)
            {
                result[i][j] = false;
                for (int k = 0; k < cols; k++)
                    result[i][j] = result[i][j] || (arr[i][k] && arr2[k][j]);

            }
        }

        for (size_t i = 0; i < rows; i++)
        {
            for (size_t j = 0; j < rows; j++)
            {
                if (result[i][j] == 1)
                    res.add(i, j);

            }
        }

        for (int i = 0; i < rows; i++)
            delete[] arr[i];
        delete[] arr;

        for (int i = 0; i < rows2; i++)
            delete[] arr2[i];

        delete[] arr2;

        res.maxC = maxC;
        res.maxR = maxR;
        return res;
    }
    bool operator==(const BoolMatrix& b) {
        Node* tempA = head;
        Node* tempB = b.head;
        while (tempA != NULL && tempB != NULL) {
            if (tempA->data1 != tempB->data1 || tempA->data2 != tempB->data2)
                return false;
            tempA = tempA->next;
            tempB = tempB->next;
        }
        return tempA == NULL && tempB == NULL;
    }
    bool operator!=(const BoolMatrix& b) {
        return !(*this == b);
    }

    friend istream& operator>>(istream& is, BoolMatrix& matrix)
    {

        Node* temp = matrix.head;
        while (temp != NULL)
        {
            Node* next = temp->next;
            delete temp;
            temp = next;
        }
        matrix.head = NULL;


        char ch;
        is >> ch;
        if (ch != '[')
        {
            cout << "error input";
            exit(0);
        }

        int row = 0, col = 0;
        while (true)
        {
            is >> ch;
            if (ch == ';')
            {
                row++;
                col = 0;
            }
            else if (ch == ']')
            {
                break;
            }
            else if (ch == ' ')
            {

            }
            else if (ch == '0')
            {
                col++;
            }
            else if (ch == '1')
            {
                matrix.add(row, col);
                col++;
            }
            else
            {
                cout << "error input";
                exit(0);
            }
        }
        row++;
        matrix.maxR = row;
        matrix.maxC = col;
        return is;
    }
    friend ostream& operator<<(ostream& os, const BoolMatrix& matrix)
    {
        if (matrix.head == NULL)
        {
            os << "[]";
            return os;
        }


        int maxRow = matrix.head->data1;
        int maxCol = matrix.head->data2;
        Node* temp = matrix.head;
        while (temp != NULL)
        {
            if (temp->data1 > maxRow)
            {
                maxRow = temp->data1;
            }
            if (temp->data2 > maxCol)
            {
                maxCol = temp->data2;
            }
            temp = temp->next;
        }
        maxRow = matrix.maxR;
        maxCol = matrix.maxC;
        maxCol--;
        maxRow--;

        os << "[";
        for (int row = 0; row <= maxRow; row++)
        {
            if (row != 0)
            {
                os << ";";
            }
            for (int col = 0; col <= maxCol; col++)
            {
                bool found = false;
                temp = matrix.head;
                while (temp != NULL)
                {
                    if (temp->data1 == row && temp->data2 == col)
                    {
                        found = true;
                        break;
                    }
                    temp = temp->next;
                }
                if (found)
                {
                    os << (col == 0 ? "" : " ") << "1";
                }
                else
                {
                    os << (col == 0 ? "" : " ") << "0";
                }

            }
        }
        os << "]";

        return os;
    }

    class Iterator {
    private:
        Node* ptr;
    public:
        Iterator(Node* p = nullptr) : ptr(p) {}

        Iterator& operator++()
        {
            ptr = ptr->next;
            return *this;
        }

        int operator*() const
        {
            return ptr->data1 * ptr->data2;
        }

        bool operator!=(const Iterator& other) const
        {
            return ptr != other.ptr;
        }
    };
    Iterator begin() const
    {
        return Iterator(head);
    }
    Iterator end() const
    {
        return Iterator(nullptr);
    }

};


int main()
{
    setlocale(LC_ALL, "Russian");

    BoolMatrix list;
    cin >> list;
    BoolMatrix list2;
    cin >> list2;
    BoolMatrix res;
    res = list * list2;
    cout << res;


    return 0;
}
