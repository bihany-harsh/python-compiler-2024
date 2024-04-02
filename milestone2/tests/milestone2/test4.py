i: int

def g(a: float) -> float:
    return a ** 2

class A():
    def __init__(self, b: int):
        self.b:int = b
    def f(self, a:int) -> int:
        self.x:float = 1.0 
        for a in range(1, 3 * 11):
            if a == 3:
                break
        while a < 3:
            if a == 2:
                continue
            print(5)
            a += 1
            g(a * 5)
        print(a)
        return a

def main():
    b: A = A(1 // 4 * 8)
    b.f(5)

if __name__ == "__main__":
  main()
