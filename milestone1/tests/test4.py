def merge_sort(arr):
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

# Example usage
arr = [38, 27, 43, 3, 9, 82, 10]
sorted_arr = merge_sort(arr)
print("Sorted array:", sorted_arr)
