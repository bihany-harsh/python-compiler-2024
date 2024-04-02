class SimpleClass:
    def __init__(self, value: int) -> None:
        self.value:int = value

    def get_value(self) -> int:
        return self.value
    
    def print_value(self) -> None:
        print(self.value)

    def square(self) -> int:
        self.value = self.value ** 2
        return 1

def main():
    # Create a SimpleClass object
    obj: SimpleClass = SimpleClass(5)
    # Get the value of the object
    value: int = obj.get_value()
    # Print the value
    print(value)
    # Print the value using the object's method
    obj.print_value()
    obj.print_value()

    if(value > 5 and value < 10):
        print("Value is between 5 and 10")
        obj.square()
    else:
        print("Value is not between 5 and 10")
    

if __name__ == "__main__":
    main()
    main()
    main()
