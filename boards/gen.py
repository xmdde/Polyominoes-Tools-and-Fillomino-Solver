import sys

def process_string(input_string):
    if input_string.startswith(","):
        input_string = "0" + input_string
    if input_string.endswith(","):
        input_string = input_string + "0"
    elements = input_string.split(',')
    elements = [element if element else '0' for element in elements]
    result = ' '.join(elements)
    return result

if len(sys.argv) < 2:
    sys.exit(1)

input_string = sys.argv[1]
output_string = process_string(input_string)
print(output_string)