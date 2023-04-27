const NAME = 0;
const LAT = 1;
const LONG = 2;
const MIN_TEMP = 3;
const MAX_TEMP = 4;
const MIN_RAIN = 5;
const MAX_RAIN = 6;
const MIN_HUMIDITY = 7;
const MAX_HUMIDITY = 8;
const TEMP_TREND = 9;
const CLIMATE = 10;
const RANK = 11;

class City {
      //A City object stores these members
      constructor(name, lat, long, minTemp, maxTemp, minRain, maxRain, minHumidity, maxHumidity, tempTrend, climate, rank) {
            this.name = name;
            this.lat = lat;
            this.long = long;
            this.minTemp = minTemp;
            this.maxTemp = maxTemp;
            this.minRain = minRain;
            this.maxRain = maxRain;
            this.minHumidity = minHumidity;
            this.maxHumidity = maxHumidity;
            this.tempTrend = tempTrend;
            this.climate = climate;
            this.rank = rank;
      }

      *getData() {
            yield this.name;
            yield this.lat;
            yield this.long;
            yield this.minTemp;
            yield this.maxTemp;
            yield this.minRain;
            yield this.maxRain;
            yield this.minHumidity;
            yield this.maxHumidity;
            yield this.tempTrend;
            yield this.climate;
            yield this.rank;
      }
}

let cities = [];
let numberCities = 0;

let numberPages = 0;
let resultsPerPage = 3;

$(document).ready(function() {
      //Retrieving data
      let data = $("extraneous").text();
      console.log(data)
      $("extraneous").remove();

      //Initializing page
      readData(data);
      initializeMapMarkers();
      pageSwap(1);
      
      //Viewing different pages of results
      $(".goTo").click(function() {
            let pageNumber = parseInt($("#pageNumber").val());

            //Page # invalid -> return
            if (isNaN(pageNumber) || pageNumber < 0 || pageNumber > numberPages)
                  return;
            
            pageSwap(pageNumber);
      });
})

function readData(data) {
      lines = data.split("\n");

      //Sort and time taken
      sort = lines[1].charAt(0).toUpperCase() + lines[1].substr(1).toLowerCase();
      timeTaken = lines[2];
      $("span.timing").text(sort+" sort took "+timeTaken+"ms")

      //Filtering out non-city data
      lines.shift();
      lines.shift();
      lines.shift();
      lines.pop();

      lines = cleanData(lines);

      //Reading in cities, each city has 12 data points
      for (let index = 0; index < lines.length; index+=12) {
            cities.push(new City(lines[index], 
                                 lines[index+1], 
                                 lines[index+2], 
                                 lines[index+3], 
                                 lines[index+4], 
                                 lines[index+5], 
                                 lines[index+6], 
                                 lines[index+7], 
                                 lines[index+8], 
                                 lines[index+9], 
                                 lines[index+10], 
                                 lines[index+11]));
      }

      //Initializing variables and text on page
      numberCities = cities.length;
      numberPages = Math.ceil(numberCities / resultsPerPage);
      $(".obscured").text("of "+numberPages);
}

function display(start, end) {
      //Removing pre-existing elements
      let infoContainer = $(".infoContainer");
      infoContainer.children().each(function() {
            $(this).remove();
      })

      //Adding the City elements/objects from index start to index end (exclusive) to the page
      for (let i = start; i < end; i++) {
            data = [...cities[i].getData()];

            infoContainer.append("<div class='infoBox'></div>");
            let infoBox = infoContainer.children().last();

            infoBox.append(element("h2", data[NAME]));
            infoBox.append(element("span", "Ranked #"+data[RANK]));
            infoBox.append(category("Average Temperature (F)", average(data[MIN_TEMP], data[MAX_TEMP])));
            infoBox.append(category("Average Rainfall (in)", average(data[MIN_RAIN], data[MAX_RAIN])));
            infoBox.append(category("Average Humidity", average(data[MIN_HUMIDITY], data[MAX_HUMIDITY])));
            infoBox.append(category("Temperature Trend", convertTrend(data[TEMP_TREND])));
            infoBox.append(category("Climate", convertClimate(data[CLIMATE])));
      }
}

function average(min, max) {
      return ((parseFloat(min) + parseFloat(max))/2.0).toFixed(1);
}

function convertTrend(temperature) {
      if (temperature > 0) 
            return "Warmer";
      return "Cooler";
}

function convertClimate(climate) {
      //The argument climate will be an integer
      climateValues = ["Tropical", "Mild", "Continental", "Polar", "Dry"];
      return climateValues[parseInt(climate)]
}

function element(tag, text) {
      return "<"+tag+">"+text+"</"+tag+">";
}

function category(label, text) {
      return '<p><span class="category">'+label+'</span>'+text+'</p>';
}

function cleanData(data) {
      cleanedData = [];
      for (let i = 0; i < data.length; i++) {
            if (data[i] != "")
                  cleanedData.push(data[i]);
      }
      return cleanedData;
}

function pageSwap(pageNumber) {
      //Determining what elements of the array display on the given page
      //Example: The elements 3 4 5 in the array [0 1 2 3 4 5 6 7 8] will appear on the page when the page number is 2 (3 per page)
      //Example: The elements 9 10 in the array [0 1 2 3 4 5 6 7 8 9 10] will appear on the page when the page number is 4 (3 per page)
      let start = (pageNumber - 1)*resultsPerPage;
      if (pageNumber == numberPages)
            if (numberCities % resultsPerPage == 0)
                  display(start, start + (resultsPerPage));
            else
                  display(start, start + (numberCities%resultsPerPage));
      else
            display(start, (pageNumber)*resultsPerPage);
}
