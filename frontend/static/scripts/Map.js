$(document).ready(function() {
      $(document).scroll(function() {


      })
})

function initializeMapMarkers() {
      let map = $("#right > #map > img");

      let position = map.position();

      let top = Math.floor(position["top"]);
      let left = Math.floor(position["left"]);
      let width = Math.floor(map.width());
      let height = Math.floor(map.height());

      for (let i = 0; i < numberCities; i++) {

      }

}
