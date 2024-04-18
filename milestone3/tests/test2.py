class A():
  def __init__(self, a: int):
    self.a: int = a
    self.b: int = a ** 2

  def result(self)-> None:
    print(self.b)
  
class B(A):
  def __init__(self, a: int):
    self.a : int = a
    self.b : int = a ** 2
    self.c: int = a ** 3

  def result(self)-> None:
    print(self.c + self.b)

def main():
  a : A = A(2)
  b : B = B(3)
  a.result()
  b.result()
  print(a.b)
  print(b.c)

if __name__ == "__main__":
  main()
