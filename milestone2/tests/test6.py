class Demo():

    def __init__(self, a: int, b: str) -> None:
        self.a : int = a
        self.b : str = b

    def f(self) -> None:
        c: int = 1
        while c:
            d: int = 0
            c = c - 1

if __name__ == "__main__":
    a : int = 1
