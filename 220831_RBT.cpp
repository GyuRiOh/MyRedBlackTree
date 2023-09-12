// 220826_Binary_Tree.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "220826_Binary_Tree.h"

#define MAX_LOADSTRING 100


HWND hWnd; //윈도우 핸들

//노드를 담고있는 별도의 배열
short nodeArray[1000] = { 0 };

//카운터
int cnt = 0;

//출력 키
int g_key = 0;

//=================================
//이진 트리 세부 구현내용
//=================================

enum color
{
    RED = 0,
    BLACK
};

struct TreeNode
{
    short key;
    TreeNode* parent;
    TreeNode* left, *right;
    bool color = RED;
};

//레드블랙트리의 루트노드, 임시노드, 닐노드
TreeNode* root = nullptr;
TreeNode* temp = nullptr;
TreeNode* NIL_node = new TreeNode;

TreeNode* Search(TreeNode* node, int key)
{
    TreeNode* node_x = node;
    if (node_x == NIL_node || key == node_x->key)
        return node_x;

    if (key < node_x->key)
        return Search(node_x->left, key);
    else
        return Search(node_x->right, key);     
}

TreeNode* IterativeSearch(TreeNode* node, int key)
{
    TreeNode* node_x = node;
    while (node_x != NIL_node && key != node_x->key)
    {
        if (key < node_x->key)
            node_x = node_x->left;
        else
            node_x = node_x->right;
    }

    return node_x;
}

TreeNode* MinValueNode(TreeNode* node)
{
    TreeNode* current = node;
    //맨 왼쪽 단말 노드를 찾으러 내려감
    while (current->left != NIL_node)
        current = current->left;

    return current;
}

TreeNode* MaxValueNode(TreeNode* node)
{
    TreeNode* current = node;
    //맨 왼쪽 단말 노드를 찾으러 내려감
    while (current->right != NIL_node)
        current = current->right;

    return current;
}


TreeNode* Successor(TreeNode* node)
{
    TreeNode* node_x = node;
    if (node_x->right != NIL_node)
        return MinValueNode(node_x->right);

    TreeNode* node_parent_y = node_x->parent;
    while (node_parent_y != NIL_node && node_x == node_parent_y->right)
    {
        node_x = node_parent_y;
        node_parent_y = node_parent_y->parent;
    }
    return node_parent_y;
}

TreeNode* Predecessor(TreeNode* node)
{
    TreeNode* node_x = node;
    if (node_x->left != NIL_node)
        return MaxValueNode(node_x->left);

    TreeNode* node_parent_y = node_x->parent;
    while (node_parent_y != NIL_node && node_x == node_parent_y->left)
    {
        node_x = node_parent_y;
        node_parent_y = node_parent_y->parent;
    }
    return node_parent_y;

}

void LeftRotate(TreeNode* node_x)
{
    TreeNode* node_right_y = node_x->right; //set node_right
    node_x->right = node_right_y->left; //turn node_right's left subtree into node's right subtree

    if (node_right_y->left != NIL_node)
        node_right_y->left->parent = node_x;

    node_right_y->parent = node_x->parent;

    if (node_x->parent == NIL_node)
        root = node_right_y;
    else if (node_x == node_x->parent->left)
        node_x->parent->left = node_right_y;
    else
        node_x->parent->right = node_right_y;

    node_right_y->left = node_x;
    node_x->parent = node_right_y;
}

void RightRotate(TreeNode* node_y)
{
    TreeNode* node_left_x = node_y->left; //set left_x
    node_y->left = node_left_x->right;

    if (node_left_x->right != NIL_node)
        node_left_x->right->parent = node_y;

    node_left_x->parent = node_y->parent;

    if (node_y->parent == NIL_node)
        root = node_left_x;
    else if (node_y == node_y->parent->right)
        node_y->parent->right = node_left_x;
    else
        node_y->parent->left = node_left_x;

    node_left_x->right = node_y;
    node_y->parent = node_left_x;
}

TreeNode* NewNode(int key, TreeNode* parent)
{
    TreeNode* new_temp_node = new TreeNode;
    new_temp_node->key = key;
    new_temp_node->parent = parent;
    new_temp_node->left = NIL_node;
    new_temp_node->right = NIL_node;
    nodeArray[cnt++] = new_temp_node->key;

    return new_temp_node;
}

TreeNode* NewRoot(int key)
{
    TreeNode* new_root_node = new TreeNode;
    new_root_node->color = BLACK;
    new_root_node->left = NIL_node;
    new_root_node->right = NIL_node;
    new_root_node->parent = NIL_node;
    new_root_node->key = key;

    root = new_root_node;
    return root;
}

void RB_InsertFixup(TreeNode* node);
void inorder_tree_walk(TreeNode* root, int xPos, int yPos, int depth);

void InsertNode(int key)
{
    TreeNode* end_y = NIL_node;
    TreeNode* start_x = root;
    TreeNode* node = nullptr;
    
    if (!start_x)
    {
        NewRoot(key);
        return;
    }

    while (start_x != NIL_node)
    {
        end_y = start_x;
        if (key < start_x->key)
            start_x = start_x->left;
        else if (key > start_x->key)
            start_x = start_x->right;
        else
            return;
    }

    if (key < end_y->key)
    {
        end_y->left = NewNode(key, end_y);
        node = end_y->left;
    }
    else if (key > end_y->key)
    {
        end_y->right = NewNode(key, end_y);
        node = end_y->right;
    }

    ////밸런스 함수 들어가기        
    //InvalidateRect(hWnd, NULL, FALSE);
    //UpdateWindow(hWnd);


    //Sleep(2000);
    RB_InsertFixup(node);
}

void RB_InsertFixup(TreeNode* node)
{
    TreeNode* node_z = node;
    while (node_z->parent->color == RED)
    {
        if (node_z->parent == node_z->parent->parent->left)
        {
            TreeNode* uncle = node_z->parent->parent->right;
            if (uncle->color == RED)
            {
                node_z->parent->color = BLACK;
                uncle->color = BLACK;
                node_z->parent->parent->color = RED;
                node_z = node_z->parent->parent;

                //InvalidateRect(hWnd, NULL, FALSE);
                //UpdateWindow(hWnd);
                //Sleep(2000);
            }
            else 
            {
                if (node_z == node_z->parent->right)
                {
                    node_z = node_z->parent;
                    LeftRotate(node_z);

                 /*   InvalidateRect(hWnd, NULL, FALSE);
                    UpdateWindow(hWnd);
                    Sleep(2000);*/
                }
                node_z->parent->color = BLACK;
                node_z->parent->parent->color = RED;
                RightRotate(node_z->parent->parent);

            /*    InvalidateRect(hWnd, NULL, FALSE);
                UpdateWindow(hWnd);
                Sleep(2000);*/
            }
        }
        else
        {
            TreeNode* uncle = node_z->parent->parent->left;
            if (uncle->color == RED)
            {
                node_z->parent->color = BLACK;
                uncle->color = BLACK;
                node_z->parent->parent->color = RED;
                node_z = node_z->parent->parent;


                /*InvalidateRect(hWnd, NULL, FALSE);
                UpdateWindow(hWnd);
                Sleep(2000);*/
            }
            else
            {
                if (node_z == node_z->parent->left)
                {
                    node_z = node_z->parent;
                    RightRotate(node_z);

                    /*InvalidateRect(hWnd, NULL, FALSE);
                    UpdateWindow(hWnd);
                    Sleep(2000)*/;
                }
                node_z->parent->color = BLACK;
                node_z->parent->parent->color = RED;
                LeftRotate(node_z->parent->parent);

                /*InvalidateRect(hWnd, NULL, FALSE);
                UpdateWindow(hWnd);
                Sleep(2000);*/
            }
        }
    }

    root->color = BLACK;


 /*   InvalidateRect(hWnd, NULL, FALSE);
    UpdateWindow(hWnd);
    Sleep(2000);*/


}

void Transplant(TreeNode* originalNode, TreeNode* replaceNode)
{
    if (originalNode->parent == NIL_node)
        root = replaceNode;
    else if (originalNode == originalNode->parent->left)
        originalNode->parent->left = replaceNode;
    else
        originalNode->parent->right = replaceNode;

    if (replaceNode != NIL_node)
        replaceNode->parent = originalNode->parent;
}


void RB_Transplant(TreeNode* originalNode, TreeNode* replaceNode)
{
    if (originalNode->parent == NIL_node)
        root = replaceNode;
    else if (originalNode == originalNode->parent->left)
        originalNode->parent->left = replaceNode;
    else
        originalNode->parent->right = replaceNode;

    //assign to replaceNode.parent when replaceNode NIL_node
    replaceNode->parent = originalNode->parent;
}

void RB_DeleteFixup(TreeNode* replaceNode);

void RB_Delete(TreeNode* deleteNode)
{
    TreeNode* successorNode = deleteNode;
    bool s_original_color = successorNode->color;
    TreeNode* s_replaceNode = nullptr;

    if (deleteNode->left == NIL_node)
    {
        s_replaceNode = deleteNode->right;
        RB_Transplant(deleteNode, deleteNode->right); 
        
        InvalidateRect(hWnd, NULL, FALSE);
        UpdateWindow(hWnd);
        Sleep(4000);
    }
    else if (deleteNode->right == NIL_node)
    {
        s_replaceNode = deleteNode->left;
        RB_Transplant(deleteNode, deleteNode->left); 
        
        InvalidateRect(hWnd, NULL, FALSE);
        UpdateWindow(hWnd);
        Sleep(4000);
    }
    else
    {
        successorNode = MinValueNode(deleteNode->right);
        s_original_color = successorNode->color;
        s_replaceNode = successorNode->right;


        InvalidateRect(hWnd, NULL, FALSE);
        UpdateWindow(hWnd);
        Sleep(4000);

        if (successorNode->parent == deleteNode)
        {
            s_replaceNode->parent = successorNode;

            InvalidateRect(hWnd, NULL, FALSE);
            UpdateWindow(hWnd);
            Sleep(4000);
        }
        else
        {
            RB_Transplant(successorNode, successorNode->right);
            successorNode->right = deleteNode->right;
            successorNode->right->parent = successorNode;


            InvalidateRect(hWnd, NULL, FALSE);
            UpdateWindow(hWnd);
            Sleep(4000);
        }

        RB_Transplant(deleteNode, successorNode);
        successorNode->left = deleteNode->left;
        successorNode->left->parent = successorNode;
        successorNode->color = deleteNode->color;


        InvalidateRect(hWnd, NULL, FALSE);
        UpdateWindow(hWnd);
        Sleep(4000);

    }


    if (s_original_color == BLACK)
        RB_DeleteFixup(s_replaceNode);

}


void RB_DeleteFixup(TreeNode* replaceNode)
{
    while (replaceNode != root && replaceNode->color == BLACK)
    {
        TreeNode* siblingNode;
        if (replaceNode == replaceNode->parent->left)
        {
            siblingNode = replaceNode->parent->right;
            if (siblingNode->color == RED)
            {
                //case 1
                siblingNode->color = BLACK;
                replaceNode->parent->color = RED;
                LeftRotate(replaceNode->parent);
                siblingNode = replaceNode->parent->right;


                InvalidateRect(hWnd, NULL, FALSE);
                UpdateWindow(hWnd);
                Sleep(4000);
            }

            if (siblingNode->left->color == BLACK && siblingNode->right->color == BLACK)
            {
                //case 2
                siblingNode->color = RED;
                replaceNode = replaceNode->parent;


                InvalidateRect(hWnd, NULL, FALSE);
                UpdateWindow(hWnd);
                Sleep(4000);
            }
            else 
            {
                if (siblingNode->right->color == BLACK)
                {
                    //case 3
                    siblingNode->left->color = BLACK;
                    siblingNode->color = RED;
                    RightRotate(siblingNode);
                    siblingNode = replaceNode->parent->right;


                    InvalidateRect(hWnd, NULL, FALSE);
                    UpdateWindow(hWnd);
                    Sleep(4000);
                }

                //case 4
                siblingNode->color = replaceNode->parent->color;
                replaceNode->parent->color = BLACK;
                siblingNode->right->color = BLACK;
                LeftRotate(replaceNode->parent);
                replaceNode = root;


                InvalidateRect(hWnd, NULL, FALSE);
                UpdateWindow(hWnd);
                Sleep(4000);
            }

        }
        else
        {
            siblingNode = replaceNode->parent->left;
            if (siblingNode->color == RED)
            {
                //case 1
                siblingNode->color = BLACK;
                replaceNode->parent->color = RED;
                RightRotate(replaceNode->parent);
                siblingNode = replaceNode->parent->left;


                InvalidateRect(hWnd, NULL, FALSE);
                UpdateWindow(hWnd);
                Sleep(4000);
            }

            if (siblingNode->right->color == BLACK && siblingNode->left->color == BLACK)
            {
                //case 2
                siblingNode->color = RED;
                replaceNode = replaceNode->parent;


                InvalidateRect(hWnd, NULL, FALSE);
                UpdateWindow(hWnd);
                Sleep(4000);
            }
            else
            {
                if (siblingNode->left->color == BLACK)
                {
                    //case 3
                    siblingNode->right->color = BLACK;
                    siblingNode->color = RED;
                    LeftRotate(siblingNode);
                    siblingNode = replaceNode->parent->left;


                    InvalidateRect(hWnd, NULL, FALSE);
                    UpdateWindow(hWnd);
                    Sleep(4000);
                }

                //case 4
                siblingNode->color = replaceNode->parent->color;
                replaceNode->parent->color = BLACK;
                siblingNode->left->color = BLACK;
                RightRotate(replaceNode->parent);
                replaceNode = root;


                InvalidateRect(hWnd, NULL, FALSE);
                UpdateWindow(hWnd);
                Sleep(4000);
            }
            
        }

    }

    replaceNode->color = BLACK;


    InvalidateRect(hWnd, NULL, FALSE);
    UpdateWindow(hWnd);
    Sleep(4000);
}


////이진 탐색 트리와 키가 주어지면 키가 저장된 노드를 삭제하고
////새로운 루트노드를 반환한다.
//TreeNode* delete_node(TreeNode* root, int key)
//{
//    if (root == nullptr)
//        return root;
//
//    //만약 키가 루트보다 작으면 왼쪽 서브트리에 있는 것이다.
//    if (key < root->key)
//        root->left = delete_node(root->left, key);
//    //만약 키가 루트보다 크면 오른쪽 서브트리에 있는 것이다.
//    else if (key > root->key)
//        root->right = delete_node(root->right, key);
//    //키가 루트와 같으면 이 노드를 삭제하면 된다.
//    else
//    {
//        //루트의 왼쪽이 비었을 때
//        if (root->left == nullptr)
//        {
//            TreeNode* temp = nullptr;
//            memmove(&temp, &root->right, sizeof(TreeNode*));
//            delete root;
//            return temp;
//        }
//        //또는 오른쪽이 비었을 때
//        else if (root->right == nullptr)
//        {
//            TreeNode* temp = nullptr;
//            memmove(&temp, &root->left, sizeof(TreeNode*));
//            delete root;
//            return temp;
//        }
//        //세 번째 경우
//        TreeNode* temp = MinValueNode(root->right);
//        //중위 순회 시 후계 노드를 복사한다.
//        root->key = temp->key;
//        //중위 순회 시 후계 노드를 삭제한다.
//        root->right = delete_node(root->right, temp->key);
//    }
//
//    return root;
//}

int Cipher(int key, int count)
{
    if (key <= 0)
        return count;

    key = key / 10;
    count++;
    
    Cipher(key, count);
}

constexpr int xLength = 40;
constexpr int yLength = 70;
constexpr int maxDepth = 9;

void inorder_tree_walk(TreeNode* root, int xPos, int yPos, HDC hdc, int depth)
{
    SetTextColor(hdc, root->color);    int x_length = 0;
    switch (depth)
    {
    case 0:
        x_length = 500;
        break;
    case 1:
        x_length = 300;
        break;
    case 2:
        x_length = 150;
        break;
    case 3:
        x_length = 100;
        break;
    case 4:
        x_length = 50;
        break;
    case 5:
        x_length = 20;
        break;
    case 6:
        x_length = 10;
        break;
    case 7:
        x_length = 5;
        break;
    case 8:
        x_length = 2;
        break;
    }

    if (root != NIL_node)
    {
        //왼쪽 x좌표
        int leftXPos = xPos - x_length;

        //오른쪽 x좌표
        int rightXPos = xPos + x_length;

        //공통 y좌표
        int commonYPos = yPos + yLength;

        //그릴 펜 준비
        HPEN hpen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
    

        HPEN hpenLine = CreatePen(PS_SOLID, 3, RGB(150, 0, 255));
        SelectObject(hdc, hpenLine);

        if (root->left != NIL_node)
        {
            MoveToEx(hdc, xPos, yPos, nullptr);
            LineTo(hdc, leftXPos, commonYPos);
        }

        if (root->right != NIL_node)
        {

            MoveToEx(hdc, xPos, yPos, nullptr);
            LineTo(hdc, rightXPos, commonYPos);
        }

        //왼쪽 서브트리 순회
        inorder_tree_walk(root->left, leftXPos, commonYPos, hdc, depth+1);
        
        //노드 방문
        wchar_t buffer[4] = {0};
        swprintf(buffer, 4, L"%d", root->key);      
        int count = Cipher(root->key, 0);
        SelectObject(hdc, hpen);


        //오른쪽 서브트리 순회
        inorder_tree_walk(root->right, rightXPos, commonYPos, hdc, depth+1);

        if(root->color)
            SetTextColor(hdc, RGB(0, 0, 0));
        else
            SetTextColor(hdc, RGB(255, 0, 0));

        Ellipse(hdc, xPos - 20, yPos - 20, xPos + 20, yPos + 20);
        TextOutW(hdc, xPos - 8, yPos - 8, buffer, count);


        DeleteObject(hpen);
        DeleteObject(hpenLine);
    }
}

void inorder_tree_walk(TreeNode* root, int xPos, int yPos, int depth)
{
    HDC hdc = GetDC(hWnd);
    int x_length = 0;
    switch (depth)
    {
    case 0:
        x_length = 500;
        break;
    case 1:
        x_length = 300;
        break;
    case 2:
        x_length = 150;
        break;
    case 3:
        x_length = 100;
        break;
    case 4:
        x_length = 50;
        break;
    case 5:
        x_length = 20;
        break;
    case 6:
        x_length = 10;
        break;
    case 7:
        x_length = 5;
        break;
    case 8:
        x_length = 2;
        break;
    }

    if (root != NIL_node)
    {
        //왼쪽 x좌표
        int leftXPos = xPos - x_length;

        //오른쪽 x좌표
        int rightXPos = xPos + x_length;

        //공통 y좌표
        int commonYPos = yPos + yLength;

        //그릴 펜 준비
        HPEN hpen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));


        HPEN hpenLine = CreatePen(PS_SOLID, 3, RGB(150, 0, 255));
        SelectObject(hdc, hpenLine);

        if (root->left != NIL_node)
        {
            MoveToEx(hdc, xPos, yPos, nullptr);
            LineTo(hdc, leftXPos, commonYPos);
        }

        if (root->right != NIL_node)
        {

            MoveToEx(hdc, xPos, yPos, nullptr);
            LineTo(hdc, rightXPos, commonYPos);
        }

        //왼쪽 서브트리 순회
        inorder_tree_walk(root->left, leftXPos, commonYPos, depth + 1);

        //노드 방문
        wchar_t buffer[4] = { 0 };
        swprintf(buffer, 4, L"%d", root->key);
        int count = Cipher(root->key, 0);
        SelectObject(hdc, hpen);


        //오른쪽 서브트리 순회
        inorder_tree_walk(root->right, rightXPos, commonYPos, depth + 1);

        if (root->color == BLACK)
            SetTextColor(hdc, RGB(0, 0, 0));
        else
            SetTextColor(hdc, RGB(255, 0, 0));

        Ellipse(hdc, xPos - 20, yPos - 20, xPos + 20, yPos + 20);
        TextOutW(hdc, xPos - 8, yPos - 8, buffer, count);


        DeleteObject(hpen);
        DeleteObject(hpenLine);
    }
    
    ReleaseDC(hWnd,hdc);
}


//=========================================================================

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.


//컬러 값
char color = 255;

//카운터
int i = 0;

//WM_PAINT 호출 플래그
bool paintFlag = false;

//반복횟수
constexpr int repeat = 25;

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY220826BINARYTREE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY220826BINARYTREE));
    MSG msg;

    srand(100);

    unordered_map<int, int> nodeMap;

    for (int iCnt = 0; iCnt < repeat; iCnt++)
    {
        int random = 0;
        while (random == 0)
        {
            random = rand() % repeat;
        }
        
        g_key = random;
        InsertNode(random);
        nodeMap.insert(make_pair(random, random));


        InvalidateRect(hWnd, NULL, FALSE);
        UpdateWindow(hWnd);
        Sleep(100);

    }
 

    InvalidateRect(hWnd, NULL, FALSE);
    UpdateWindow(hWnd);
    Sleep(500);
    inorder_tree_walk(root, 800, 40, 1);


    auto iter = nodeMap.begin();
    for (; iter != nodeMap.end(); ++iter)
    {
        //이터레이터에서 키 뽑기
        auto pair = *iter;
        g_key = pair.first;     

        //키에 대응되는 노드 찾기
        TreeNode* delNode = IterativeSearch(root, g_key);
        RB_Delete(delNode);
    }

    


    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY220826BINARYTREE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)GetStockObject(GRAY_BRUSH);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY220826BINARYTREE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   NIL_node->color = BLACK;
   NIL_node->left = nullptr;
   NIL_node->right = nullptr;
   NIL_node->parent = nullptr;

   InsertNode(5);


   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
    {
        static HDC hdc, MemDC, tmpDC;
        static HBITMAP BackBit, oldBackBit;
        static RECT bufferRT;
        PAINTSTRUCT ps;
        hdc = BeginPaint(hWnd, &ps);

        GetClientRect(hWnd, &bufferRT);
        MemDC = CreateCompatibleDC(hdc);
        BackBit = CreateCompatibleBitmap(hdc, bufferRT.right, bufferRT.bottom);
        oldBackBit = (HBITMAP)SelectObject(MemDC, BackBit);
        PatBlt(MemDC, 0, 0, bufferRT.right, bufferRT.bottom, WHITENESS);
        tmpDC = hdc;
        hdc = MemDC;
        MemDC = tmpDC;

        inorder_tree_walk(root, 800, 40, hdc, 1);
        //키를 문자열로 변환
        wchar_t buffer[4] = { 0 };
        swprintf(buffer, 4, L"%d", g_key);
        int count = Cipher(g_key, 0);

        //왼쪽 상단에 출력
        TextOutW(hdc, 100, 100, buffer, count);

        tmpDC = hdc;
        hdc = MemDC;
        MemDC = tmpDC;

        GetClientRect(hWnd, &bufferRT);
        BitBlt(hdc, 0, 0, bufferRT.right, bufferRT.bottom, MemDC, 0, 0, SRCCOPY);
        SelectObject(MemDC, oldBackBit);
        DeleteObject(BackBit);
        DeleteDC(MemDC);
        EndPaint(hWnd, &ps);
    }
       
        break;
    case WM_LBUTTONDOWN:
       // root = delete_node(root, nodeArray[i++]);
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
