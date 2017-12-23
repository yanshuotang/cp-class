# ABOUT cp-class
a library written by c language,for parsing the standard java class byte codes  
C语言实现的一个解析JAVA class字节码的库。

# build
you can mkdir build package in the project root path.  
then cd build.  
excute cmake..  
then excute make  
the cp-class static library will be found in "cp-class/build/src/cp_class"  
进入项目根目录/cp-class  
新建build目录。mkdir build  
在build目录下执行CMAKE编译命令:cmake ..  
然后在继续执行命令:make  
项目即编译完成。  
cp-class的静态库就生成了："cp-class/build/src/cp_class"  

# example
when you finsh the project building.  
the example runtime file can be found int "cp-class/build/src/example"  
you can cd to this package and excute ./example  
make sure you have edit the class file in right path.  
当完成上面的编译工作后，项目自带的实例的可执行文件也已经生成了。  
你可以在"cp-class/build/src/example"目录下找到。  
执行命令 ./example  
注意，这时候如果报未找到class文件请编辑示例代码的class文件所在目录。  
cp-class/src/exmple/main.c/main::ubuntu_path  

## LICENSE
[BSD](LICENSE)