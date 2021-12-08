#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>


rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  node_t *node_nil = (node_t *)calloc(1, sizeof(node_t));
  node_nil ->color = RBTREE_BLACK;
  node_nil ->left = NULL;
  node_nil ->right = NULL;
  p->root = node_nil;
  p->nil = node_nil;
  return p;
}

void del_node(rbtree *t, node_t *n){
  if(n != t->nil){
    del_node(t, n->left);
    del_node(t, n->right);
    free(n);
  }
}

void delete_rbtree(rbtree *t) {
  del_node(t, t->root);
  free(t->nil);
  free(t);
}
void left_rotate(rbtree *t, node_t *target){
  node_t *target_R = target->right;
  target->right = target_R->left;
  if(target_R->left != t->nil){
    target_R->left->parent = target;
  }
  target_R->parent = target->parent;
  if (target->parent == t->nil){
    t->root = target_R;
  }else if( target == target->parent->left){
    target->parent->left = target_R;
  }
  else {
    target->parent->right = target_R;
  }
  target_R->left = target;
  target->parent = target_R;
}
void right_rotate(rbtree *t, node_t *target){
  node_t *target_L = target->left;
  target->left = target_L->right;
  if(target_L->right != t->nil){
    target_L->right->parent = target;
  }
  target_L -> parent = target->parent;
  if(target->parent == t->nil){
    t->root = target_L;
  }else{
    if(target->parent->right == target){
      target->parent->right = target_L ;
    }else{
      target->parent->left = target_L;
    }
  }
  target_L->right = target;
  target->parent = target_L;
}

void rb_insert_fix(rbtree *t, node_t *target){
  node_t *u;
  // parent is black
  // 아래의 반복문을 거치지 않고 root를 검은색으로 변경 후 함수 종료
  while(target->parent->color == RBTREE_RED){
    // grandparent's left node is parent
    if (target->parent == target->parent->parent->left){
      u = target->parent->parent->right;
      // case 1: uncle is red
      if (u->color == RBTREE_RED){
        u->color = RBTREE_BLACK;
        target->parent->color = RBTREE_BLACK;
        target->parent->parent->color = RBTREE_RED;
        target = target->parent->parent;
      // case 2: uncle is black with triangle
      }else{ 
        if(target->parent->right == target){
          target = target->parent;
          left_rotate(t,target);
        }
        // case 3: uncle is black with line
        target->parent->parent->color = RBTREE_RED;
        target->parent->color = RBTREE_BLACK;
        right_rotate(t, target->parent->parent);

      }
    // grandparent's right node is parent
    }else{
      u = target->parent->parent->left;
      // case 1: uncle is red
      if (u->color == RBTREE_RED){
        u->color = RBTREE_BLACK;
        target->parent->color = RBTREE_BLACK;
        target->parent->parent->color = RBTREE_RED;
        target = target->parent->parent;
      // case 2: uncle is black with triangle
      }else{ 
        if(target->parent->left == target){
          target = target->parent;
          right_rotate(t, target);
        }// case 3: uncle is black with line
        target->parent->parent->color = RBTREE_RED;
        target->parent->color = RBTREE_BLACK;
        left_rotate(t, target->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}
node_t *rbtree_insert(rbtree *t, const key_t key) {
    node_t *new_node = (node_t*)calloc(1, sizeof(node_t));
    node_t *parent = t->nil; // parent
    node_t *ptr = t->root; // temp
    // 아래 반복문을 통해 p에는 새로 노드를 삽입할 곳의 부모노드가 들어오게 된다.
    while(ptr != t->nil){
        parent = ptr;
        if(ptr->key > key){
            ptr = ptr->left;
        }else{
            ptr = ptr->right;
        }
    }
    new_node -> parent = parent;
    if (parent == t->nil){
      t->root = new_node;
    }else if(key < parent->key){
      parent->left = new_node;
    }else{
      parent->right = new_node;
    }
    new_node->left = t->nil;
    new_node->right = t->nil;
    new_node->key = key;
    new_node->color = RBTREE_RED;
    rb_insert_fix(t, new_node);
  return new_node;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *ptr;
  ptr = t->root;
  while(ptr != t->nil){
    if(ptr->key > key){
      ptr = ptr->left;
    }else if(ptr->key < key){
      ptr = ptr->right;
    }else{
      return ptr;
    }
  }
  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t *ptr;
  ptr = t->root;
  if(ptr != t->nil){
    while(ptr ->left != t->nil){
      ptr = ptr->left;
    }
  }
  return ptr;
}
node_t *node_min(const rbtree *t, node_t *n) {
  // TODO: implement find
  node_t *ptr;
  ptr = n;
  while(ptr ->left != t->nil){
    ptr = ptr->left;
  }
  return ptr;
}


node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *ptr;
  ptr = t->root;
  if(ptr != t->nil){
    while(ptr ->right != t->nil){
      ptr = ptr->right;
    }
  }
  return ptr;
}

void rb_transplant(rbtree *t, node_t *u, node_t *v){
  if(u->parent == t->nil){
    t->root = v;
  }else if(u == u->parent->left){
    u->parent->left = v;
  }else{
    u->parent->right = v;
  }
  v->parent = u->parent;
}

void rb_delete_fixup(rbtree *t, node_t *target){
  node_t *bro;
  while(t->root != target && target->color == RBTREE_BLACK){
    if(target->parent->left == target){
      bro = target->parent->right;
      // case1: uncle이 red
      if(bro->color == RBTREE_RED){
        target->parent->color = RBTREE_RED;
        bro->color = RBTREE_BLACK;
        left_rotate(t, target->parent);
        bro = target->parent->right;
      }
      // case2: uncle이 black이고 두 자식이 모두 black
      if(bro->left->color == RBTREE_BLACK && bro->right->color == RBTREE_BLACK){
        bro->color = RBTREE_RED;
        target = target->parent;
      }
      // case3: uncle이 black이고 왼쪽 자식은 red 오른쪽 자식은 black
      else{
        if(bro->right->color == RBTREE_BLACK){
          bro->color = RBTREE_RED;
          bro->left->color = RBTREE_BLACK;
          right_rotate(t, bro);
          bro = target->parent->right;
        }
        // case4: uncle이 black이고 오른쪽 자식이 red
        bro->color = target->parent->color;
        target->parent->color = RBTREE_BLACK;
        bro->right->color = RBTREE_BLACK;
        left_rotate(t, target->parent);
        target = t->root;
      }
    }else{
      bro = target->parent->left;
      // case1: uncle이 red
      if(bro->color == RBTREE_RED){
        target->parent->color = RBTREE_RED;
        bro->color = RBTREE_BLACK;
        right_rotate(t, target->parent);
        bro = target->parent->left;
      }
      // case2: uncle이 black이고 두 자식이 모두 black
      if(bro->right->color == RBTREE_BLACK && bro->left->color == RBTREE_BLACK){
        bro->color = RBTREE_RED;
        target = target->parent;
      }
      // case3: uncle이 black이고 오른쪽 자식은 red 왼쪽 자식은 black
      else{ 
        if(bro->left->color == RBTREE_BLACK){
          bro->color = RBTREE_RED;
          bro->right->color = RBTREE_BLACK;
          left_rotate(t, bro);
          bro = target->parent->left;
        }
        // case4: uncle이 black이고 왼쪽 자식이 red
        bro->color = target->parent->color;
        target->parent->color = RBTREE_BLACK;
        bro->left->color = RBTREE_BLACK;
        right_rotate(t, target->parent);
        target = t->root;
      }
    }
  }
  target->color = RBTREE_BLACK;
}
int rbtree_erase(rbtree *t, node_t *p) {
  if(p == NULL){
    return 0;
  }
  // TODO: implement erase
  // 삭제 진행
  node_t *target = p;
  node_t *ptr; // x
  color_t y_color = p->color;
  if(p->left == t->nil){
    ptr = p->right;
    rb_transplant(t, p, p->right);
  }
  else if(p->right == t->nil){
    ptr = p->left;
    rb_transplant(t, p, p->left);
  }else{
    target = node_min(t, p->right);
    // printf("node_min->key: %d\n", target->key);
    y_color = target->color;
    ptr = target->right;
    if(target ->parent == p){
      ptr->parent = target;
    }else{
      rb_transplant(t, target, target->right);
      p->right->parent = target;
      target->right = p->right;
    }
    rb_transplant(t, p, target);
    target->left = p->left;
    p->left->parent = target;
    target->color = p->color;
  }
  if(y_color == RBTREE_BLACK){
    rb_delete_fixup(t, ptr);
  }
  free(p);
  return 0;
}
void node_to_array(const rbtree *t, const node_t *n, key_t *arr, int *i, const size_t num){
  if (*i >= num || n == t->nil){
    return;
  }
  node_to_array(t, n->left, arr, i, num);
  arr[(*i)++] = n->key;
  node_to_array(t, n->right, arr, i, num);
}


int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  int i = 0;
  node_to_array(t, t->root, arr, &i, n);
  return 0;
}
