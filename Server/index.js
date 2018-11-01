let express = require('express');
let app = express();
let Machine = require("./models/CoffeeMachine");
let FritzBox = require("./models/FritzBox");


app.get('/', function (req, res) {
});

app.listen(3000, function () {
  console.log('Example app listening on port 3000!');
  FritzBox.init();
});