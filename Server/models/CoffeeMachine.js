const ADRESS = "http://192.168.178.132/api"
const axios = require('axios');
const logger = require("../logger");
const START_TIME = 60;
const BREW_TIME = 30;
const SHUTDOWN_TIME = 60*60;
const statusCodes = {
    "C045": "tray missing",
    "C404": "no water",
    "C444": "no tray and no water",
    "4005": "machine on"
}
const config = {
    headers: {
        'Content-Length': 0,
        'Content-Type': 'text/plain'
    },
   responseType: 'text'
};

class CoffeeMachine {

    static get ON() {
        return this.on;
    }
    static set ON(on) {
        this.on = on
    }
    static async start() {
        //start machine
        let res = await axios.post(ADRESS, "AN:01",config);
        await this.timeout(START_TIME);
        logger.info("Machine started...")
        this.on = true
        this.sleep()
    }

    static async stop() {
        let res = await axios.post(ADRESS, "AN:02", config);
        this.on = false
        logger.info("Machine stop")
    }

    static async brew(order) {
        let res = await axios.post(ADRESS, "FA:0" + order,config);
        logger.info("Start brewing "+order+" "+res.body)
        await this.timeout(BREW_TIME)
        this.sleep()
    }

    static sleep() {
        logger.info("reset sleep timer")
        clearTimeout(this.shutdownTimeout)
        this.shutdownTimeout = setTimeout(() => {
            
            this.on = false
            logger.info("Machine Sleeping "+this.on)
        }, SHUTDOWN_TIME * 1000)
    }
    static async getStatus() {
        let res = await axios.post(ADRESS, "RR:03");
        if (statusCodes.hasOwnProperty(res.body))
            return statusCodes[res.body]
        else return "unknown status code " + res.body
    }

    static async getStats() {
        let res = await axios.post(ADRESS, "stats",config);
        return res.body
    }


    static async manual(command) {
        logger.info("recieved manual command "+command)
        let res = await axios.post(ADRESS, command,config);
    }

    static timeout(s) {
        return new Promise(resolve => setTimeout(resolve, s * 1000));
    }
}
//set class variables
module.exports = CoffeeMachine;