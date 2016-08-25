const net = require('net');
const readline = require('readline');

// Parse Arguments
var args = process.argv.slice(2);


if (args.length == 2) {
    var ip = args[0];
    var port = args[1];

    var client = net.connect(port, ip, function() {

        var rl = readline.createInterface(process.stdin, process.stdout);
        rl.setPrompt('Command (on, off, or close) > ');
        rl.prompt();
        rl.on('line', function(cmd) {
            if (cmd === "on")  {
                client.write(new Buffer([1]));
            }
            else if (cmd == "off") { // off

                client.write(new Buffer([0]));
            }
            else if (cmd == "close") {
                rl.close();
            }
            else {
                console.log("Unknown Command");
            }
            rl.prompt();
        }).on('close',function(){
            process.exit(0);
        });

    });
    
    client.on('close', function() {
        console.log("Connection Closed");
    })
   
}
else {
    console.log("Usage (ex.): npm run start --ip=192.168.1.1 --port=8888");
}
