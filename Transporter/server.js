import EnturService from '@entur/sdk';
const express = require('express');

const app = express();
const port = 3000;

const entur = new EnturService({ clientName: 'student-hk' });
const primaryStopPlaceId = "NSR:StopPlace:62031"; // Heimdalsgata
const secondaryStopPlaceId = "NSR:StopPlace:59675"; // Herslebs Gate

const now = new Date();

async function getLatestTimes() {

  // Fix:
  // const departures = await entur.getStopPlaceDepartures([primaryStopPlaceId, secondaryStopPlaceId]);

  const params = {
    departures: 2
  };

  const departures = await entur.getStopPlaceDepartures(primaryStopPlaceId, params);

  const foundDepartures = [];

  departures.forEach((departure) => {
    const { expectedDepartureTime, destinationDisplay, serviceJourney } = departure;
    const { line } = serviceJourney.journeyPattern;

    const departureTime = new Date(expectedDepartureTime);
    const minDiff = minutesDifference(now, departureTime);
    const departureLabel = minDiff < 15 ? `${minDiff} min` : toTimeString(departureTime);

    const dep = {
      timeUntilNext: departureLabel,
      estimatedDepartureDate: departureTime,
      line: line.publicCode,
      destination: destinationDisplay.frontText
    };

    console.log(dep);

    foundDepartures.push(dep);

  });

  return foundDepartures;

}

/*
  minutesDifference and toTimeString are borrowed from the Entur API example
  https://github.com/entur/sdk/blob/master/example/get-departures.js
  December 8th 2019
 */

function minutesDifference(date1, date2) {
  const timeDiff = Math.abs(date2.getTime() - date1.getTime());
  return Math.floor(timeDiff / (1000 * 60));
}

function toTimeString(date) {
  const hour = String(date.getHours()).padStart(2, '0');
  const minute = String(date.getMinutes()).padStart(2, '0');
  return `${hour}:${minute}`;
}


app.get('/', async (req, res) => {

  const times = await getLatestTimes();
  console.log(times);

  res.send(times);

});

app.listen(port, () => console.log(`Listening on ${port}`));

module.exports = app;
