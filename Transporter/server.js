import EnturService from '@entur/sdk';
const express = require('express');

const app = express();
const port = 3000;

const entur = new EnturService({ clientName: 'student-hk' });
const primaryStopPlaceId = "NSR:StopPlace:62031"; // Heimdalsgata
const secondaryStopPlaceId = "NSR:StopPlace:59675"; // Herslebs Gate

async function getLatestTimes() {

  const params = {
    departures: 2
  };

  const departures = await entur.getStopPlaceDepartures([primaryStopPlaceId, secondaryStopPlaceId], params);

  const primaryDepartures = departures[0].departures;
  const secondaryDepartures = departures[1].departures;

  const allDepartures = primaryDepartures.concat(secondaryDepartures);

  const foundDepartures = [];

  secondaryDepartures.forEach((departure) => {
    const { expectedDepartureTime, destinationDisplay, serviceJourney } = departure;
    const { line } = serviceJourney.journeyPattern;

    const now = new Date();

    const departureTime = new Date(expectedDepartureTime);
    const inMinutes = minutesDifference(now, departureTime);
    const departureLabel = inMinutes > 0 ? `${inMinutes} min` : toTimeString(departureTime);

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

  if ((hour === "0") && (minute === "0")) {
    return "Nå";
  }

  return `${hour}:${minute}`;
}


app.get('/', async (req, res) => {

  res.setHeader("Content-Type", "application/json");

  const times = await getLatestTimes();
  console.log(times);

  await res.json(times);

});

app.listen(port, () => console.log(`Listening on ${port}`));

module.exports = app;
