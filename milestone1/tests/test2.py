def get_matrix_input(prompt):
    print(prompt)
    rows = int(input("Enter the number of rows: "))
    cols = int(input("Enter the number of columns: "))

    matrix = []
    print("Enter the rows of the matrix (space-separated values):")
    for _ in range(rows):
        row = list(map(int, input().split()))
        assert len(row) == cols, "Row does not match specified column count."
        matrix.append(row)
    
    return matrix

def validate_matrices_for_multiplication(matrix_a, matrix_b):
    # Number of columns in matrix_a must equal number of rows in matrix_b
    return len(matrix_a[0]) == len(matrix_b)

def matrix_multiply(matrix_a, matrix_b):
    if not validate_matrices_for_multiplication(matrix_a, matrix_b):
        raise ValueError("Matrices cannot be multiplied due to incompatible dimensions.")

    rows_a = len(matrix_a)
    cols_a = len(matrix_a[0])
    cols_b = len(matrix_b[0])

    result = [[0 for _ in range(cols_b)] for _ in range(rows_a)]

    for i in range(rows_a):
        for j in range(cols_b):
            for k in range(cols_a):
                result[i][j] += matrix_a[i][k] * matrix_b[k][j]

    return result

def print_matrix(matrix):
    print("Result:")
    for row in matrix:
        print(" ".join(map(str, row)))

def main():
    try:
        matrix_a = get_matrix_input("Enter the first matrix")
        matrix_b = get_matrix_input("Enter the second matrix")

        result = matrix_multiply(matrix_a, matrix_b)
        print_matrix(result)
    except ValueError as e:
        print(e)
    except AssertionError as e:
        print(e)

if __name__ == "__main__":
    main()
