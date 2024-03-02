def bubbleSort(array: list[int]) -> None:
  i: int = 0
  for i in range(len(array)):
    swapped: bool = False
    for j in range(0, len(array) - i - 1):
      if array[j] > array[j + 1]:
        temp: int = array[j]
        array[j] = array[j + 1]
        array[j + 1] = temp
        swapped = True
    if not swapped:
      break


def main():
  data: list[int] = [-2, 45, 0, 11, -9]
  bubbleSort(data)

  print('Sorted Array in Ascending Order:')
  i: int = 0
  for i in range(len(data)):
    print(data[i])


if __name__ == "__main__":
  main()
