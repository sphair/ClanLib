
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

});
