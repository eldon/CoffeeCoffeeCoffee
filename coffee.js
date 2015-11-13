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
    });

    $('#brew-modal').on('shown.bs.modal', function () {
        $('#coffee-input').focus();
    });
});
