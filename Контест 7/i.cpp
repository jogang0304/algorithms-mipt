// C-3PO — протокольный робот, в функционале которого заложена опция
// переводчика. Для этого C-3PO умеет на слух распознавать огромное множество
// языков, а далее находить нужную ему подпрограмму перевода с одного языка на
// другой и обратно. В его памяти есть N языков, пронумерованных от 1 до N.
// Поиск нужной подпрограммы прост, для перевода с i-го языка на j-й берется
// модуль с номером i + j. Если подпрограммы с таким номером нет, то C-3PO
// способен переводить речь на промежуточные языки, чтобы получить речь на
// нужном языке.

// После очередной передряги у него отказал модуль, сопоставляющий устной речи
// ее язык. Теперь он не может сопоставить языку его номер, а значит, не может
// больше быть переводчиком. Конечно, джедаев это не устраивает, поэтому они
// обратились к вам, самому талантливому юнглингу. Восстановите исходную
// нумерацию языков, зная, какие номера подпрограмм перевода у него были, к
// счастью, они оказались неповрежденными.

// Из документации к данной серии роботов вы узнали, что отсутствуют
// подпрограммы перевода с одного языка на тот же самый. Так же там написано,
// что C-3PO мог переводить с любого известного ему языка на любой другой, быть
// может через промежуточные языки.

// Для удобства работы с памятью C-3PO, языки уже занумеровали от 1 до N
// каким-то образом, то есть формально надо найти такую перестановку p[], что
// если p[i] = k, то i-й язык во входных данных имел номер k в изначально.

// Идея:  построить граф, где каждой вершине будет соответствовать пара
// {константа, коэффициент перед x_1}. У x_1 пара - {0, 1}. Тогда можно выразить
// каждый элемент через x_1. После этого можно просто пройтись по графу и найти
// циклы. После этого можно восстановить все элементы (если одному элементу
// соответствуют две разные пары). Если цикла нет, то находим вершину, которая
// может быть единицей, и восстанавливаем ответ от неё.

#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

const int NoneValue = -1;

struct Node {
  // int index = NoneValue;
  int constant = NoneValue;
  int x_coefficient = NoneValue;
};

bool operator==(const Node& left, const Node& right) {
  return (left.constant == right.constant) &&
         (left.x_coefficient == right.x_coefficient);
}

bool operator!=(const Node& left, const Node& right) {
  return !(left == right);
}

struct Edge {
  int weight = NoneValue;
  int start = NoneValue;
  int end = NoneValue;
};

bool operator<(const Edge& left, const Edge& right) {
  return left.weight < right.weight;
}

bool operator==(const Edge& left, const Edge& right) {
  return (left.weight == right.weight) && (left.start == right.start) &&
         (left.end == right.end);
}

std::vector<std::set<Edge>> graph;
std::vector<Node> nodes;
std::vector<int> answer;
int assumed_one_plus_coefficient = NoneValue;
int assumed_one_minus_coefficient = NoneValue;

void InitializeNodes(int start = NoneValue) {
  nodes.resize(graph.size());
  answer.resize(graph.size(), NoneValue);
  if (start == NoneValue) {
    start = 0;
    nodes[start] = {0, 1};
  }

  if (nodes[start].x_coefficient > 0 &&
      nodes[start].constant < assumed_one_plus_coefficient) {
    assumed_one_plus_coefficient = nodes[start].constant;
  }
  if (nodes[start].x_coefficient < 0 &&
      nodes[start].constant < assumed_one_minus_coefficient) {
    assumed_one_minus_coefficient = nodes[start].constant;
  }

  for (const auto& edge : graph[start]) {
    Node new_node = {edge.weight - nodes[start].constant,
                     -nodes[start].x_coefficient};
    if (nodes[edge.end].constant == NoneValue) {
      nodes[edge.end] = new_node;
      InitializeNodes(edge.end);
    } else if (new_node != nodes[edge.end]) {
      // found cycle
      answer[0] = (new_node.constant - nodes[edge.end].constant) /
                  (nodes[edge.end].x_coefficient - new_node.x_coefficient);
    }
  }
}

int should_add = 1;
bool CheckAndCalculateAnswers(int start, std::set<int>& used_numbers,
                              int default_set_answer_index = 0) {
  answer[start] = nodes[start].constant +
                  nodes[start].x_coefficient * answer[default_set_answer_index];
  bool ans = true;
  if (answer[start] <= 0 ||
      used_numbers.find(answer[start]) != used_numbers.end()) {
    if (answer[start] <= 0) {
      should_add = std::max(should_add, 1 - answer[start]);
    }
    ans = false;
  }
  used_numbers.insert(answer[start]);
  for (const auto& edge : graph[start]) {
    if (answer[edge.end] == NoneValue) {
      auto result = CheckAndCalculateAnswers(edge.end, used_numbers,
                                             default_set_answer_index);
      if (!result) {
        ans = result;
      }
    }
  }
  return ans;
}

void GuessAnswer(int amount_of_languages) {
  std::set<int> used;
  used.clear();
  answer.clear();
  answer.resize(amount_of_languages, NoneValue);
  answer[0] = 1 - assumed_one_plus_coefficient;
  auto result = CheckAndCalculateAnswers(0, used);
  if (result) {
    return;
  }

  used.clear();
  answer.clear();
  answer.resize(amount_of_languages, NoneValue);
  answer[0] = assumed_one_minus_coefficient - 1;
  result = CheckAndCalculateAnswers(0, used);
  if (result) {
    return;
  }

  for (int i = 1; i <= amount_of_languages; ++i) {
    used.clear();
    answer.clear();
    answer.resize(amount_of_languages, NoneValue);
    answer[0] = i;
    should_add = 1;
    auto result = CheckAndCalculateAnswers(0, used);
    i += should_add - 1;
    if (result) {
      break;
    }
  }
}

int main() {
  std::cin.tie(0);
  std::cout.tie(0);
  std::ios::sync_with_stdio(false);

  int amount_of_languages;
  int amount_of_programs;
  std::cin >> amount_of_languages >> amount_of_programs;
  graph.resize(amount_of_languages);
  for (int i = 0; i < amount_of_programs; ++i) {
    int start;
    int end;
    int value;
    std::cin >> start >> end >> value;
    --start;
    --end;
    graph[start].insert({value, start, end});
    graph[end].insert({value, end, start});
  }
  InitializeNodes();
  std::set<int> used;
  if (answer[0] != NoneValue) {
    CheckAndCalculateAnswers(0, used);
  } else {
    GuessAnswer(amount_of_languages);
  }

  for (int i = 0; i < amount_of_languages; ++i) {
    std::cout << answer[i] << ' ';
  }
}
