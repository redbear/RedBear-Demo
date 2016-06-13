var Particle = require('particle-api-js');
var particle = new Particle();
var token = 'xxxxxx'; // from result of particle.login
var func_name;
var func_arg;

process.argv.forEach(function (val, index, array) {
                     func_name = array[2];
                     func_arg = array[3];
                     console.log(`${index}: ${val}`);
                     });


var fnPr = particle.callFunction({ deviceId: 'xxxx', name: func_name, argument: func_arg , auth: token });

fnPr.then(
          function(data) {
          console.log('Function called succesfully:', data);
          }, function(err) {
          console.log('An error occurred:', err);
          });