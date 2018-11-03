const ADRESS = "https://jsonplaceholder.typicode.com/posts"
const axios = require('axios');
const logger = require("../logger");
const START_TIME = 3;
const BREW_TIME = 3;
const SHUTDOWN_TIME = 20;
const statusCodes = {
    "C045": "tray missing",
    "C404": "no water",
    "C444": "no tray and no water",
    "4005": "machine on"
}

class CoffeeMachine {

    static get ON() {
        return this.on;
    }
    static set ON(on) {
        this.on = on
    }
    static async start() {
        //start machine
        let res = await axios.post(ADRESS, "AN:01");
        await this.timeout(START_TIME);
        this.on = true
        this.sleep()
    }

    static async stop() {

        let res = await axios.post(ADRESS, "AN:02");
    }

    static async brew(order) {
        let res = await axios.post(ADRESS, "FA:0" + order);
        await this.timeout(BREW_TIME)
        this.sleep()
    }

    static sleep() {
        logger.info("reset sleep timer")
        clearTimeout(this.shutdownTimeout)
        this.shutdownTimeout = setTimeout(() => {
            logger.info("Machine Sleeping")
            this.on = false
        }, SHUTDOWN_TIME * 1000)
    }
    static async getStatus() {
        let res = await axios.post(ADRESS, "RR:03");
        if (statusCodes.hasOwnProperty(res.body))
            return statusCodes[res.body]
        else return "unknown status code " + res.body
    }

    static async getStats() {
        let res = await axios.post(ADRESS, "stats");
        return res.body
    }


    static async manual(command) {
        let res = await axios.post(ADRESS, command);

    }

    static timeout(s) {
        return new Promise(resolve => setTimeout(resolve, s * 1000));
    }
}
//set class variables
module.exports = CoffeeMachine;