# SQLite in C

This repository contains a simple, lightweight clone of a SQLite-like database, written from the ground up in C. It demonstrates fundamental database concepts by implementing a B-Tree data structure for storage, a pager for managing memory and disk I/O, and a command-line REPL for interaction.

The database supports basic `INSERT`, `SELECT`, and `UPDATE` operations on a single table with a predefined schema (`id`, `username`, `email`).

## Features

- **B-Tree Data Structure:** Data is organized in a B-Tree to provide efficient insertions, searches, and ordered retrieval.
- **Persistent Storage:** All data is saved to a disk file, ensuring durability between sessions.
- **REPL Interface:** A simple Read-Eval-Print Loop (REPL) allows for interactive command execution.
- **SQL-like Operations:** Supports `INSERT`, `SELECT`, and `UPDATE` statements.
- **Meta-Commands:** Includes helper commands like `.btree` to visualize the tree structure, `.constants` to inspect internal memory layout, and `./exit` to safely close the database.

## Getting Started

### Prerequisites

You will need a C compiler, such as `gcc`, to build the project.

### Compilation

Clone the repository and compile the source files using the following command:

```bash
gcc src/*.c -o sqldb
```

This will create an executable file named `sqldb` in the root directory.

### Usage

Run the program by providing a filename for your database. If the file does not exist, it will be created.

```bash
./sqldb mydatabase.db
```

This will launch the REPL, where you can execute commands.

#### Example Session

```
$ ./sqldb mydatabase.db
db> insert 1 ram ram@example.com
Executed
db> insert 2 sita sita@example.com
Executed
db> select
(1, ram, ram@example.com)
(2, sita, sita@example.com)
Executed
db> .btree
Tree:
- leaf (size 2)
 - 1
 - 2
db> update 1 sita_gita sita_git@example.com
Executed
db> select
(1, sita_gita, sita_gita@example.com)
(2, sita, sita@example.com)
Executed
db> ./exit
```

## Commands

The database supports the following commands.

### SQL Statements

- **`insert <id> <username> <email>`**
  - Inserts a new row with the specified data. The `id` must be a positive integer.
  - Example: `insert 3 some some@example.com`

- **`select`**
  - Retrieves and prints all rows from the database in ascending order of their `id`.

- **`update <id> <new_username> <new_email>`**
  - Modifies the `username` and `email` for the row with the specified `id`.
  - Example: `update 3 some some@example.com`

### Meta-Commands

Meta-commands are prefixed with a dot (`.`).

- **`./exit`**
  - Flushes all data from memory to the database file and exits the program.

- **`.btree`**
  - Prints a visual representation of the internal B-Tree structure, showing keys and node hierarchy.

- **`.constants`**
  - Displays internal constants, such as node and page sizes, providing insight into the memory layout.

## Testing

The repository includes a test suite written in Ruby using RSpec.

**Prerequisites:**
- Ruby
- RSpec (`gem install rspec`)

To run the full test suite, execute the `rspec` command from the root directory:

```bash
rspec