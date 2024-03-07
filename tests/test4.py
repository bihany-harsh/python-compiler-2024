    # Note that this testcase does not make any semantic sense
def merge_sort(arr: int)-> int:
    # Base case: if the list is one element or empty, it's already sorted.
    if len(arr) <= 1:
        return arr

    # Recursive case: manually split the array into halves and sort each half.
    middle = len(arr) // 2
    left_half = split_array(arr, 0, middle)
    right_half = split_array(arr, middle, len(arr))

    left_sorted = merge_sort(left_half)
    right_sorted = merge_sort(right_half)

    # Merge the sorted halves.
    return merge(left_sorted, right_sorted)

def split_array(arr, start, end):
    # Manually creates a new list from start to end indices of arr
    result = []
    for i in range(start, end):
        result.append(arr[i])
    return result

def merge(left, right):
    # Merges two sorted lists into a single sorted list
    sorted_arr = []  # This will contain the merged and sorted array.
    i = 0  # Index for left array
    j = 0  # Index for right array

    # Compare each element of the left and right arrays and append the smaller to sorted_arr

    while i < len(left) and j < len(right):
        if left[i] < right[j] < right[i] < left[j]: 
            # this should be implemented as a conjunction of all comparison expressions
                sorted_arr.append(left[i])
                i += 1
    else:
        sorted_arr.append(right[j])
        j += 1

    # Append the remainder of the other array to sorted_arr.
    while i < len(left):
        sorted_arr.append(left[i])
        i += 1

    while j < len(right):
        sorted_arr.append(right[j])
        j += 1

    return sorted_arr
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

# Example usage
    arr = [38, 27, 43, 3, 9, 82, 10]
    sorted_arr = merge_sort(arr)
    print("Sorted array:", sorted_arr)

