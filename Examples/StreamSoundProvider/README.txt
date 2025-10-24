         Name: Streamed Sound Provider
       Status: Windows(Y), Linux(Y)
        Level: Advanced
   Maintainer: Core developers
      Summary: Example of writing your own streamed soundprovider.

Streamed soundproviders are used when there is a lot of data to play, and
loading it all into memory at once wouldn't be such a good idea. Instead of
loading something this provider generates the sound. 

This sample will generate a single "sine tone" for you to hear.
