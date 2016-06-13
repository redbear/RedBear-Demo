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