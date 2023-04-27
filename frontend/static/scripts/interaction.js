$(document).ready(function() {
      //Showing button selection
      $(".buttonContainer > .button").click(function() {
            let button = $(this);
            resetSelectedButton(button.parent()); //Remove previously selected button
            button.toggleClass("selected"); //Select selected button
      })

      //Preset values
      $(".preset.button").click(function() {
            setDefaultValues($(this), $(this).attr("id"));
      })

      //Registering input for trend buttons
      $(".button.trending").click(function() {
            updateTrendingInput($(this), $(this).attr("id"));
      })
})

function resetSelectedButton(defaultInputContainer) {
      defaultInputContainer.children().each(function() {
            if ($(this).hasClass("selected")) {
                  $(this).toggleClass("selected");
            }
      })
      return false;
}

function setDefaultValues(element, setting) {
      defaultValues = {
            "cold": [1, 10],
            "warm": [11, 20],
            "hot": [21, 30],
            "dryer": [1, 50],
            "moister": [51, 100],
            "more": [36, 70],
            "less": [0, 50],
            "more": [51, 100]
      }
      let minElement = element.parent().parent().children(".numbersContainer").children().eq(0).children("input");
      let maxElement = element.parent().parent().children(".numbersContainer").children().eq(1).children("input");
      minElement.val(defaultValues[setting][0]);
      maxElement.val(defaultValues[setting][1]);
}

function updateTrendingInput(element, trend) {
      let inputElement = element.parent().parent().children("input");
      inputElement.val(trend);
}

