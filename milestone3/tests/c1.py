def binarySearch(array: list[int], x: int, low: int, high: int) -> int:

  # i: int
  # for i in range(7):
  #   z: int = array[i]
  #   print(z)

  # print(x)

  while low <= high:
    mid: int = (low + high) // 2
    # z: int = array[mid]
    print(mid)
    if array[mid] == x:
        return mid
    elif array[mid] > x:
        low = mid + 1
    else:
        high = mid - 1
  return -1

def bubble_sort(arr: list[int]) -> int:
  # n = len(arr)
  n: int = 7
  i: int
  for i in range(n):
      # print(i)
      swapped: int = 0
      j: int
      for j in range(0, n - i - 1):
          # print(j)
          temp: int
          if arr[j] > arr[j + 1]:
              temp = arr[j]
              arr[j] = arr[j + 1]
              arr[j + 1] = temp 
              swapped = 1
      if swapped == 0:
          break
  return 0

def gib_list()->list[int]:
  x: list[int] = [4, 1, 2, 6, 7, 7, 8]
  return x

class A():
  def __init__(self, a:int):
    self.a: int = a
    self.b:int = a * a

    # self.b: str = "abc"
  def f(self)->int:
    # self.c:int = 6
    array: list[int] = gib_list()
    y:int = bubble_sort(array)
    result: int = binarySearch(array, 4, 0, 7)
    # print("Hello")
    return result

def main():
  # print("abc")

  # array: list[int] = [3, 4, 5, 6, 7, 8, 9]
  # result: int = binarySearch(array, 5, 0, 7)
  # print(result)
  
  # x:int = array[5]
  # print(x)

  # if result != -1:
  #   print("Element is present at index:")
  #   print(result)
  # else:
  #   print("Element is not present")
  #   print(-1)
  array: list[int] = gib_list()

  i: int
  # for i in range(7):
  #   x: int = array[i]
  #   print(x)

  y:int = bubble_sort(array)

  # for i in range(7):
  #   x: int = array[i]
  #   print(x)
  result: int = binarySearch(array, 7, 0, 7)

  print(result)


# if __name__ == "__main__":
#   main()

