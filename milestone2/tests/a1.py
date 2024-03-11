class Base:
    def __init__(self, name: str) -> None:
        self.name = name

    def greet(self) -> str:
        return "Hello, from the Base class!"
class Derived(Base):
    def __init__(self, name: str, age: int) -> None:
        super().__init__(name)
        self.age = age

# Creating an object of the Base class
base_obj: Base = Base("Alice")

# Creating an object of the Derived class
derived_obj: Derived = Derived("Bob", 30)
# Creating a list of integers with type annotation
int_list: list[int] = [1, 2, 3, 4, 5]
