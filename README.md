# Win32Messages

This is a sample used to investigate how Windows uses the window message pump.

The application can be started as "flooder" or as "receiver" using the two command lines `-flooder` or `-receiver`:

- as a receiver, the application will wait for synchronization and then will wait for a custome message to enter a "sleep mode" where the messages from the pump are not extracted
- as a flooder, the application will wait for synchronization and then can send a "blocking message" to the receiver to block it; then will flood its message pump with messages

the synchronization is performed through a piece of shared memory.
