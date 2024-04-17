def binarySearch(array: list[int], x: int, low: int, high: int) -> int:
  while low <= high:
    mid: int = low + high // 2
    if array[mid] == x:
      return mid
    elif array[mid] < x:
      low = mid + 1
    else:
      high = mid - 1
  return -1

# def fact(x: int) -> int:
#     # z: int = x + y
#     # print(x)
#     if x == 1:
#       return 1
#     else:
#       return x * fact(x - 1)

# class A:
#   def __init__(self, a: int):
#     self.a: int = a


def main():
  array: list[int] = [3, 4, 5, 6]
  result: int = binarySearch(array, 4, 0, 7)  
  
  # if result != -1:
  #   # print("Element is present at index:")
  #   print(result)
  # else:
  #   # print("Element is not present")
  #   print(-1)

  # x: int = array[1] + 2

  # a: A = A(1)

  # print(a.a)

  # 3 = 4

  # array[1] += 4

  # print(array[1])

  # x: int = fact(5)
  # print(x)
  # print(array[1])
  # z :int = add(56, 44)
  # print(add(56, 44) + 2)

# if __name__ == "__main__":
#   main()

