# User-Authentication-System-using-Berkeley-Socket-API

#About
I have developed this authentication system as part of my 3rd year module Client Serving Programming. I have also uploaded the assignment info given by the lecturer as .pdf.</br>
The authClient.c part was given by our lecturer and we had to create the server part.</br>
I hosted the client on a virtual machine 1 and the server on a virtual machine 2.</br>
The user(client side), gets 3 chances to enter their right username or password(which should be admin and pass).</br>
A different message will get displayed, depending on the number of attempts, if the credentials were correct or if the user ran out of chances.</br>
Afterwards, the server closes the connection with the client.</br>

#More in-depth info
The server creates a socket for incoming connections, constructs a local address structure and then binds to the socket.</br>
It then marks the socket to listen for incoming connections. I have limited the total number of connections to be 5.</br>
I then wrote an infinite loop which will accept any incoming suggestions. After the connection is accepted, I have created a while loop, which will run as long as there's 3 attempts.</br>
The server side is looking for "\r\n\r\n" in order to stop receiving data. We use sscanf in order to check the receive buffer for the username and password.</br>
We then perform basic checks (if username==admin etc) to check what message to send back to the client (eg: welcome, denied entry etc).</br>
All the messages we send back to the client must contain "\r\n", (in order to mark the end of our message to the client side, that's what it's expecting).
