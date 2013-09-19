
$(document).ready(function () {

    $(".example-filter").on('click', function () {

        var filter = $(this).attr('data-filter');

        if (filter == "*") {
            $(".example-image").fadeIn();
        }
        else {
            $(".example-image").fadeOut();
            $(filter).fadeIn();
        }

    });


    $(".image-fullscreen-link").on('click', function () {

        var imageSrc = $(this).children('img').attr('src');

        $("#image-fullscreen").attr('src', imageSrc);
        $("#image-popup").fadeIn();

    });

    $("#image-popup").on('click', function () {

        $("#image-popup").fadeOut();

    });

});
