class Person:
    def __init__(self, name: str, age: int) -> None:
        self.name:str = name
        self.age:int = age

    def introduce(self) -> None:
        print("Name: ", self.name)
        print("Age: ", self.age)

class Employee(Person):

    def __init__(self, name: str, age: int, employee_id: int) -> None:
        self.employee_id:int = employee_id
        self.name = name
        self.age = age

    def introduce(self) -> None:
        print("Employee ID: ", self.employee_id)

class Manager(Employee):

    def __init__(self, name: str, age: int, employee_id: int, department: str) -> None:
        self.name = name
        self.age = age
        self.employee_id = employee_id
        self.department: str = department

    def introduce(self) -> None:
        print("Department: ", self.department)

def main() -> None:
    person: Person = Person("John Doe", 30)
    employee: Employee = Employee("Jane Doe", 28, 1001)
    manager: Manager = Manager("Alex Smith", 35, 1002, "Engineering")

    print("Introducing Person:")
    person.introduce()

    print("\nIntroducing Employee:")
    employee.introduce()

    print("\nIntroducing Manager:")
    manager.introduce()

if __name__ == "__main__":
    main()
