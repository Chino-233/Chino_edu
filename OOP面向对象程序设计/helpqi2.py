for i in range(1, 10):
    j = 1
    while j <= i:
        print(f"{j} * {i} = {i * j}", end="\t")
        j += 1
    print()
i = 0
print("------------------")
while i <= 9:
    i=i+1
    j = 1
    while j <= 10 - i:
        print(f"{j} * {10 - i} = {j * (10 - i)}", end="\t")
        j += 1
    print()
