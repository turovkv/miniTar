# miniTar
## Description
Tar-like applicataion for zipping UNIX files with Huffman compression.  
All attributes, righst, and types of files is saved.
## Usage 
-c / --create: create mini-tar  
-x / --extract: extract mini-tar
-d / --directory <путь>: directory which you want to zip (or unzip). Default - current directory. Must exist  
After flags the only argument is result filename
### Examples:  
./mini_tar -c -d input/ archive.mTar  
./mini_tar --directory output/ --extract archive.mTar  
