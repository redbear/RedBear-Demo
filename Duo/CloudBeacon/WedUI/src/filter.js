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
