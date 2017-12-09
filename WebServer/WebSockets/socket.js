const PORT = 8000;
const SUBPROTOCOL = 'sub-protocol';

var WebSocketServer = require('websocket').server;

var https = require('https');
var fs = require('fs');
var net = require('net');

// Private key and certification (self-signed for now) 
var options = {
	key: fs.readFileSync("/etc/letsencrypt/live/davidkopala.com/privkey.pem"),
	cert: fs.readFileSync("/etc/letsencrypt/live/davidkopala.com/fullchain.pem")
};

// callback function is called only when localhost:8000 is accessed via https protocol
var server = https.createServer(options, function(request, response) {
    // it sends 404 response so browser stops loading, otherwise it keeps loading 
    console.log((new Date()) + ' Received HTTP(S) request for ' + request.url);
    response.writeHead(404);
    response.end();
}); 

// bind server object to listen to PORT number
server.listen(PORT, function() {
    console.log((new Date()) + ' Server is listening on port ' + PORT);
});

var netClient = new net.Socket();
netClient.connect(15003, '127.0.0.1', function() {
	console.log("Connected to streaming server");
	netClient.write("_ FROM WEB SOCKET SERVER");
});

wsServer = new WebSocketServer({
    httpServer: server,
    // You should not use autoAcceptConnections for production
    // applications, as it defeats all standard cross-origin protection
    // facilities built into the protocol and the browser.  You should
    // *always* verify the connection's origin and decide whether or not
    // to accept it.
    autoAcceptConnections: false
});


function originIsAllowed(origin) {
  // put logic here to detect whether the specified origin is allowed.
  return true;
}

// If autoAcceptConnections is set to false, a request event will be emitted
// by the server whenever a new WebSocket request is made
wsServer.on('request', function(request) {

    if (!originIsAllowed(request.origin)) {
      // Make sure we only accept requests from an allowed origin
      request.reject();
      console.log((new Date()) + ' Connection from origin ' + request.origin + ' rejected.');
      return;
    }

    // accepts connection and return socket for this connection
    var connection = request.accept(SUBPROTOCOL, request.origin);

    console.log((new Date()) + ' Connection accepted.');

    // when message is received
    connection.on('message', function(message) {

        // echo
        connection.send(connection, message.utf8Data);
	console.log(message.utf8Data);
	netClient.write(message.utf8Data);
    });


    connection.on('close', function(reasonCode, description) {
        console.log((new Date()) + ' Peer ' + connection.remoteAddress + ' disconnected.');
    });

});
