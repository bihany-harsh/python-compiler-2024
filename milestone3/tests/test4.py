def binarySearch(array: list[int], x: int, low: int, high: int) -> int:
  while low <= high:
    mid: int = (low + high) // 2
    if array[mid] == x:
      return mid
    elif array[mid] < x:
      low = mid + 1
    else:
      high = mid - 1
  return -1

def BubbleSort(array: list[int], n: int) -> list[int]:
  i : int
  j : int
  temp : int
  for i in range(n):
    for j in range(n - i - 1):
      if array[j] > array[j + 1]:
        # Swap array j and j + 1
        temp = array[j]
        array[j] = array[j + 1]
        array[j + 1] = temp
  return array

def main():
  array: list[int] = [3, 6, 1, 4, 9]
  n: int = 5
  array = BubbleSort(array, n)
  result: int = binarySearch(array, 4, 0, n - 1)
  
  
  if result != -1:
    print("Element is present at index:")
    print(result)
  else:
    print("Element is not present")


if __name__ == "__main__":
  main()
