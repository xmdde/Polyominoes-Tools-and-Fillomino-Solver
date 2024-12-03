import os
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

def main():
    if len(sys.argv) != 2:
        print("Użycie: python script.py <nazwa_pliku>")
        sys.exit(1)

    input_file = sys.argv[1]

    if not os.path.isfile(input_file):
        print(f"Błąd: Plik '{input_file}' nie istnieje.")
        sys.exit(1)

    output_dir = "10x15"
    os.makedirs(output_dir, exist_ok=True)

    with open(input_file, 'r') as file:
        lines = file.readlines()

    start_number = 31  # Początkowy numer pliku

    for idx, line in enumerate(lines):
        line = line.strip()
        result = process_string(line)
        output_file = os.path.join(output_dir, f"{start_number + idx}.txt")
        with open(output_file, 'w') as out_file:
            out_file.write("10 15\n")
            out_file.write(result)

    print(f"Przetwarzanie zakończone. Wyniki zapisano w folderze '{output_dir}'.")

if __name__ == "__main__":
    main()
