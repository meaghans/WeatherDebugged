$(document).ready(function() {
      $("#climateForm").submit(function(event) {
            $("input").each(function() {
                  //If the input hasn't been entered -> autofill input
                  if ($(this).val() == "") {
                        fill($(this), $(this).attr('name'));
                  }
            });
      });
})

function fill(element, name) {
      defaultValues = {
            "tempMax": "NULL",
            "tempMin": "NULL",
            "humidityMax": "NULL",
            "humidityMin": "NULL",
            "rainfallMax": "NULL",
            "rainfallMin": "NULL",
            "tempTrend": "NULL",
            "climateType": "NULL",
            "sortMethod": "quick"
      }
      element.val(defaultValues[name]);
}
