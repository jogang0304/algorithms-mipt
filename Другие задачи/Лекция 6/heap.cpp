/*
Биномиальная куча
- insert
- getMin
- extractMin
- decreaseKey
- merge (слияние 2 куч)
Биномиальное дерево:
В вершинах числа, удовлетворяющие требованию кучи
дерево ранга k -- 2 связанных за корень дерева ранга k-1
В дереве T_k ровно 2^k вершин

Биномиальная куча - набор биномиальных деревьев попарно различных рангов
Если всего в куче n элементов, то деревьев не больше чем log_2(n)
Глубина T_k = k
Хранятся ссылки на все кучи

merge:
  Сливание кучи с кучей (массивов деревьев)
  Если сливается с деревом ранга k, а такое дерево уже есть в куче, то они
объединяются в дерево ранга k+1

insert(x): O(logn)
  T_0 = {x}
  merge(T_0, heap)

decreaseKey(ptr, delta): O(logn)
  heap[ptr] -= delta
  siftUp(ptr)

getMin(): O(logn)
  min(getMin от всех деревьев) 
можно сделать за O(1), если хранить указатель на минимальный корень

extractMin(): O(logn)
  extractMin() в нужном дереве,
  оставшиеся (распавшиеся) деревья объединяем в новую кучу
  merge куча и новая куча



*/

#include <iostream>
#include <vector>

class BinaryHeap {
 private:
  std::vector<int> s;
  void siftUp(int i) {
    if (i <= 1) return;
    int p = i / 2;
    if (s[i] < s[p]) {
      std::swap(s[i], s[p]);
      siftUp(p);
    }
  }
  void siftDown(int i) {
    if (2 * i > s.size() - 1) return;
    int u = 2 * i;
    if (2 * i + 1 <= s.size() - 1 && s[2 * i + 1] < s[2 * i]) u = 2 * i + 1;
    if (s[u] < s[i]) {
      std::swap(s[u], s[i]);
      siftDown(u);
    }
  }

 public:
  BinaryHeap() { s.resize(1); }
  BinaryHeap(std::vector<int>& values) {
    s = values;
    for (int i = s.size() - 1; i >= 1; --i) {
      siftDown(i);
    }
  }
  int getMin() {
    if (s.size() > 1)
      return s[1];
    else
      return 0;
  }
  void extractMin() {
    if (s.size() > 1) {
      std::swap(s[1], s[s.size() - 1]);
      s.pop_back();
      siftDown(1);
    }
  }
  void insert(int x) {
    s.push_back(x);
    siftUp(s.size() - 1);
  }
  void decreaseKey(int i, int delta) {
    if (delta <= 0 || i < 1 || s.size() <= 1) return;
    s[i] -= delta;
    siftUp(i);
  }
  int size() { return s.size() - 1; }

  int operator[](int i) {
    if (i < 0 || s.size() <= 1) return 0;
    return s[i + 1];
  }
};

void HeapSort(std::vector<int>& values) {
  BinaryHeap heap(values);
  int v_index = 0;
  while (heap.size() > 0) {
    values[v_index++] = heap.getMin();
    heap.extractMin();
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
