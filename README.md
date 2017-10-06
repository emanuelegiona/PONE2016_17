# PONE2016_17

## Multiplatform client/server file cypher application (C, POSIX API, WIN32 API)

This project consists in a multiplatform client/server file cipher application, where the client is installed in the "master" system and the server in the "slave" system.

## How to use it

- Run the included makefile (`make` and `make clean` commands both for Unix/Linux and Windows); this Windows makefile only works if Visual Studio is installed and is run from a Developer Command Prompt (where `cl.exe` is set up as the C/C++ compiler)

- starting from the same directory where the makefiles are in:

    - `/unix/userver.out` and `client.out` are the Unix/Linux executables
    
    - `wserver.exe` and `client.exe` are the Windows executables
    
- **Server commands**:

    - `-c path`: sets `path` as working directory, where the server will cipher the files from (necessary if `folder` parameter isn't specified inside `server.conf`)
    
    - `-p port`: uses `port` as port to listen commands from
    
    - `-n nThreads`: creates a thread pool with `nThreads` threads
    
    - `-h`: shows a help message (currently in Italian only)
    
- **Client commands**:
    
    - `-l`: lists all files in the server's working directory
    
    - `-R`: lists all files in the server's working directory and any subdirectory of it
    
    - `-e seed path`: ciphers the file `path` (including the working directory) using `seed` as seed
    
    - `-d seed path`: deciphers the file `path` using `seed` as seed
    
    - `-s path`: shows the latest seed used for file `path`
    
    - `-h`: shows a help message (currently in Italian only)
    
- **Config files**:

    - must be under the `misc` directory, which must be at the same level as the makefiles 
    
    - `server.conf` (any of this parameter is ignored if a command line option of the same kind is specified valid)
    
        - folder (necessary if `-c` is not specified)
    
        - port (default: 8888)
        
        - threadNumber (default: 1)
    
    - `client.conf` (necessary if the server is running from a different computer, otherwise the default values are used)
    
        - address (default: 127.0.0.1)
        
        - port (default: 8888)

### Notes

- You can find several log files in the `misc` directory:

    - `server.log`: contains all received commands, also listing which IP address they came from and mapping each of them to a request ID, in order to track the outcome of the command with it; additionally, there's the PID too if using the Unix/Linux implementation.
    
    - `seeds.log`: each row contains a file path and its relative seed used; it's updated if and only if the encryption is successful.
    
    - `error.log`: contains errors arose during I/O operations.

- Theoretically, there is no maximum supported file size; however, tests have run flawlessly with files up to 10 GB for the Unix/Linux implementation and up to 4 GB for the Windows implementation, omitting the remaining tests up to 10 GB, assuming there would be no problem whatsoever

- OpenMP is used during the cipher/decipher phase, while any other threaded operation makes use of native threads (POSIX or WIN32)

- Unix/Linux implementation runs as a daemon

### Known bugs

- Client file list (`-l` or `-R`) visualization fails, printing out quite a lot spaces instead of actual file paths; this happens when client and server applications run from different computers. This doesn't happen when both client and server applications run within the same computer.
