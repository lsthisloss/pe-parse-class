# pe-parse-class
Slution how to read import table from executable files.

First step: creating named shared memory with CreateFile() and open it for read, then mapping it with CreateFileMapping().
Secong step: get offset with field e_lfanew to pe header signature and initiate local variables
Third step: get entry point to DataDirectory and read import table using importDescriptor 

Example:
![image](https://user-images.githubusercontent.com/66689123/166145162-39436d5d-9aaf-40f8-95dc-69d863d2850b.png)
