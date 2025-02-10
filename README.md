## About this repository
This is for those who want to use DLL file of [HOSHIN CAMAC crate controller CCP-F and CCP-USB](https://www.kagaku.com/hoshin/camac.html) with the GNU C compiler.  
I do not have any responsibility for any problems that may occur with your equipment using these program.

## Usage
You should use these files with `HECMUSB.DLL`.   
Read the sample program for more detail.

## About error
I found that when I used `HECMUSB.DLL` to handle CCP-USB with the GNU C compiler, the return address of raw DLL function was invalid and the execution file did not work.  
This error may occur because of the compiler of `HECMUSB.DLL` may not be the GNU C compiler.  
These codes like `asm("sub $0x4, %esp");` are for rearranging the return address by calling back assembly.
These numbers like `$0x4` were debugged with GDB and found in my environment.  
Some of the less used functions have not been researched yet.  
You should debug and find your own difference between true return address and its practice in case you found that this program does not work.
I hope your debugging will be succeessful.
