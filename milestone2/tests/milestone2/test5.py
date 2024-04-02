# Global list declaration
values: list[int] = [10, 20, 30, 40, 50]
size: int = 5  # Static size of the list
i : int

def sum_list() -> int:
    total: int = 0
    for i in range(size):  # Use the global size
        total += values[i]
    return total

def find_max() -> int:
    max_value: int = values[0]
    for i in range(1, size):  # Use the global size
        if values[i] > max_value:
            max_value = values[i]
    return max_value

def reverse_list() -> None:
    for i in range(size // 2):
        # Swap the elements using a temporary variable
        temp: int = values[i]
        values[i] = values[size - 1 - i]
        values[size - 1 - i] = temp

def main() -> None:
    print("Original list:")
    for i in range(size):  # Iterating using the global size
        print(values[i])

    print("\nSum of the list:")
    print(sum_list())

    print("\nMaximum value in the list:")
    print(find_max())

    reverse_list()
    print("\nReversed list:")
    for i in range(size):
        print(values[i])

if __name__ == "__main__":
    main()
