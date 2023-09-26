def f():
    a = list(map(int, input().split()))
    b = list(map(int, input().split()))
    k = int(input())

    # max за n
    countValuesA = [0 for i in range(max(a) + 1)]
    # цикл за n
    for x in a:
        countValuesA[x] += 1
    # max за m
    countValuesB = [0 for i in range(max(b) + 1)]
    # цикл за m
    for x in b:
        countValuesB[x] += 1
    answer = 0
    # цикл за n
    for x in a:
        answer += countValuesB[k - x]

    print(answer)
    # 3n + 2m = O(n + m)


# решение получше
def g():
    a = list(map(int, input().split()))
    b = list(map(int, input().split()))
    k = int(input())

    i = 0
    j = len(b) - 1
    answer = 0
    while i < len(a) and j >= 0:
        if a[i] + b[j] == k:
            answer += 1
            if j > 0:
                j -= 1
            else:
                i += 1
            # нет более сложных проверок, потому что массивы строго возрастающие, то есть нет одинаковых элемментов
        elif a[i] + b[j] < k:
            i += 1
        else:
            j -= 1
    print(answer)

g()
