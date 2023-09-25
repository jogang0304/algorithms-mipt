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
