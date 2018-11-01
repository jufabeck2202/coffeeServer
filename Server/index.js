let express = require('express');
let app = express();
let Machine = require("./models/CoffeeMachine");

app.get('/', function (req, res) {
});

app.listen(3000, function () {
  console.log('Example app listening on port 3000!');
});