/*
Динамическое дерево отрезков

Node:
  tl, tr
  sum
  Node* left
  Node* right

update(Node* v, pos, x): O(logn)
  v.sum += x
  if (v.tl == v.tr) return
  tm = (v.tl + v.tr) / 2
  if (pos <= tm)
    если левого сына нет, его надо создать
    update(v.left, pos, x)
  else
    update(v.right, pos, x)

getSum(default): O(logn)
  default



Персистентные структуры данных



*/