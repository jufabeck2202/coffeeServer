let express = require('express');
let app = express();
let bodyParser = require('body-parser')
let Machine = require("./models/CoffeeMachine");

app.use((req, res, next) => {
  res.header("Access-Control-Allow-Origin", "*");
  res.header("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
  next()
});

app.get('/', (req, res) => {
   
});

app.post("/manual", async (req, res) => {
  if(req.body.command == "AN:0A")
    res.status(500).send();
  await Machine.manual(req.body.command);
  res.status(200).send();
});

app.listen(3000, function () {
  console.log('Example app listening on port 3000!');
});