def f()->None:
  print("static polymorphism")
def f(a : int)->int:
  return a << 2

def main():
  f()
  print(f(2))

if __name__ == "__main__":
  main()
