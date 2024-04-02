def add(a: int, b: int) -> int:
    return a + b

def add(a: float, b: float) -> float:
    return a + b

def multiply(a: int, b: int) -> int:
    return a * b

def multiply(a: float, b: float) -> float:
    return a * b

def is_positive(a: int) -> bool:
    return a > 0

def is_positive(a: float) -> bool:
    return a > 0.0

def coerce_to_float(a: int) -> float:
    return a

def coerce_to_int(a: bool) -> int:
    return a

def main() -> None:
    print("Int addition: ")
    print(add(3, 4))

    print("Float addition: ")
    print(add(3.5, 4.5))

    print("Int multiplication: ")
    print(multiply(3, 4))

    print("Float multiplication: ")
    print(multiply(3.5, 4.5))

    print("Is 5 positive?")
    print(is_positive(5))

    print("Is -3.5 positive?")
    print(is_positive(-3.5))

    print("Coerce 10 to float: ")
    print(coerce_to_float(10))

    print("Coerce True to int: ")
    print(coerce_to_int(True))

    print("Iterating over range(5):")
    i : int
    for i in range(5):
        print(i)

    print("Iterating over range(2, 5):")
    for i in range(2, 5):
        print(i)

if __name__ == "__main__":
    main()
