class Base:
    def __init__(self, name: str):
        self.name: str = name

    def greet(self) -> str:
        return "Hello, " + self.name

class Derived(Base):
    def __init__(self, name: str, age: int):
            if(age < 0):
                    print("not valid")

                super().__init__(name)
                self.age: int = age

    def greet(self) -> str:
        greeting: str = super().greet()
        return greeting + ", you are " + str(self.age) + " years old."

def factorial(n: int) -> int:
    if n == 0 or n == 1:
        return 1
    else:
        return n * factorial(n - 1)

def main():
    # Primitive data types and print
    my_int: int = 42
    my_float: float = 3.14
    my_str: str = "Compiler Test"
    my_bool: bool = True
    print(my_int)
    print(my_float)
    print(my_str)
    print(my_bool)

    # 1D list
    my_list: list = [1, 2, 3, 4, 5]
    print(my_list)

    # Basic operators
    sum: int = my_int + 5
    product: int = my_int * 2
    division: float = my_int / 2
    floor_div: int = my_int // 2
    modulo: int = my_int % 3
    exponent: int = my_int ** 2
    print(sum, product, division, floor_div, modulo, exponent)

    # Relational and logical operators
    is_equal: bool = my_int == 42
    is_not_equal: bool = my_int != 43
    is_greater: bool = my_int > 40
    and_operator: bool = is_equal and is_greater
    or_operator: bool = is_equal or is_not_equal
    not_operator: bool = not is_equal
    print(is_equal, is_not_equal, is_greater, and_operator, or_operator, not_operator)

    # Control flow
    if my_int > 40:
        print("Greater than 40")
    elif my_int < 40:
        print("Less than 40")
    else:
        print("Equal to 40")

    for i in range(5):
        print(i)

    i: int = 0
    while i < 5:
        print(i)
        i += 1

    # Recursion
    print("Factorial of 5: " + str(factorial(5)))

    # Classes, objects, and inheritance
    base_obj: Base = Base("Base User")
    derived_obj: Derived = Derived("Derived User", 30)
    print(base_obj.greet())
    print(derived_obj.greet())

if __name__ == "__main__":
    main()
