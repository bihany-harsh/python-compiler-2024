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

class A():
  def __init__(self, a : int, b : int):
    self.a : int = a
    self.b : int = b
  def add(self) -> list[int]:
    a : list[int] = [1,2,3]
    a[1] += 5
    self.c : int = self.a + self.b
    return a

def f() -> int:
  qw: int = 10
  return qw

def f(x : int) -> list[int]:
  if x:
    a : list[int] = [1,2,3]
    return a
  else:
    b : list[int] = [4,5,6]
    return b

# class A:
#   def __init__(self, a: int):
#     self.a: int = a


def main():
  # array: list[int] = [3, 4, 5, 6]
  a : A = A(1, 2)
  dsaad : list[int] = f(5)
  dsaad[1] //= 4
  # result: int = binarySearch(array, 4, 0, 7) 
  asdsad: list[int] = a.add()
  asdsad[1] = a.b * 10
  # a : int = 10
  # print(f())
  
  # array[1] = True * False
  # array[1] += 2

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

