# miniTar
## Description
Tar-like applicataion for zipping UNIX files with Huffman compression.  
All attributes, rights, and types of files are saved.
## Usage 
-c / --create: create mini-tar  
-x / --extract: extract mini-tar  
-d / --directory <path>: directory which you want to zip (or unzip).
  Default - current directory. Directory must exist.   
After flags the only argument is result filename
### Examples:  
./mini_tar -c -d input/ archive.mTar  
./mini_tar --directory output/ --extract archive.mTar  
