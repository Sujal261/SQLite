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


