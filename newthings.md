size_t :
<br>
- Unsigned integer type
- big enough to represent themaximum possible size of any object in memory. 
```
size_t bytes= sizeof(int);
```
It is used in case of memory sizes , array lengths, buffer capacities and object size. It is used because it is 64bit and default return type of many methods which return the size is of size_t
<br>
ssize_t:
<br>
- Signed integer type 
- A function returns the size on the success but returns -1 on error so we need signed one. 
```
ssize_t = read(f, buffer, size); 
```
<br>
sscanf:<br>
- read the formatted input from the string. similar to how scanf reads input from the standard keyboard. Essentially it parses data from a string according to the format. 

```
sscanf(const char *str, const char *format, ...)
```
... ->pointer to variables where the extracted values will be stored. <br>
<strong>Return value</strong>:
Returns the number of input items successfully matched and assigned. 
<br>
unit32_t:
- unsigned integer of exactly 32 bits.

memcpy():<br>
- It is a function used to copy block of memory from one loaction to another. 

```
void *memory(void *dest, const void *src, size_t n);
```
dest - pointer to the destination memory where data will be copied. <br>
src - pointer to the source memory from where data will be copied. <br>
n - number of bytes to copy

sizeof(((Struct*)0)->Attribute):<br>
(Struct*)0 = This casts the integer 0 to a pointer to Struct* so now we have a pointer to a struct but it points to address 0(NULL).
<br>
It is useful when we want the size of a struct member without creating a struct object. 


### Never thought I would use ruby to run the tests but it was good working with it. 
```
 def run_script(commands)
    raw_output = nil
    IO.popen("./db", "r+") do |pipe|
      commands.each do |command|
        pipe.puts command
      end

      pipe.close_write

      # Read entire output
      raw_output = pipe.gets(nil)
    end
    raw_output.split("\n")
  end
```
this is the function sort of thing in Ruby
<br> 
```
it 'inserts and retrieves a row' do
    result = run_script([
      "insert 1 user1 person1@example.com",
      "select",
      ".exit",
    ])
    expect(result).to match_array([
      "db > Executed.",
      "db > (1, user1, person1@example.com)",
      "Executed.",
      "db > ",
    ])
  end
```
this is how we make test cases in rspec.

```
int fd = open(filename, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);
```
This line opens a file and returns a file descriptor. This is system call in Linux/Unix. It returns an integer file descriptor. 
<br>
open():
```
Syntax:
open(pathname, flags, mode)
```
Flags are: <br>
O_RDWR- Open the file for read and write. 
<br>
O_CREAT: If the file doesnot exist, create it. Flags can be combined using |. 

mode: <br>
S_IWUSR: User has write permission. 
<br>
S_IRUSR: User has read permission.

lseek():
```
lseek(int fd, off_t offset, int whence);
```
fd= file descriptor(returned by open())<br>
offset = how many bytes to move.<br>
whence = reference point
<br>
SEEK_SET = Set the offset to offset bytes from the beginning of the file. 
<br>
SEEK_CUR = Set the offset to offset bytes from the current position. 
<br>
SEEK_END = Set the offset to offset bytes from the end of the file. 



