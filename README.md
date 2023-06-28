# Gameplan

We need 3 main things:

1. A client that accepts commands (repeatedly), sends them to the server and
   reports back messages from it.
2. A server that accepts a single connection from a client and manages a binary
   tree. It will receive 3 commands:
        * Insert
        * Delete
        * Find
   It will report any errors it encounters and the command results to the client.
3. A integer binary tree that has functions which insert, delete and search for
   values in it.

I will avoid using any external libraries other than libc and the C++ Standard
library. If we are pressed for time, we could use the network functions from the
boost library.

# Notes

* I should probably abstract the C function calls (especially network calls) to
  clasess. That way I don't have to see them too much.
* I won't write unit tests during prototyping, but once I start refactoring I
  will then write tests before functions.

# Bonus

* Multithreading. Should be pretty simple, each connection gets his own thread.
* Visualize the binary tree in a GUI or image. Should be an interesting
  challenge.
* Speaking of GUI's, maybe make one for the client (perhaps in the terminal?)
