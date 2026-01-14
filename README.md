# SQLite 
The structure we will follow to make our SQLite Database is:

![structure](images/structure.png)

### Frontend: 
A query goes through the chain of components to retrieve, modify and insert the data. The frontend has:
1. Tokenizer 
2. Parser 
3. Code Generator
The output of the code generator is bytecode which is passed in virtual machine. 
### Backend
It gets the virtual machine bytecode(this is a compiled program that can operate on the database). Backend consist of:
1. Virtual Machine 
2. B-Tree 
3. Pager 
4. OS Interface
<br>

We can think Virtual machine as big switch statement on the type of bytecode instruction. 
<br> 
#### We will start with making REPL 
- From the REPL we use ./exit to get out of the terminal. 
- insert : to insert the values for now  id name and email
- select: to select the values from the table. 

#### What we will do to store the rows. 
- Store the rows in blocks of memory called pages. 
- Each page stores as many rows as it can fit. 
- Rows are serialized into a compact representation with each page. 
- Keep a fixed size of array of pointers to pages.

The layout of a serialized row look likes: 

|column|size(bytes)|offset|
|------|-----------|------|
|id|4|0|
|username|32|4|
|email|255|36|
|total|291||

### B-Tree
The B-tree is the data structure SQLite uses to represent both tables and indexes. Characteristics of B-Tree:
1. Searching for a particular value is fast(logarithmic time). 
2. Inserting / deleting a value we have already found is fast. (constant time to rebalance).
3. Traversing a range of values is fast(unlike a hash map).
<br>

|For m B Tree|Internal node|Leaf node|
|------------|-------------|---------|
|stores|keys and pointers to m children|keys and values|
|number of pointers|number of keys+1|none|
|number of values|none|number of keys|
|key purpose|used for routing|paired with value|
|stores values|no|yes|

Why do we stores the table as Trees?
<br>
Pages can contain metadata, primary keys and data which helps for indexes.Insertion happens in O(nlogn). Deletion also happens in O(nlogn). Lookup by id happens in O(nlogn). Duto these reasons we use Trees as our primary data structure to stores the keys. 



