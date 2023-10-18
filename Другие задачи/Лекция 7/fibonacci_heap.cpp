#include <iostream>
#include <vector>

/*
Insert O(1)
GetMin O(1)
Merge O(1)
DecreaseKey O*(1)
ExtractMin O*(logn)

Куча - набор деревьев. Корни деревьев соединены в список (двусвязный список,
есть ссылки на самый левый и правый элементы) Ранг вершины - количество её детей


*/

/*
SparseTable

Есть массив a[1:n]
За запрос надо найти минимум (максимум) на [a[l], a[r]]. O(1)

Найдём ответы на всех подотрезках длины степени двойки. O(n logn)
При запросе найдём максимальную степень двойки (c), которая не больше r-l.
Рассмотрим два отрезка [l, l+c], [r-c, r]. Минимум на [l,r] = минимум из этих
двух отрезков. Получается ответ на запрос за O(1)

Sparse[k][i] - минимум на отрезке длины 2^k, начинающемся в a[i]
Пусть посчитаны все Sparse[k]
j = min(a.size, i + 2^k)
Sparse[k + 1][i] = min(Sparse[k][i], Sparse[k][j])



*/

class SparseTable {
  std::vector<int> elements;
  std::vector<std::vector<int>> sparse;
  int Pow_2(int power) { return 1 << power; }

  SparseTable(std::vector<int>& vec) {
    elements = vec;
    sparse.resize(0);
    sparse.push_back(elements);
    int max_k = 0;
    while (Pow_2(max_k) <= elements.size()) {
      ++max_k;
    }
    --max_k;
  }
};
