angular.module('redbear.ibeaconDemo', ['ui.router'])
    .run(function() {
        console.log('app atart');

    })
    .config(function($stateProvider, $urlRouterProvider) {
        $urlRouterProvider.otherwise('/login');

        $stateProvider
            .state('login', {
                url: '/login',
                templateUrl: 'templates/login.html',
                controller: 'loginCtrl'
            })
            .state('controlPanel', {
                url: '/cbcontrol',
                templateUrl: 'templates/cbcontrol.html',
                controller: 'beaconCtrl'
            });
    })
angular.module('redbear.ibeaconDemo')
    .constant('config', {
        // Put any config here
    });
angular.module('redbear.ibeaconDemo')
    .controller('loginCtrl', function($scope, ParticleCloud, $state) {

        // User scope
        $scope.user = {
            username: '',
            password: ''
        };

        // login Function
        $scope.login = function() {

            ParticleCloud.login($scope.user)
                .then(function(data) {
                    $state.go('controlPanel');
                }, function(err) {
                
                    alert(err.errorDescription);
                });
        };


    })
    .controller('beaconCtrl', function($scope, ParticleCloud, $state, $filter) {
        if (ParticleCloud.access_token === null) {
            $state.go('login');
        }
        else {
            $scope.user = {
                selectedDevice: null,
                devices: [],
                status: 'waiting for device'
            };

            $scope.beaconConfig = {
                ibeacon: {
                    UUID: '64C871727D7E7580B2DED572E11A0000',
                    en: true
                },
                eddyStone: {
                    UID0: '100',
                    UID1: '120',
                    UID2: '130',
                    UID3: '222',
                    UID4: '225',
                    UID5: '226',
                    name: '726564626561726C6162',
                    urlscheme: '00',
                    url: 'redbear.cc',
                    urlencoding: '',
                    en: true,
                    enURL: true,
                    enEIDString: true,
                    key: "RedBearLab"
                },
                interval: "5000"

            };

            ParticleCloud.listDevices().then(
                function(data) {
                   
                    $scope.user.devices = $filter('connected')(data.body);
                    if ($scope.user.devices.length > 0) {
                        $scope.user.selectedDevice = $scope.user.devices[0].id;
                        $scope.user.status = "Device Ready";
                    }
                },
                function(err) {
                    alert(err.errorDescription);
                }
            );

            $scope.$watch('user.selectedDevice', function(device) {
                console.log(device);
            })

            // Set iBeacon UUID
            $scope.setiBeacon = function() {
                var param = {
                    deviceId: $scope.user.selectedDevice,
                    uuid: $scope.beaconConfig.ibeacon.UUID,
                    en: $scope.beaconConfig.ibeacon.en
                };

                $scope.user.status = 'Updating iBeacon...';

                if (param.deviceId && param.uuid != '') {
                    ParticleCloud.setIBeacon(param.deviceId, param.en, param.uuid)
                        .then(function(success) {
                            $scope.user.status = 'Updated iBeacon Configuration';
                        }, function(err) {
                            $scope.user.status = 'Error! Cannot update!';
                        });
                }
                else {
                    $scope.user.status = 'Error! Invalid setting!';
                }
            }

            // Set EddyStone UID
            $scope.setESId = function() {
                var param = {
                    deviceId: $scope.user.selectedDevice,
                    uid: [ 
                        $scope.beaconConfig.eddyStone.UID0,
                        $scope.beaconConfig.eddyStone.UID1,
                        $scope.beaconConfig.eddyStone.UID2,
                        $scope.beaconConfig.eddyStone.UID3,
                        $scope.beaconConfig.eddyStone.UID4,
                        $scope.beaconConfig.eddyStone.UID5],
                    en: $scope.beaconConfig.eddyStone.en
                };

                $scope.user.status = 'Updating EddyStone UID...';

                if (param.deviceId) {
                    ParticleCloud.setUIDEn(param.deviceId, param.en, param.uid)
                        .then(function(success) {
                            $scope.user.status = 'Updated EddyStone Configuration';
                        }, function(err) {
                            $scope.user.status = 'Error! Cannot update!';
                        });
                }
                else {
                    $scope.user.status = 'Error! Invalid setting!';
                }
            }


            // Set EddyStone Name Instance
            $scope.setESName = function() {
                var param = {
                    deviceId: $scope.user.selectedDevice,
                    nID: $scope.beaconConfig.eddyStone.name
                };

                 $scope.user.status = 'Updating EddyStone Name...';

                if (param.deviceId && param.nID != '') {
                    ParticleCloud.setUIDName(param.deviceId, param.nID)
                        .then(function(success) {
                            $scope.user.status = 'Updated EddyStone Configuration';
                        }, function(err) {
                            $scope.user.status = 'Error! Cannot update!';
                        });
                }
                else {
                    $scope.user.status = 'Error! Invalid setting!';
                }
            }

            // Set URL String
            $scope.setESUrl = function() {
                var newURL = $scope.beaconConfig.eddyStone.urlscheme + 
                              $filter('urlEncode')($scope.beaconConfig.eddyStone.url) +
                              $scope.beaconConfig.eddyStone.urlencoding;
                var param = {
                    deviceId: $scope.user.selectedDevice,
                    url: newURL,
                    en: $scope.beaconConfig.eddyStone.enURL
                };

                 $scope.user.status = 'Updating EddyStone URL...';

                if (param.deviceId && param.url != '') {
                    ParticleCloud.setURL(param.deviceId, param.en, param.url)
                        .then(function(success) {
                            $scope.user.status = 'Updated EddyStone Configuration';
                        }, function(err) {
                            $scope.user.status = 'Error! Cannot update!';
                        });
                }
                else {
                    $scope.user.status = 'Error! Invalid setting!';
                }
            }


            // Set EID String
            $scope.setEIDString = function() {
                var param = {
                    deviceId: $scope.user.selectedDevice,
                    key: $scope.beaconConfig.eddyStone.key,
                    en: $scope.beaconConfig.eddyStone.enEIDString
                };

                 $scope.user.status = 'Updating EddyStone EID String...';

                if (param.deviceId && param.key != '') {
                    ParticleCloud.setEID(param.deviceId, param.en, param.key)
                        .then(function(success) {
                            $scope.user.status = 'Updated EddyStone Configuration';
                        }, function(err) {
                            $scope.user.status = 'Error! Cannot update!';
                        });
                }
                else {
                    $scope.user.status = 'Error! Invalid setting!';
                }
            }

            // Set Interval
            $scope.setInterval = function() {

                var param = {
                    deviceId: $scope.user.selectedDevice,
                    interval: $scope.beaconConfig.interval
                };

                $scope.user.status = 'Updating Interval...';

                if (param.deviceId && param.interval != '') {
                    ParticleCloud.setInterval(param.deviceId, param.interval)
                        .then(function(success) {
                            $scope.user.status = 'Updated Interval Configuration';
                        }, function(err) {
                            $scope.user.status = 'Error! Cannot update!';
                        });
                }
                else {
                    $scope.user.status = 'Error! Invalid setting!';
                }
            }



        }


    });
angular.module('redbear.ibeaconDemo')
    .filter('connected', function() {

        return function(input) {

            input = input || [];
            var out = [];
            for (var i=0; i < input.length; i++) {
                
                if (input[i].connected) {
                    out.push(input[i]);
                }
            }
            return out;
        }
    })
    .filter('urlEncode', function() {

        return function(input) {

            input = input || '';
            var out = '';
            for (var i=0; i < input.length; i++) {

                out = out + input.charCodeAt(i).toString(16);

            }
            return out;

        }

    })

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