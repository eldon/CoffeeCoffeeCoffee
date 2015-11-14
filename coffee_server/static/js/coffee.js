$(document).ready(function () {

    $('#brew').on('click', function () {
        $('#brew-modal').modal('show');
    });

    $('#brew-modal').on('shown.bs.modal', function () {
        $('#coffee-input').focus();
    });

    $('#lets-brew').on('click', function () {
        var coffeeType = $('#coffee-input').val();
        if(coffeeType.length === 0){
            $('.form-group').addClass('has-error');
            return;
        }

        $('#coffee-type').text(coffeeType);

        var d = new Date();
        var minutes = d.getMinutes();
        if(minutes < 10) {
            minutes = '0'+minutes
        }

        var madeAt = 'Made at ' + d.getHours() + ':' + minutes;
        $('#time-made').text(madeAt);

        $('.brew-button').addClass('hidden');
        $('.progress').removeClass('hidden');

        $('#brew-modal').modal('hide');
        progressBar();

        $.ajax({
            url: '/coffee/start',
            type: 'GET',
            success: function(response){
                console.log('success');
            }
        });

    });

    $('#off').on('click', function (){
        $.ajax({
            url: '/coffee/stop',
            type: 'GET',
            success: function(response){
                console.log('success');
            }
        });
    });


    var progressBar = function () {
        //assuming it takes 10 minutes (600000 ms)
        var endTime = 600000;
        var time = endTime * 0.01; //so we have a bit of green at the start
        var intervalTime = 50;
        var bar = $('.progress-bar');

        var interval = setInterval(function () {
            if(time >= endTime){
                bar.width('100%');
                $('.brew-button').removeClass('hidden');
                $('.progress').addClass('hidden');
                clearInterval(interval);
                coffeeReady();
            }
            time += intervalTime;
            bar.width((time/endTime)*100 + '%');

        }, intervalTime);
    };

    var coffeeReady = function () {
        $('#coffee-ready-modal').modal('show');
        setTimeout(function(){
            $('#coffee-ready-modal').modal('hide');
        }, 3000);
    };

    var getStats = function () {
        var statsInterval = setInterval(function () {
            $.ajax({
                url: '/coffee',
                type: 'GET',
                success: function(response) {
                    $('#temperature').text(Math.round(response.temp));
                    $('#fill-level').text(response.fill * 100 + '%');
                    if(response.status){
                        $('#status').text('On');
                    } else {
                        $('#status').text('Off');
                    }
                },
            });
        }, 1000);
    }();
});
