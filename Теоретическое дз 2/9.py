import random


def f(random_list: list[int]):
    copied_list = random_list.copy()
    for i in range(1, len(copied_list)):
        swaps_for_this_i = 0
        while i != copied_list[i] and swaps_for_this_i <= len(copied_list):
            a = copied_list[i]
            b = copied_list[copied_list[i]]
            copied_list[i] = b
            copied_list[a] = a
            swaps_for_this_i += 1
    ans = 0
    for i in range(1, len(copied_list)):
        if i != copied_list[i]:
            ans = copied_list[i]
            break
    return (ans, copied_list)


n = 100
counter = 1
while True:
    random_list = [random.randint(1, n - 1) for i in range(n + 1)]
    random_list[0] = 0
    ans = f(random_list)
    if random_list.count(ans[0]) > 1:
        print(f"ok {counter}")
        counter += 1
    else:
        print(f"error: {random_list}, ans: {ans}")
        break

# f([0, 3, 4, 2, 1, 4])
