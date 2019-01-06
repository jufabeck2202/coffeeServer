const express = require('express');
const cors = require("cors");
const bodyParser = require('body-parser')
const Machine = require("./models/CoffeeMachine");
const logger = require("./logger");
Machine.ON = false;


let app = express();
app.use(bodyParser.json())
app.use(cors())
app.use(express.static('../vue/dist'));

app.get('/', (req, res) => {
  res.sendFile('../vue/dist/index.html');
});
app.post("/start", async (req, res) => {
  Machine.start()
});

//for Homebridge
app.get('/getStatus', (req, res) => {
  if (Machine.ON)
    res.status(200).send("1")
  else
    res.status(200).send("0")
});

app.post("/stop", async (req, res) => {
  Machine.stop()
});

app.post("/status", async (req, res) => {
  res.status("200").send({
    status: Machine.ON
  })
});


app.post("/manual", async (req, res) => {
  if (req.body.command == "AN:0A")
    res.status(500).send();
  await Machine.manual(req.body.command);
  res.status(200).send();

});


app.post("/orderInstant", async (req, res) => {
  startBrew(req.body.order)
  res.status("200").send({
    status: "brewing coffee",
    seconds: "100"
  })
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

app.get("/orderInstant/:id", async (req, res) => {
  startBrew(req.params.id)
  res.status("200").send({
    status: "brewing coffee",
    seconds: "100"
  })
});

app.get("/order/:id", async (req, res) => {
  if (!Machine.ON) {
    startMachineAndBrew(req.params.id)
    res.status("200").send({
      status: "starting Machine and brewing",
      seconds: "300"
    })
  } else {
    startBrew(req.params.id)
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
  logger.info("recived order " + order + ", starting Machine");
  await Machine.start()
  logger.info("Machine Started");
  await Machine.brew(order)
  logger.info("Finished brewing");
}
app.listen(3000, function () {
  console.log('Example app listening on port 3000!');

});
