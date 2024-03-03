def validate_matrices(matrix_a, matrix_b):
    # Check if matrices are not empty and have proper structure
    if not matrix_a or not matrix_b:
        print("Error: One of the matrices is empty.")
        return False
    
    rows_a = len(matrix_a)
    cols_a = len(matrix_a[0])
    rows_b = len(matrix_b)
    cols_b = len(matrix_b[0])

    # Check for uniformity in matrix dimensions
    for row in matrix_a:
        if len(row) != cols_a:
            print("Error: Matrix A is not uniform.")
            return False
    for row in matrix_b:
        if len(row) != cols_b:
            print("Error: Matrix B is not uniform.")
            return False

    # Check if multiplication is possible
    if cols_a != rows_b:
        print("Error: Matrices cannot be multiplied due to incompatible dimensions.")
        return False

    return True

def matrix_multiply(matrix_a, matrix_b):
    # Validate matrices before proceeding
    if not validate_matrices(matrix_a, matrix_b):
        return None

    # Initialize the result matrix with zeros
    rows_a = len(matrix_a)
    cols_b = len(matrix_b[0])
    result = []
    for i in range(rows_a):
        result_row = []
        for j in range(cols_b):
            result_row.append(0)
        result.append(result_row)

    # Perform the multiplication
    for i in range(rows_a):
        for j in range(cols_b):
            cell_sum = 0
            for k in range(len(matrix_a[0])):
                cell_sum += matrix_a[i][k] * matrix_b[k][j]
            result[i][j] = cell_sum

    return result

def print_matrix(matrix):
    # Function to print matrix in a formatted way
    if matrix is None:
        print("Matrix is None, cannot print.")
        return
    for row in matrix:
        print_row = ""
        for element in row:
            print_row += "{:4}".format(element)
        print(print_row)

# Example usage
matrix_a = [
    [1, 2, 3],
    [4, 5, 6]
]

matrix_b = [
    [7, 8],
    [9, 10],
    [11, 12]
]

print("Matrix A:")
print_matrix(matrix_a)
print("\nMatrix B:")
print_matrix(matrix_b)

result = matrix_multiply(matrix_a, matrix_b)
print("\nResult of Matrix A multiplied by Matrix B:")
print_matrix(result)
