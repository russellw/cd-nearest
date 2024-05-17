Directory names are only locally unique. In the general case, `cd` requires a full path to unambiguously specify the destination. These things are necessarily true, but for many practical everyday uses, it is also true that the destination could be disambiguated with far fewer characters, and I am nostalgic for the days of Norton CD on MS-DOS, so I finally got around to doing something about it.

Of course on a modern system with tens of thousands of directories, it can't really work by scanning the whole hard disk the way NCD did. Instead, it keeps a list of directories you recently accessed, and chooses the first matching one. So you have to specify the full path the first time:

`c \MyProjects\FirstProject\Subdirectory`

But after that, you can often go back there with a much shorter command like:

`c su`

The command I chose is `c`, for maximum brevity; if you prefer to keep that reserved for something else, feel free to rename `c.bat` accordingly.

The batch file `c.bat` (the actual command) and the accompanying program `nearest.exe` (containing most of the logic) need to be placed somewhere in your path.

The batch file is Windows-specific. Pull requests with shell scripts for other operating systems are welcome. It would also be necessary to conditionally change the location of the recently-accessed list from `%USERPROFILE%\Documents\cd-nearest.csv` to something appropriate for each operating system.

ChatGPT 4o was used in writing this program. It was an interesting exercise in testing its capabilities given a simple but not entirely trivial specification. First, a Python version (I have found that to be the language ChatGPT is best at):

---
Q: Write a Python script for Windows, to print the nearest recent matching dir. That is:

The script keeps a list of directories the user has visited before. The list is stored in the user's Documents folder. It is called `cd-nearest.csv`. It is a simple text file, one entry per line. Each entry just consists of a full pathname of a directory. If the list does not already exist, an empty one is created.

The first thing the script does is read the current list.

Then it examines the command line argument. 

If the argument is an absolute pathname, then that is the directory to go to.

If it is a relative pathname, one that contains at least one `\`, then go to that directory relative to the current one.

Otherwise, the argument is taken as a prefix. Go through the list of previously visited directories, and take the first one that is matched, i.e. the commandline argument is a prefix (case insensitive) of the last component of the path.

Move the chosen directory to the top of the list.

Then print that directory.

A: Here's a Python script for Windows that accomplishes the described functionality. This script reads a list of previously visited directories from a file, processes a command-line argument to determine the target directory, and updates the list accordingly.

```python
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
        print("Usage: python script.py <directory>")
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
```

### Explanation
1. **Reading the Directory List**: The `read_directory_list` function reads the list of directories from `cd-nearest.csv`. If the file doesn't exist, it creates an empty one.
2. **Writing the Directory List**: The `write_directory_list` function writes the updated list of directories back to `cd-nearest.csv`.
3. **Finding a Matching Directory**: The `find_matching_directory` function looks for a directory in the list whose last component matches the prefix provided in the command-line argument.
4. **Main Function**:
   - Checks if the script is run with the correct number of arguments.
   - Reads the current list of directories from `cd-nearest.csv`.
   - Determines the target directory based on whether the argument is an absolute path, a relative path, or a prefix.
   - Moves the chosen directory to the top of the list.
   - Writes the updated list back to the file.
   - Prints the target directory.

### Usage
Run the script from the command line, providing a directory argument:
```shell
python script.py <directory>
```
- If `<directory>` is an absolute path, it will be used directly.
- If `<directory>` is a relative path, it will be resolved relative to the current directory.
- If `<directory>` is a prefix, it will find the first matching directory from the list of previously visited directories and use it.
---

Very good! The above worked first time.

There was a perceptible delay in running the program. Entirely understandable, runtime speed is not Python's biggest selling point, but for a command I use many times a day, it would be nice to have instant response.
