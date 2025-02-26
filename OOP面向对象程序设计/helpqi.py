for i in range(1, 10):
    for j in range(1, i + 1):
        print(f"{j} * {i} = {i * j}", end="\t")
    print()
i=0
print("------------------")
for i in range(1,10):
    for j in range(1, 11-i):
        print(f"{j} * {10-i} = {j * (10-i)}", end="\t")
    print()