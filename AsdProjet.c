#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    int row;
    int col;
    int value;
    struct Node *next;
} Node;

Node *createNode(int r, int c, int v)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (!newNode)
        return NULL;

    newNode->row = r;
    newNode->col = c;
    newNode->value = v;
    newNode->next = NULL;

    return newNode;
}

int **allocateMatrix(int n, int m)
{
    int **mat = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++)
        mat[i] = (int *)calloc(m, sizeof(int));
    return mat;
}

void freeMatrix(int **mat, int n)
{
    for (int i = 0; i < n; i++)
        free(mat[i]);
    free(mat);
}

void freeList(Node *head)
{
    Node *temp;
    while (head)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}

void inputMatrix(int **mat, int n, int m)
{
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
        {
            printf("mat[%d][%d]: ", i, j);
            scanf("%d", &mat[i][j]);
        }
}

void displayMatrix(int **mat, int n, int m)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
            printf("%d ", mat[i][j]);
        printf("\n");
    }
}

int isSparse(int **mat, int n, int m)
{
    int count = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            if (mat[i][j] != 0)
                count++;
    return ((float)count / (n * m)) <= 0.3;
}

Node *convertToList(int **mat, int n, int m)
{
    Node *head = NULL, *tail = NULL;

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            if (mat[i][j] != 0)
            {
                Node *newNode = createNode(i, j, mat[i][j]);
                if (!newNode)
                    continue;

                if (!head)
                {
                    head = newNode;
                    tail = newNode;
                }
                else
                {
                    tail->next = newNode;
                    tail = newNode;
                }
            }

    return head;
}

void displayList(Node *head)
{
    while (head)
    {
        printf("(%d,%d,%d) -> ", head->row, head->col, head->value);
        head = head->next;
    }
    printf("NULL\n");
}

void searchElement(Node *head, int r, int c)
{
    while (head)
    {
        if (head->row == r && head->col == c)
        {
            printf("Found: %d at (%d,%d)\n", head->value, r, c);
            return;
        }
        head = head->next;
    }
    printf("Element at (%d,%d) is 0 (not in list).\n", r, c);
}

Node *insertAtBeginning(Node *head, int r, int c, int v)
{
    Node *newNode = createNode(r, c, v);
    if (!newNode)
        return head;

    newNode->next = head;
    return newNode;
}

Node *insertAtEnd(Node *head, int r, int c, int v)
{
    Node *newNode = createNode(r, c, v);
    if (!newNode)
        return head;

    if (!head)
        return newNode;

    Node *temp = head;
    while (temp->next)
        temp = temp->next;

    temp->next = newNode;
    return head;
}

Node *insertMiddle(Node *head, int r, int c, int v)
{
    if (!head)
        return createNode(r, c, v);

    Node *temp = head;
    while (temp->next && temp->next->row < r)
        temp = temp->next;

    Node *newNode = createNode(r, c, v);
    if (!newNode)
        return head;

    newNode->next = temp->next;
    temp->next = newNode;
    return head;
}

Node *deleteElement(Node *head, int r, int c)
{
    Node *temp = head, *prev = NULL;
    while (temp && (temp->row != r || temp->col != c))
    {
        prev = temp;
        temp = temp->next;
    }
    if (!temp)
        return head;

    if (!prev)
        head = temp->next;
    else
        prev->next = temp->next;

    free(temp);
    return head;
}

void reconstructMatrix(Node *head, int **mat, int n, int m)
{
    if (!head)
    {
        printf("List is empty. Nothing to reconstruct.\n");
        return;
    }

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            mat[i][j] = 0;

    while (head)
    {
        if (head->row >= 0 && head->row < n &&
            head->col >= 0 && head->col < m)
        {
            mat[head->row][head->col] = head->value;
        }
        else
        {
            printf("Invalid node ignored: (%d,%d)\n", head->row, head->col);
        }
        head = head->next;
    }
}

void displayStats(Node *head, int n, int m)
{
    if (!head)
        return;

    int maxVal = head->value;
    int *rowCount = (int *)calloc(n, sizeof(int));
    int *colCount = (int *)calloc(m, sizeof(int));

    Node *temp = head;
    while (temp)
    {
        if (temp->value > maxVal)
            maxVal = temp->value;
        rowCount[temp->row]++;
        colCount[temp->col]++;
        temp = temp->next;
    }

    int maxR = 0, maxC = 0;
    for (int i = 1; i < n; i++)
        if (rowCount[i] > rowCount[maxR])
            maxR = i;
    for (int j = 1; j < m; j++)
        if (colCount[j] > colCount[maxC])
            maxC = j;

    printf("Largest non-zero: %d\n", maxVal);
    printf("Row with most non-zeros: %d\n", maxR);
    printf("Col with most non-zeros: %d\n", maxC);

    free(rowCount);
    free(colCount);
}

void compareMemory(int n, int m, int nonZero)
{
    int arrayMem = n * m * sizeof(int);
    int listMem = nonZero * sizeof(Node);
    printf("Memory used by array: %d bytes\n", arrayMem);
    printf("Memory used by linked list: %d bytes\n", listMem);
}

int main()
{
    int n, m, choice, r, c, v;
    Node *list = NULL;

    printf("Enter dimensions (N M): ");
    scanf("%d %d", &n, &m);

    int **mat = allocateMatrix(n, m);

    do
    {
        printf("\n--- MENU ---\n");
        printf("1. Input Matrix\n2. Display Matrix\n3. Check Sparse\n4. Convert to Linked List\n");
        printf("5. Display Linked List\n6. Search Element\n7. Insert Beginning\n8. Insert End\n");
        printf("9. Insert Middle\n10. Delete Element\n11. Reconstruct Matrix\n12. Display Stats\n");
        printf("13. Compare Memory\n0. Exit\nChoice: ");

        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            inputMatrix(mat, n, m);
            break;
        case 2:
            displayMatrix(mat, n, m);
            break;
        case 3:
            printf(isSparse(mat, n, m) ? "Sparse\n" : "Not Sparse\n");
            break;
        case 4:
            list = convertToList(mat, n, m);
            break;
        case 5:
            displayList(list);
            break;
        case 6:
            printf("Enter i j: ");
            scanf("%d %d", &r, &c);
            searchElement(list, r, c);
            break;
        case 7:
            printf("Enter i j value: ");
            scanf("%d %d %d", &r, &c, &v);
            list = insertAtBeginning(list, r, c, v);
            break;
        case 8:
            printf("Enter i j value: ");
            scanf("%d %d %d", &r, &c, &v);
            list = insertAtEnd(list, r, c, v);
            break;
        case 9:
            printf("Enter i j value: ");
            scanf("%d %d %d", &r, &c, &v);
            list = insertMiddle(list, r, c, v);
            break;
        case 10:
            printf("Enter i j: ");
            scanf("%d %d", &r, &c);
            list = deleteElement(list, r, c);
            break;

        case 11:
            if (!list)
                printf("List is empty. Convert first!\n");
            else
            {
                reconstructMatrix(list, mat, n, m);
                printf("Matrix rebuilt.\n");
            }
            break;

        case 12:
            displayStats(list, n, m);
            break;

        case 13:
        {
            int count = 0;
            Node *temp = list;
            while (temp)
            {
                count++;
                temp = temp->next;
            }
            compareMemory(n, m, count);
            break;
        }
        }

    } while (choice != 0);

    freeMatrix(mat, n);
    freeList(list);

    return 0;
}
