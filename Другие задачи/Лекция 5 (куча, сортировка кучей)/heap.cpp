/*
Куча S:
- insert (x): добавить x в S
- getMin: узнатить минимум S
- extractMin: удалить минимум из S
- decreaseKey (ptr, delta)

Реализация бинарной кучи:
S: массив a_1, a_2, ..., a_n
a_1 - корень. Дети: a_2, a_3
У a_2 дети a_4, a_5
У a_3 дети a_6, a_7
...
У a_k дети a_2k, a_2k+1

На таком дереве выполняется требование кучи:
a_n <= любого его потомка

getMin за O(1)
остальное за O(logn)


Вспомогательные процедуры:
1. SiftUp(i):
  (i == 1) return
  родитель элемента с индексом i - a[i/2]
  если (a[i] < a[i/2]) swap(a[i], a[i/2]); SiftUp(i/2)
2. siftDown(i):
  (2i > n) return - нет детей, некуда опускать
  u = 2i
  (2i+1 <= n && a[2i+1] < a[2i]) u = 2i+1
  (a[u] < a[i]) swap(a[u], a[i]); siftDown(u)

extractMin():
  swap(a[1], a[n])
  n -= 1
  siftDown(1)

insert(x):
  n += 1
  a[n] = x
  SiftUp(n)

decreaseKey(i, delta > 0):
  a[i] -= delta
  SiftUp(i)


*/

#include <iostream>
#include <vector>

class BinaryHeap {
 private:
  std::vector<int> s;
  void SiftUp(int i) {
    if (i <= 1) return;
    int p = i / 2;
    if (s[i] < s[p]) {
      std::swap(s[i], s[p]);
      SiftUp(p);
    }
  }
  void SiftDown(int i) {
    if (2 * i > s.size() - 1) return;
    int u = 2 * i;
    if (2 * i + 1 <= s.size() - 1 && s[2 * i + 1] < s[2 * i]) u = 2 * i + 1;
    if (s[u] < s[i]) {
      std::swap(s[u], s[i]);
      SiftDown(u);
    }
  }

 public:
  BinaryHeap() { s.resize(1); }
  BinaryHeap(std::vector<int>& values) {
    s.resize(values.size() + 1);
    for (int i = 1; i < s.size(); ++i) {
      s[i] = values[i - 1];
    }
    for (int i = s.size() - 1; i >= 1; --i) {
      SiftDown(i);
    }
  }
  int GetMin() {
    if (s.size() > 1)
      return s[1];
    else
      return 0;
  }
  void ExtractMin() {
    if (s.size() > 1) {
      std::swap(s[1], s[s.size() - 1]);
      s.pop_back();
      SiftDown(1);
    }
  }
  void Insert(int x) {
    s.push_back(x);
    SiftUp(s.size() - 1);
  }
  void DecreaseKey(int i, int delta) {
    if (delta <= 0 || i < 1 || s.size() <= 1) return;
    s[i] -= delta;
    SiftUp(i);
  }
  int Size() { return s.size() - 1; }

  int operator[](int i) {
    if (i < 0 || s.size() <= 1) return 0;
    return s[i + 1];
  }
};

void HeapSort(std::vector<int>& values) {
  BinaryHeap heap(values);
  int v_index = 0;
  while (heap.Size() > 0) {
    values[v_index++] = heap.GetMin();
    heap.ExtractMin();
  }
}

int main() {
  std::vector<int> values = {1, 2, -12, 0, 53, 62, 2, 53, -1232, -12};
  for (auto el : values) {
    std::cout << el << " ";
  }
  std::cout << "\n";
  HeapSort(values);
  for (auto el : values) {
    std::cout << el << " ";
  }
}
