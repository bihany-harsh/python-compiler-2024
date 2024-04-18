class A():
  def __init__(self, a: int, b: int):
    self.a: int = a
    self.b: int = b
  

def listA()->list[A]:
  A1: A = A(1, 2)
  A2: A = A(2, 3)
  list_obj: list[A] = [A1, A2]
  return list_obj


def main():

  list_obj : list[A] = listA()

  list__ : list[int] = [1, 2, list_obj[1].b]

  x : int
  a : int
  x = a = 1 << 2 ** 2
  c : str
  d : str
  c = d = "abc"

  i : int
  for i in range(10):
    print(i)
    if i > 5:
      break
  print("")
  while 4 < i < 8:
    print(i)
    i -= 1
  print(c)
  print(a)


if __name__ == "__main__":
  main()
