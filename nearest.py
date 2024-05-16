import os
import sys
import csv

def read_directory_list(file_path):
    if not os.path.exists(file_path):
        with open(file_path, 'w') as f:
            pass  # Create an empty file if it doesn't exist
        return []
    with open(file_path, 'r') as f:
        reader = csv.reader(f)
        return [row[0] for row in reader]

def write_directory_list(file_path, dir_list):
    with open(file_path, 'w', newline='') as f:
        writer = csv.writer(f)
        for dir in dir_list:
            writer.writerow([dir])

def find_matching_directory(argument, dir_list):
    for dir in dir_list:
        if os.path.basename(dir).lower().startswith(argument.lower()):
            return dir
    return None

def main():
    if len(sys.argv) != 2:
        print("Usage: nearest <directory>")
        return

    argument = sys.argv[1]
    documents_folder = os.path.join(os.path.expanduser("~"), "Documents")
    list_file = os.path.join(documents_folder, "cd-nearest.csv")

    # Read the current list of directories
    dir_list = read_directory_list(list_file)

    if os.path.isabs(argument):
        target_dir = argument
    elif '\\' in argument:
        target_dir = os.path.abspath(argument)
    else:
        target_dir = find_matching_directory(argument, dir_list)
        if target_dir is None:
            print(f"No matching directory found for prefix: {argument}")
            return

    # Move the chosen directory to the top of the list
    if target_dir in dir_list:
        dir_list.remove(target_dir)
    dir_list.insert(0, target_dir)

    # Write the updated list back to the file
    write_directory_list(list_file, dir_list)

    # Print the target directory
    print(target_dir)

if __name__ == "__main__":
    main()
