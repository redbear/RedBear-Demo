angular.module('redbear.ibeaconDemo')
    .factory('ParticleCloud', function($window, $q) {

        var particle;

        if (!particle) {
            particle = new $window.Particle();
        } 
        return {    
            access_token: null,

            login: function(user) {

                var deferred = $q.defer();
                var _this = this;
                particle.login({ 
                    username: user.username,
                    password: user.password
                }).then(function(data) {

                    _this.access_token = data.body.access_token;
                    deferred.resolve({'status' : 'Access Token retrived'});
                }, function(err) {
                    deferred.reject(err);
                });

                return deferred.promise;


            },
            listDevices: function() {

                var deferred = $q.defer();

                particle.listDevices({
                    auth: this.access_token
                }).then(function(devices) {
                    
                    deferred.resolve(devices);
                }, function(err) {
                    deferred.reject(err);
                });

                return deferred.promise;
            },
          
            setIBeacon: function(deviceId, en, uuid) {

                var deferred = $q.defer();
                
                particle.callFunction({
                    deviceId: deviceId,
                    name: 'setIBeacon',
                    argument: JSON.stringify({
                        "en" : (en ? "1" : "0"),
                        "UUID" : uuid
                    }),
                    auth: this.access_token
                }).then(function(data) {
                   
                    deferred.resolve("OK");
                }, function(err) {
                    deferred.reject(err);
                });

                return deferred.promise;

            },
            setUIDEn: function(deviceId, en, uID) {

                var deferred = $q.defer();
        

                particle.callFunction({
                    deviceId: deviceId,
                    name: 'setUIDEn',
                    argument: JSON.stringify({
                       "en" : (en ? "1" : "0"),
                       "iID" : uID
                    }),
                    auth: this.access_token
                }).then(function(data) {
                   
                    deferred.resolve("OK");
                }, function(err) {
                    deferred.reject(err);
                });

                return deferred.promise;

            },
            setUIDName: function(deviceId, nID) {

                var deferred = $q.defer();
                
                particle.callFunction({
                    deviceId: deviceId,
                    name: 'setUIDName',
                    argument: JSON.stringify({
                        "nID" : nID
                    }),
                    auth: this.access_token
                }).then(function(data) {
                   
                    deferred.resolve("OK");
                }, function(err) {
                    deferred.reject(err);
                });

                return deferred.promise;

            },
            setURL: function(deviceId, en, url) {

                var deferred = $q.defer();
               

                particle.callFunction({
                    deviceId: deviceId,
                    name: 'setURL',
                    argument: JSON.stringify({
                        "en" : (en ? "1" : "0"),
                        "url" : url
                    }),
                    auth: this.access_token
                }).then(function(data) {
                   
                    deferred.resolve("OK");
                }, function(err) {
                    deferred.reject(err);
                });

                return deferred.promise;

            },
            setEID: function(deviceId, en, key) {

                var deferred = $q.defer();
                
                particle.callFunction({
                    deviceId: deviceId,
                    name: 'setEID',
                    argument: JSON.stringify({
                        "en" : (en ? "1" : "0"),
                        "key" : key
                    }),
                    auth: this.access_token
                }).then(function(data) {
                   
                    deferred.resolve("OK");
                }, function(err) {
                    deferred.reject(err);
                });

                return deferred.promise;

            },
            setInterval: function(deviceId, interval) {
                var deferred = $q.defer();
                
                particle.callFunction({
                    deviceId: deviceId,
                    name: 'setInterval',
                    argument: JSON.stringify({
                        "int" : interval
                    }),
                    auth: this.access_token
                }).then(function(data) {
                   
                    deferred.resolve("OK");
                }, function(err) {
                    deferred.reject(err);
                });

                return deferred.promise;
            }

        };



    });