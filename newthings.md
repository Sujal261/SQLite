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
