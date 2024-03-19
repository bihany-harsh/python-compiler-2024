global x
x = 1

def f():
    global x
    x = 3
    print(x)

f()
print(x)
