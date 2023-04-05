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
  return t->root;
}

node_t *rbtree_min(const rbtree *t)
{
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t)
{
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p)
{
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  // TODO: implement to_array
  return 0;
}