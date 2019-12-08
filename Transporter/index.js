const express = require('express');

const app = express();
const port = 3000;

app.get('/', (req, res) => {
  res.send('17: 3 min');
});

app.listen(port, () => console.log(`Listening on ${port}`))
