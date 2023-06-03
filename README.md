# README

This code represents a graphical user interface (GUI) application that allows users to input coordinates or addresses and display a map with a route between the specified locations. The application is built using the Qt framework and utilizes the Google Maps API for displaying the map and calculating routes.

## Features
- The application window has a minimum size of 800x600 pixels.
- Users can select the input type (coordinates or addresses) using a dropdown combo box.
- Input fields are dynamically shown or hidden based on the selected input type.
- The input fields include:
  - Start Address
  - End Address
  - Start Latitude
  - Start Longitude
  - End Latitude
  - End Longitude
  - Max Distance
- The map is displayed using the QWebEngineView widget.
- The application dynamically resizes the input fields based on the window size.
- The `setInputFieldsVisibility()` function updates the visibility of the input fields based on the selected input type.
- The `setMapLocation()` function retrieves the input values, constructs an HTML string, and sets it to the QWebEngineView widget to display the map.
- The HTML string includes JavaScript code that interacts with the Google Maps API to calculate and display the route.
- When the user clicks on the map, a marker is added if it is within a certain distance from the route.
- The map is centered around Kiev, Ukraine, with a zoom level of 11.
- The walking mode is used for calculating the route.
- The route is split into segments based on the maximum distance specified by the user.
- Each segment is displayed with a different color on the map.

## Usage
1. Install the necessary dependencies, including the Qt framework.
2. Build and run the application.
3. Select the input type (Coordinates or Address) from the dropdown combo box.
4. Enter the required input values based on the selected input type.
5. The map will be displayed with the route between the specified locations.
6. Click on the map to add markers within the specified distance from the route.

## Dependencies
- Qt framework (minimum version required: [insert version here])
- Google Maps API key

## Contributors
- [Insert your name and contact information if applicable]

## License
[Insert license information here]
