def binarySearch(array: list[int], x: int, low: int, high: int) -> int:
  print("in bs")
  while low <= high:
    mid: int = (low + high) // 2
    if array[mid] == x:
      print("done 2")
      return mid
    elif array[mid] < x:
      low = mid + 1
    else:
      high = mid - 1
  print("done")
  return -1

# def f(a : int, b: int) -> int:
#   return a + b

class A():
  def __init__(self, a: int, b: int):
    self.a: int = a
    self.b: int = b
    self.sum: int = a + b
  def f(self)->None:
    print(self.sum)
    print("hello")

def main():
  array: list[int] = [3, 4, 5, 6, 7, 8, 9]
  result: int = binarySearch(array, 4, 0, 7) 

  print("Before starting the function")
  print(result) 

  if result != -1:
    print("Element is present at index:")
    print(result)
  else:
    # print("Element is not present")
    # print("After the function call")
    print(-1)

  print(19999)
  print("this is another string")

  a:A = A(5, 6)
  a.f()

  print("After the function call")
  print("hello hehre")

  if (a.sum == 11):
    print("Sum is correct")
  else:
    print("Sum is incorrect")
  
  print("After the check")

  while result > 0:
    print("inside loop")
    result -= 1
  
  print("here now")
  x: int = 10

  c: str = "harsh"
  b: str = "bihany"
  # print(f(1,2))
  print(b)

  if c <= b:
    print("wowo")
  else:
    print("no")
