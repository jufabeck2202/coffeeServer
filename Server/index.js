let express = require('express');
let app = express();
let bodyParser = require('body-parser')
let Machine = require("./models/CoffeeMachine");
const logger = require("./logger");
Machine.ON = false;



app.use(bodyParser.json())

app.use((req, res, next) => {
  res.header("Access-Control-Allow-Origin", "*");
  res.header("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
  next()
});

app.get('/', (req, res) => {

});
app.post("/start", async (req, res) => {
  Machine.start()
});

app.post("/stop", async (req, res) => {
  Machine.stop()
});

app.post("/manual", async (req, res) => {
  if (req.body.command == "AN:0A")
    res.status(500).send();
  await Machine.manual(req.body.command);
  res.status(200).send();

});

app.post("/order", async (req, res) => {
  if (!Machine.ON) {
    startMachineAndBrew(req.body.order)
    res.status("200").send({
      status: "starting Machine and brewing",
      seconds: "300"
    })
  } else {
    startBrew(req.body.order)
    res.status("200").send({
      status: "brewing coffee",
      seconds: "100"
    })
  }
});

async function startBrew(order) {
  logger.info("recived order, start brewing");
  await Machine.brew(order)
  logger.info("Finished brewing");
}

async function startMachineAndBrew(order) {
  logger.info("recived order "+order+", starting Machine");
  await Machine.start()
  logger.info("Machine Started");
  await Machine.brew(order)
  logger.info("Finished brewing");
}
app.listen(3000, function () {
  console.log('Example app listening on port 3000!');

});