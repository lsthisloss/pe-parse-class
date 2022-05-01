# pe-parse-class
Slution how to read import table from executable files.

First step: creating named shared memory with CreateFile() and open it for read, then mapping it with CreateFileMapping().
Secong step: get offset with field e_lfanew to pe header signature and initiate local variables
Third step: get entry point to DataDirectory and read import table using importDescriptor 
![image](https://user-images.githubusercontent.com/66689123/166144626-6c23350a-5cdc-4ac6-a6a5-1ad47d721a9d.png)
