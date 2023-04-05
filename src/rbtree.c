#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void)
{
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed

  node_t *nil_node = (node_t *)calloc(1, sizeof(node_t)); // sentinel 노드 생성

  nil_node->color = RBTREE_BLACK;
  nil_node->key = 0;
  nil_node->parent = nil_node->left = nil_node->right = p->nil;

  p->nil = nil_node;
  p->root = p->nil;
  return p;
}

void delete_rbtree_node(node_t *node, node_t *nil)
{
  if (node == nil)
  {
    return;
  }
  if (node->left != nil)
  {
    delete_rbtree_node(node->left, nil);
  }
  if (node->right != nil)
  {
    delete_rbtree_node(node->right, nil);
  }
  free(node);
}

void delete_rbtree(rbtree *t)
{
  // TODO: reclaim the tree nodes's memory
  delete_rbtree_node(t->root, t->nil); // dfs로 자식 노드들을 재귀적으로 먼저 해제해 메모리 누수를 막음.
  free(t->nil);
  free(t);
}

void left_rotate(rbtree *t, node_t *x)
{
  node_t *y = x->right; // y는 회전 후 x의 부모가 될 노드
  x->right = y->left;   // y의 왼쪽 서브트리를 x의 오른쪽 서브트리로 이동
  if (y->left != t->nil)
  {
    y->left->parent = x;
  }
  y->parent = x->parent; // x와 y의 부모를 연결
  if (x->parent == t->nil)
  {
    t->root = y;
  }
  else if (x == x->parent->left)
  {
    x->parent->left = y;
  }
  else
  {
    x->parent->right = y;
  }
  y->left = x; // x를 y의 왼쪽으로 이동
  x->parent = y;
}

void right_rotate(rbtree *t, node_t *x)
{
  node_t *y = x->left; // y는 회전 후 x의 부모가 될 노드
  x->left = y->right;  // y의 오른쪽 서브트리를 x의 왼쪽 서브트리로 이동
  if (y->right != t->nil)
  {
    y->right->parent = x;
  }
  y->parent = x->parent; // x와 y의 부모를 연결
  if (x->parent == t->nil)
  {
    t->root = y;
  }
  else if (x == x->parent->left)
  {
    x->parent->left = y;
  }
  else
  {
    x->parent->right = y;
  }
  y->right = x; // x를 y의 오른쪽으로 이동
  x->parent = y;
}

void rbtree_insert_fixup(rbtree *t, node_t *z) // 확인할 것 1. 부모와 삼촌의 색깔이 동일한가 2. 부모와 조부모와 내가 일직선상에 있는가
{
  while (z->parent->color == RBTREE_RED) // 조건 4를 만족하면 반복문 종료 (애초에 삽입된 노드의 부모가 black이라면 fix up 수행할 필요 없음)
  {
    if (z->parent == z->parent->parent->left) // 삼촌 찾는 조건문
    {
      node_t *y = z->parent->parent->right; // 삼촌을 가르키는 포인터 선언
      if (y->color == RBTREE_RED)           // 자신의 부모와 삼촌이 모두 red일 경우 (red를 모아서 조부모에게 전달 한다는 느낌)
      {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED; // 조부모 이하의 트리는 rbtree 속성을 모두 만족 시킨 상황
        z = z->parent->parent;                 // 조부모를 z로 선언하여 반복문 처음으로 돌아감.
      }
      else // 자신의 부모는 red이고 삼촌은 black일 경우
      {
        if (z == z->parent->right) // 자신과 부모와 조부모가 일직선 상에 없는 경우 (왼쪽 회전 후 오른쪽 회전)
        {
          z = z->parent;
          left_rotate(t, z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        right_rotate(t, z->parent->parent);
      }
    }
    else // left와 right를 바꿈 (거울모드 느낌)
    {
      node_t *y = z->parent->parent->left;
      if (y->color == RBTREE_RED)
      {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else
      {
        if (z == z->parent->left)
        {
          z = z->parent;
          right_rotate(t, z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotate(t, z->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key)
{
  // TODO: implement insert
  node_t *z = (node_t *)calloc(1, sizeof(node_t)); // 삽입할 새로운 노드를 메모리에 할당
  z->key = key;

  node_t *x = t->root; // 새로운 노드가 들어갈 위치를 찾는 포인터
  node_t *y = t->nil;

  while (x != t->nil) // 새로운 노드 z의 부모를 찾으면 반복문 탈출
  {
    y = x;
    if (z->key < x->key) // 주어진 노드보다 작으면 왼쪽으로 x 포인터 이동
    {
      x = x->left;
    }
    else
    {
      x = x->right; // 주어진 노드보다 크면 오른쪽으로 x 포인터 이동
    }
  }
  z->parent = y;

  if (y == t->nil) // 최초의 노드 삽입의 경우
  {
    t->root = z;
  }
  else if (z->key < y->key) // 부모보다 값이 작으면 왼쪽에 삽입
  {
    y->left = z;
  }
  else // 부모보다 값이 크면 오른쪽에 삽입
  {
    y->right = z;
  }
  z->left = t->nil; // 새로 말단에 삽입된 노드의 left, right는 nil노드여야 함
  z->right = t->nil;
  z->color = RBTREE_RED; // 삽입하는 노드의 색깔은 무조건 red

  rbtree_insert_fixup(t, z); // 삽입 후 만족하지 않는 rbtree 특성을 맞춰주는 함수

  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key)
{
  // TODO: implement find
  node_t *x = t->root; // 트리의 root부터 탐색해 나감

  while (x != t->nil) // 포인터가 nil을 가리키면 해당 값이 트리에 없다는 것이므로 반복문 종료
  {

    if (key < x->key)
    {
      x = x->left;
    }
    else if (key > x->key)
    {
      x = x->right;
    }
    else if (key == x->key)
    {
      return x; // 값을 찾아내면 해당 노드를 가리키는 포인터를 리턴
    }
  }

  return NULL; // 값이 없다면 NULL 리턴
}

node_t *rbtree_min(const rbtree *t)
{
  // TODO: implement find
  node_t *x = t->root;
  while (x->left != t->nil) // 왼쪽 자식이 없을 때까지 왼쪽을 찾아 들어감
  {
    x = x->left;
  }
  return x;
}

node_t *rbtree_max(const rbtree *t)
{
  // TODO: implement find
  node_t *x = t->root; // 오른쪽 자식이 없을 때까지 오른쪽을 찾아 들어감
  while (x->right != t->nil)
  {
    x = x->right;
  }
  return x;
}

void rbtree_transplant(rbtree *t, node_t *u, node_t *v) // 후임자(successor)를 삭제한 노드 자리에 넣어주는 함수
{
  if (u->parent == t->nil) // 삭제하려고 하는 노드 u가 루트일 경우
  {
    t->root = v;
  }
  else if (u == u->parent->left)
  {
    u->parent->left = v;
  }
  else
  {
    u->parent->right = v;
  }
  v->parent = u->parent;
}

node_t *rbtree_minimum(rbtree *t, node_t *x) // 주어진 node x의 오른쪽 자식 트리에서의 가장 최소값(successor)을 찾는 함수
{
  while (x->left != t->nil)
  {
    x = x->left;
  }
  return x;
}

void rbtree_erase_fixup(rbtree *t, node_t *x)
{
}

// 삭제하려는 노드의 유효한 (nil 노드가 아닌) 자식의 개수가 1개 이하인지 2개인지 판별
// 한 개 이하 라면 그 하나의 자식으로 transplant
// 두 개 라면 successor를 찾아서 그 값으로 transplant
int rbtree_erase(rbtree *t, node_t *p)
{
  node_t *y = p;
  node_t *x;
  color_t y_origin_color = y->color;

  if (p->left == t->nil) // 삭제하려는 노드의 왼쪽 자식이 nil일 때
  {
    x = p->right;
    rbtree_transplant(t, p, p->right); // 삭제하려는 노드의 오른쪽 자식을 본인 자리로 올려줌
  }
  else if (p->right == t->nil) // 삭제하려는 노드가의 오른쪽 자식이 nil일 때
  {
    x = p->left;
    rbtree_transplant(t, p, p->left); // 삭제하려는 노드의 왼쪽 자식을 본인 자리로 올려줌
  }
  else // 삭제하려는 노드의 자식이 두 개 일 때,
  {
    y = rbtree_minimum(t, p->right); // successor 설정
    y_origin_color = y->color;       // successor의 색깔 저장
    x = y->right;
    if (y->parent == p) // successor가 바로 p의 자식일 경우
    {
      x->parent = y;
    }
    else // successor가 p의 자손일 경우
    {
      rbtree_transplant(t, y, y->right); // y를 p자리 이동하기 위해 y자리를 y의 자식으로 바꿔줌
      y->right = p->right;               // y의 left는 nil 이므로 처리해 줄 과정이 없음
      y->right->parent = y;
    }
    rbtree_transplant(t, p, y); // p와 y의 자리를 바꿔줌
    y->left = p->left;
    y->left->parent = y;
    y->color = p->color;
  }

  if (y_origin_color == RBTREE_BLACK) // 원래 색깔이 red 였다면 삭제해도 문제가 되지 않음
  {
    rbtree_erase_fixup(t, x);
  }
  return 0;
}

void inorder(node_t *root, node_t *nil, key_t *arr, int *index) // 크기순으로 배열에 담기 위한 중위순회 재귀함수
{
  if (root == nil)
  {
    return;
  }

  inorder(root->left, nil, arr, index);
  arr[(*index)++] = root->key; // 중위 순회는 가운데에서 arr에 저장
  inorder(root->right, nil, arr, index);
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  // TODO: implement to_array
  int *index = (int *)calloc(1, sizeof(int)); // 배열의 인덱스를 가리키는 포인터
  inorder(t->root, t->nil, arr, index);       // 중위 순회를 하면서 값을 배열에 저장
  free(index);
  return 0;
}