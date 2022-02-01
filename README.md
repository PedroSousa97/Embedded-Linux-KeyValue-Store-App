## Embedded Persistent Key-Value Storage Application

The application which I call KVStoreAPP, implements the following requirements:

- Developed in C++;
- Persistently store an arbitrary number of key-value pairs to disk, using LevelDB;
- Controlled via stdin/stdout (command line);
- Allows for it to be changed to be controlled over other interface, using IPC;
- Supports SET/GET/DELETE commands as shown below:
- Compiles and runs on any desktop Linux;
- Can be added as a package to an embedded Linux build for any ARM architecture (so part of the rootfs produced by your build system) using Buildroot. The Buildroot packages can be found inside the Buildroot Build folder. It can run in an emulated environment such as QEMU;

## Available Commands

- SET (key) (value) – This command is used to store a new Key-Value pair to the Database;
  If SET is called on an already existing key, it will override its content. This is useful when you need to change a value of a certain key.
- GET (key) – This command is used to get the value of the specified key from the Database. If the key doesn’t exist, an error message will be shown to the user informing that situation;
- DELETE (key) - This command is used to Key-Value pair of the specified key. If the key doesn’t exist, an error message will be shown to the user informing that situation;
- (IPC) – This command is used to enable the IPC Execution Mode, the KVStoreAPP starts behaving as a Server, and will only accept requests to the Database from a Client.
  After entering this mode, only the Client can close the connection and make the KVStoreAPP go back to Command Line Execution Mode;
- (EXIT) – Command used to close the Database, exit and close the application;

## KVSTORE APP BUILD INSTRUCTION

Dependency management is hard and engineers prefer to grab a project, configure, build and run in as little steps as possible and without polluting our systems.

For that same reason, I tried to simplify the project build as much as possible, and for that reason I have created two bash script files that do the following:

- Build.sh - Script to automate and manage the project dependencies. It detects and installs all needed dependencies, you just need to approve the installations. At the end it will build the executable needed to run the KVStoreAPP application (./kvstoreapp to execute it);
- removeDependencies.sh - Script to automate and manage the project dependencies removal. It deletes the previously installed dependencies for this project, if you actually wish to remove them. Some of the dependencies may already been previously installed and you may not wish to uninstall them.

Note: If “Permission Denied” Error appears, just use chmod 777 followed by the name of the script file, inside the KVStoreAPP, and it should allow execution.

As simple as that! Regarding the Client Application, to build it, open a terminal in the Client directory and type “make”. No dependencies in this case, they are already included in the Build.sh script.

Other important thing to remind is that this project only has three dependencies, and two of them are generic libraries used to build C++ projects (make and g++):

- libleveldb-dev : LevelDB is a fast key-value storage library written at Google that provides an ordered mapping from keys to values;
- make : Make enables the end user to build and install your package without knowing the details of how that is done;
- g++ : C++ compiler;

Note2: Both shell script were tested on Ubuntu and should work just fine, if they don’t, please install the dependencies individually and then execute the “make” in both KVStoreAPP and Client folders to build both application executables.

## Execute KVStoreAPP and Client App

To execute the KVStoreAPP, after building it do the following:
1- Open a terminal inside the KVStoreAPP folder;
2- Type: ./kvstoreapp

To execute the Client APP, after building it do the following:
3- Open a terminal inside the Client folder;
4- Type: ./clientapp

Note: Client App is a seperate app that I developed used to test the IPC capabilities of KVStoreAPP. The scripts won't build this application automatically, to build it you have to open a terminal inside the Client folder, and run the make command.

## Author

Pedro Henrique Santos Sousa
