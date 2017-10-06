# PONE2016_17

## Multiplatform client/server file cypher application (C, POSIX API, WIN32 API)

This project consists in a multiplatform client/server file cipher application, where the client is installed in the "master" system and the server in the "slave" system.

## How to use it

- Run the included makefile (`make` and `make clean` commands both for Unix/Linux and Windows); this Windows makefile only works if Visual Studio is installed and from a Developer Command Prompt (where `cl.exe` is set up as the C/C++ compiler)

- starting from the same directory where the makefiles are in:

    - /unix/userver.out and client.out are the Unix/Linux executables
    
    - wserver.exe and client.exe are the Windows executables
    
- Server commands:

    - `-c`
    
    - `-p`
    
    - `-n`
    
    - `-h`
    
- Client commands:
    
    - `-l`
    
    - `-R`
    
    - `-e seed path`
    
    - `-d seed path`
    
    - `-s`
    
    - `-h`

### Notes

- Theoretically, there is no maximum supported file size; however, tests have run flawlessly with files up to 10 GB for the Unix/Linux implementation and up to 4 GB for the Windows implementation, omitting the remaining tests up to 10 GB, assuming there would be no problem whatsoever

- OpenMP is used during the cipher/decipher phase, while any other threaded operation makes use of native threads (POSIX or WIN32)

- Unix/Linux implementation runs as a daemon

### Known bugs

- Client files visualization fails, printing out quite a lot spaces instead of actual file paths; this happens when client and server applications run from different computers. This doesn't happen when both client and server applications run within the same computer.
