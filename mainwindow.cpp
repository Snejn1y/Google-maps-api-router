#include "mainwindow.h"
#include <QVBoxLayout>
#include <QResizeEvent>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSizePolicy>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    setMinimumSize(800, 600);
    inputType = new QComboBox;
    inputType->addItem("Coordinates");
    inputType->addItem("Address");

    startAddressLabel = new QLabel("Start Address:");
    endAddressLabel = new QLabel("End Address:");
    startLatLabel = new QLabel("Start Latitude:");
    startLngLabel = new QLabel("Start Longitude:");
    endLatLabel = new QLabel("End Latitude:");
    endLngLabel = new QLabel("End Longitude:");
    maxDistanceLabel = new QLabel("Max Distance:");

    startAddressInput = new QLineEdit;
    endAddressInput = new QLineEdit;
    startLatInput = new QLineEdit;
    startLngInput = new QLineEdit;
    endLatInput = new QLineEdit;
    endLngInput = new QLineEdit;
    maxDistanceInput = new QLineEdit;
    mapView = new QWebEngineView;

    QVBoxLayout *mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    QGridLayout *inputLayout = new QGridLayout;
    inputLayout->addWidget(inputType, 0, 0, 1, 2);
    inputLayout->addWidget(startAddressLabel, 1, 0);
    inputLayout->addWidget(startAddressInput, 1, 1);
    inputLayout->addWidget(endAddressLabel, 2, 0);
    inputLayout->addWidget(endAddressInput, 2, 1);
    inputLayout->addWidget(startLatLabel, 3, 0);
    inputLayout->addWidget(startLatInput, 3, 1);
    inputLayout->addWidget(startLngLabel, 4, 0);
    inputLayout->addWidget(startLngInput, 4, 1);
    inputLayout->addWidget(endLatLabel, 5, 0);
    inputLayout->addWidget(endLatInput, 5, 1);
    inputLayout->addWidget(endLngLabel, 6, 0);
    inputLayout->addWidget(endLngInput, 6, 1);
    inputLayout->addWidget(maxDistanceLabel, 7, 0);
    inputLayout->addWidget(maxDistanceInput, 7, 1);

    mainLayout->addLayout(inputLayout);
    mainLayout->addWidget(mapView);

    connect(inputType, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::setInputFieldsVisibility);
    connect(startAddressInput, &QLineEdit::textChanged, this, &MainWindow::setMapLocation);
    connect(endAddressInput, &QLineEdit::textChanged, this, &MainWindow::setMapLocation);
    connect(startLatInput, &QLineEdit::textChanged, this, &MainWindow::setMapLocation);
    connect(startLngInput, &QLineEdit::textChanged, this, &MainWindow::setMapLocation);
    connect(endLatInput, &QLineEdit::textChanged, this, &MainWindow::setMapLocation);
    connect(endLngInput, &QLineEdit::textChanged, this, &MainWindow::setMapLocation);
    connect(maxDistanceInput, &QLineEdit::textChanged, this, &MainWindow::setMapLocation);

    setInputFieldsVisibility();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    updateFieldSizes();
}

void MainWindow::updateFieldSizes()
{
    int width = size().width() / 2;
    startAddressInput->setMinimumWidth(width);
    endAddressInput->setMinimumWidth(width);
    startLatInput->setMinimumWidth(width);
    startLngInput->setMinimumWidth(width);
    endLatInput->setMinimumWidth(width);
    endLngInput->setMinimumWidth(width);
    maxDistanceInput->setMinimumWidth(width);
}

void MainWindow::setInputFieldsVisibility()
{
    bool isAddressInput = (inputType->currentIndex() == 1);
    startAddressLabel->setVisible(isAddressInput);
    startAddressInput->setVisible(isAddressInput);
    endAddressLabel->setVisible(isAddressInput);
    endAddressInput->setVisible(isAddressInput);
    startLatLabel->setVisible(!isAddressInput);
    startLatInput->setVisible(!isAddressInput);
    startLngLabel->setVisible(!isAddressInput);
    startLngInput->setVisible(!isAddressInput);
    endLatLabel->setVisible(!isAddressInput);
    endLatInput->setVisible(!isAddressInput);
    endLngLabel->setVisible(!isAddressInput);
    endLngInput->setVisible(!isAddressInput);
    maxDistanceLabel->setVisible(true);
    maxDistanceInput->setVisible(true);

    updateFieldSizes();
}

void MainWindow::setMapLocation()
{
    QString startLocation;
    QString endLocation;
    QString maxDistance = maxDistanceInput->text();

    if (inputType->currentIndex() == 1) { // Address
        startLocation = startAddressInput->text();
        endLocation = endAddressInput->text();
    } else { // Coordinates
        QString startLat = startLatInput->text();
        QString startLng = startLngInput->text();
        QString endLat = endLatInput->text();
        QString endLng = endLngInput->text();
        startLocation = QString("%1,%2").arg(startLat, startLng);
        endLocation = QString("%1,%2").arg(endLat, endLng);
    }

    if (!startLocation.isEmpty() && !endLocation.isEmpty()) {
        QString html = QString(R"(
<!DOCTYPE html>
<html>
<head>
<meta http-equiv="Content-Security-Policy" content="default-src * 'unsafe-inline' 'unsafe-eval'; img-src * data:; connect-src *;">
    <meta name="viewport" content="initial-scale=1.0, user-scalable=no">
    <meta charset="utf-8">
    <style>
        html, body, #map {
            height: 100%;
            margin: 0;
            padding: 0;
        }
    </style>
    <script src="https://unpkg.com/@turf/turf@6.3.0/turf.min.js"></script>
    <script async defer src="https://maps.googleapis.com/maps/api/js?key=Key&libraries=geometry&callback=initMap"></script>
    <script>
let map, directionsService, polyline, line, maxDistanceKm;

function addMarker(location) {
    if (google.maps.geometry.poly.isLocationOnEdge(location, polyline, 0.0005)) {
        let from = turf.nearestPointOnLine(line, [location.lng(), location.lat()]);
        let distance = turf.distance(from.geometry.coordinates, [location.lng(), location.lat()], {units: 'kilometers'});
        if (distance <= maxDistanceKm) {
            new google.maps.Marker({
                position: location,
                map: map
            });
        } else {
            alert('The marker is too far from the path.');
        }
    } else {
        alert('The marker is not on the path.');
    }
}

function initMap() {
    directionsService = new google.maps.DirectionsService();
    const kiyv = new google.maps.LatLng(50.44872580947909, 30.514827772247568);
    const mapOptions = {
        zoom: 11,
        center: kiyv,
        streetViewControl: false
    };
    map = new google.maps.Map(document.getElementById('map'), mapOptions);

    map.addListener('click', function(event) {
        addMarker(event.latLng);
    });

    calculateAndDisplayRoute(directionsService);
}

function calculateAndDisplayRoute(directionsService) {
    const startLocation = '%1';
    const endLocation = '%2';
    maxDistanceKm = parseFloat('%3');

    const request = {
        origin: startLocation,
        destination: endLocation,
        travelMode: 'WALKING'
    };

    directionsService.route(request, function(result, status) {
        if (status === 'OK') {
            let pathCoordinates = result.routes[0].overview_path.map(function(location) {
                return [location.lng(), location.lat()];
            });
            line = turf.lineString(pathCoordinates);

            let waypointsForward = [];
            let waypointsBackward = [];
            let lineDistance = turf.length(line, {units: 'kilometers'});
            for (let i = 0; i < lineDistance; i += maxDistanceKm) {
                let waypointForward = turf.along(line, i, {units: 'kilometers'});
                waypointsForward.push({
                    location: new google.maps.LatLng(waypointForward.geometry.coordinates[1], waypointForward.geometry.coordinates[0]),
                    stopover: true,
                });
                let waypointBackward = turf.along(line, lineDistance - i, {units: 'kilometers'});
                waypointsBackward.push({
                    location: new google.maps.LatLng(waypointBackward.geometry.coordinates[1], waypointBackward.geometry.coordinates[0]),
                    stopover: true,
                });
            }

            let waypoints = waypointsForward.concat(waypointsBackward.reverse());

            request.waypoints = waypoints;

            directionsService.route(request, function(result, status) {
                if (status === 'OK') {
                    let color = "#FF0000"; // initial color
                    for (let i = 0; i < result.routes[0].legs.length; i++) {
                        let leg = result.routes[0].legs[i];
                        let path = leg.steps.reduce(function(path, step) {
                            return path.concat(step.path);
                        }, []);
                        polyline = new google.maps.Polyline({
                            path: path,
                            strokeColor: color,
                            strokeWeight: 2
                        });
                        polyline.setMap(map);
                        color = (color === "#FF0000") ? "#0000FF" : "#FF0000"; // switch color
                    }
                }
            });
        }
    });
}
    </script>
</head>
<body>
<div id="map"></div>
</body>
</html>

                           )").arg(startLocation, endLocation, maxDistance);
        mapView->setHtml(html, QUrl("qrc:/"));
    }
}
