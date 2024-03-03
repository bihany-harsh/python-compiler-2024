# This testcase contains an error that is appropriately reported
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

if __name__ == "__main__":
    main()
    print("Main has not been defined")
    a : int = 1
    b : int = 5
    c : float //= b ** (a + 1) - 2
    d : bool
    if (a * b - c) > c:
        d = True
    else:
        d = False
    if (d):
        print("nearing the end of the program")
    else:
        print("this execution was not very nice")

# Example usage
arr = [1,2,3,5,6]
print(arr.length())