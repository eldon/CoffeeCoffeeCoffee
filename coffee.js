$(document).ready(function () {

    $('#brew').on('click', function () {
        $('#brew-modal').modal('show');
    });

    $('#lets-brew').on('click', function () {
        var coffeeType = $('#coffee-input').val();
        if(coffeeType.length === 0){
            $('.form-group').addClass('has-error');
            return;
        }

        $('#coffee-type').text(coffeeType);

        var d = new Date();
        var madeAt = 'Made at ' + d.getHours() + ':' + d.getMinutes() + ' on ' + d.toLocaleDateString();
        $('#time-made').text(madeAt);

        $('.brew-button').addClass('hidden');
        $('.progress').removeClass('hidden');

        $('#brew-modal').modal('hide');
        progressBar();

        $.ajax({
            url: '/coffee',
            data: JSON.stringify({title: 'hallo', article: 'test'}),
            type: 'POST',
            success: function(response) {
                console.log('success');
                console.log(response);
            },
            error: function(error) {
                console.log('error');
                console.log(error);
            }
        });

    });

    $('#brew-modal').on('shown.bs.modal', function () {
        $('#coffee-input').focus();
    });

    var progressBar = function () {
        //assuming it takes 3 minutes (180000 ms)
        var endTime = 180000;
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
    }

    var coffeeReady = function () {
        $('#coffee-ready-modal').modal('show');
        setTimeout(function(){
            $('#coffee-ready-modal').modal('hide');
        }, 3000);
    }
});
