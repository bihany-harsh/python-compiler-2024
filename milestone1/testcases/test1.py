import ast

string = """
a = 1
if a == 1:
    print("hello")
elif a == 2:
    print("elif")
else:
    print("else")

if a != 3:
    print("hello again")

"""

parsed_string = ast.parse(string)
print(ast.dump(parsed_string, indent=4))