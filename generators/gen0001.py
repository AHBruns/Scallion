import random

f = open("gen0001_output/test.scallion", "w+")
operations = ["+", "-", "*", "//"]
for test in range(0, 1000):  # gen 1000 tests
    s = str(random.randint(1, 10))
    for token in range(0, 10):  # each test consists of 100 operations
        s += operations[random.randint(0, len(operations)-1)]
        s += str(random.randint(1, 10))
    f.write(s + "\n")
f.close()