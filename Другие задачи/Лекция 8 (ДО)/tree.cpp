/*
Дерево отрезков

Задача: на массиве a[0:n]:

update(pos, x): O(logn)
  a[pos] += x

getSum(l, r): O(logn)
  sum(a[l:r])


Реализация:

Массив tree[]
Корень tree[1]
tree[i] хранит сумму на контролируемом подотрезке
Дети tree[v]: tree[2v], tree[2v + 1]
Если tree[v] контролирует [tl:tr], то tm = (tl + tr) / 2.
tree[2v] контролирует [tl:tm]
tree[2v + 1] контролирует [tm + 1:tr]

Все вершины имеют номера от 1 до 4n не включительно.

Update(v, tl, tr, pos, x):
  if tl == tr: - дошли до листа (исходного элемента из a)
    tree[v] += x
    return
  tm
  if pos <= tm:
    Update(2v, tl, tm, pos, x)
  else:
    Update(2v + 1, tm + 1, tr, pos, x)

GetSum(v, tl, tr, l, r):
  if (tl == l && tr == r):
    return tree[v]
  int ans = 0;
  tm
  if l <= tm:
    ans += GetSum(2v, tl, tm, l, min(tm, r))
  if r >= tm + 1:
    ans += GetSum(2v + 1, tm + 1, tr, max(tm + 1, l), r)


Отложенные операции:
Добавление x на отрезке:
Если все элементы в контроллируемом отрезке надо увеличить, то в вершине кроме
суммы сохраняем x.


*/

#include <algorithm>
#include <iostream>
#include <vector>

class SumTree {
 private:
  std::vector<int> tree;
  int array_size;

 public:
  SumTree(size_t array_size)
      : tree(4 * array_size, 0), array_size(array_size) {}
  SumTree(std::vector<int>& array) : SumTree(array.size() + 1) {
    for (size_t i = 0; i < array.size(); ++i) {
      Add(i, array[i]);
    }
  }

  void Add(int pos, int value, int tree_index = 1, int node_left = 0,
           int node_right = -1) {
    if (node_right == -1) {
      node_right = array_size;
    }
    if (node_left > node_right) {
      return;
    }
    if (node_left == node_right) {
      tree[tree_index] += value;
      return;
    }
    int tm = (node_left + node_right) / 2;
    if (pos <= tm) {
      Add(pos, value, tree_index * 2, node_left, tm);
    } else {
      Add(pos, value, tree_index * 2 + 1, tm + 1, node_right);
    }
    tree[tree_index] = tree[tree_index * 2] + tree[tree_index * 2 + 1];
  }

  // void AddBulk(int pos_left, int pos_right, int value, int tree_index = 1,
  //              int node_left = 0, int node_right = -1) {
  //   if (node_right == -1) {
  //     node_right = tree.size();
  //   }
  //   if (node_left > node_right) {
  //     return;
  //   }
  //   if (pos_left + 1 == node_left && pos_right + 1 == node_right) {
  //     tree[tree_index] += value;
  //     return;
  //   }
  //   int node_middle = (node_left + node_right) / 2;
  //   if (pos_left + 1 <= node_middle) {
  //     AddBulk(pos_left, std::min(pos_right, node_middle - 1), value,
  //             tree_index * 2, node_left, node_middle);
  //   }
  //   if (pos_right + 1 >= node_middle + 1) {
  //     AddBulk(std::max(pos_right, node_middle), pos_right, value,
  //             tree_index * 2 + 1, node_middle + 1, node_right);
  //   }
  //   tree[tree_index] = tree[tree_index * 2] + tree[tree_index * 2 + 1];
  // }

  int GetSum(int l, int r, int tree_index = 1, int node_left = 0,
             int node_right = -1) {
    if (node_right == -1) {
      node_right = array_size;
    }
    if (node_left == l + 1 && node_right == r + 1) {
      return tree[tree_index];
    }
    int tm = (node_left + node_right) / 2;
    int ans = 0;
    if (l <= tm) {
      ans += GetSum(l, std::min(tm, r + 1), tree_index * 2, node_left, tm);
    }
    if (r >= tm + 1) {
      ans += GetSum(std::max(tm + 1, l + 1), r, tree_index * 2 + 1, tm + 1,
                    node_right);
    }
    return ans;
  }

  void Print() {
    for (size_t i = 0; i < tree.size(); ++i) {
      std::cout << tree[i] << " ";
    }
    std::cout << std::endl;
  }
};

int main() {
  std::vector<int> start{2, 5, 7, 1, 6, 8};
  SumTree t(start);
  t.Print();
  t.Add(1, 10);
  t.Print();
}
