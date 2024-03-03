import ast

string = """
a ** 2
"""

parsed_string = ast.parse(string)
print(ast.dump(parsed_string, indent=4))