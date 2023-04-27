//This script initializes the random placements of the yellow men. However, if the yellow men aren't to be
//randomly positioned, then this script can be scrapped.
initializedYellowMen = false;

$(document).ready(function() {

      $(document).scroll(function() {
            
            
      })
})

//If we're no longer randomizing the yellow men, then the functions below are probably not needed.
//The yellow men will be placed on the map based on the lat/long coordinates of the city the yellow man corresponds to.

function randomInteger(max) {
      return Math.floor(Math.random() * max);
}

function initYellowMen() {
      if (initializedYellowMen)
            return

      let map = $("#right > #map > img")

      let position = map.position()
      let top = Math.floor(position["top"])
      let left = Math.floor(position["left"])
      let width = Math.floor(map.width())
      let height = Math.floor(map.height())

      let numberPlaces = $(".infoContainer").children().length

      for (let i = 0; i < numberPlaces; i++) {
            let yellowManLeft = left + randomInteger(width)
            let yellowManTop = top + randomInteger(height)
            
            let yellowMan = document.createElement("yellowMan")

            yellowMan.style.top = yellowManTop;
            yellowMan.style.left = yellowManLeft;

            document.getElementById("yellowManContainer").appendChild(yellowMan)
      }

      initializedYellowMen = true;
}
