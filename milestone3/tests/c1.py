def f(a: int, b: int, c:int, d:int, e: int, f: int, g: int)->int:
    y: int = a + b + c
    y = a + 2
    x: int = 5
    x = a
    x = 7 + 8
    # x = e*f + g * d + f*c
    # y = -x
    return y + x

def main() -> None:
    f(3, 4, 5, 6, 7, 8, 9)

if __name__ == "__main__":
    main()

