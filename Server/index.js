let express = require('express');
let app = express();
let bodyParser = require('body-parser')
let Machine = require("./models/CoffeeMachine");
let FritzBox = require("./models/FritzBox");

app.use( bodyParser.json() );       // to support JSON-encoded bodies
app.use(bodyParser.urlencoded({     // to support URL-encoded bodies
  extended: true
})); 

app.use((req, res, next) => {
  res.header("Access-Control-Allow-Origin", "*");
  res.header("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
  next()
});

app.get('/', (req, res) => {
   
});
app.post("/manual", (req, res) => {
  Machine.manual(req.body.command);
});

app.listen(3000, function () {
  console.log('Example app listening on port 3000!');
  FritzBox.init();
});