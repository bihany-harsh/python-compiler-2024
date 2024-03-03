def merge_sort(arr):
    """
    Sorts an array in ascending order using the Merge Sort algorithm.
    """
    if len(arr) > 1:
        # Finding the mid of the array
        mid = len(arr) // 2

        # Dividing the array elements into 2 halves
        left_half = arr[:mid]
        right_half = arr[mid:]

        # Sorting the first half
        merge_sort(left_half)

        # Sorting the second half
        merge_sort(right_half)

        # Merging the sorted halves
        merge(arr, left_half, right_half)

def merge(arr, left_half, right_half):
    """
    Merges two halves sorted in ascending order to produce a single sorted array.
    """
    i = j = k = 0

    # Copy data to temp arrays left_half[] and right_half[]
    while i < len(left_half) and j < len(right_half):
        if left_half[i] < right_half[j]:
            arr[k] = left_half[i]
            i += 1
        else:
            arr[k] = right_half[j]
            j += 1
        k += 1

    # Checking if any element was left in left_half
    while i < len(left_half):
        arr[k] = left_half[i]
        i += 1
        k += 1

    # Checking if any element was left in right_half
    while j < len(right_half):
        arr[k] = right_half[j]
        j += 1
        k += 1

# Example usage
if __name__ == "__main__":
    arr = [12, 11, 13, 5, 6, 7]
    print("Given array is:", arr)
    merge_sort(arr)
    print("Sorted array is:", arr)
