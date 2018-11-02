const ADRESS = "https://jsonplaceholder.typicode.com/posts"
const axios = require('axios');
const statusCodes = {"C045":"tray missing","C404":"no water","C444":"no tray and no water","4005":"machine on"}
class CoffeeMachine {

    static startMachine() {

    }
    static stopMachine() {

    }

    static brew(order) {

    }

    static order() {

    }
    static async getStatus() {
        let res = await axios.post(ADRESS, "RR:03");
        if(statusCodes.hasOwnProperty(res.body))
            return statusCodes[res.body]
        else return "unknown status code "+res.body
    }
    static getStats() {

    }


    static async  manual(command) {
        let res = await axios.post(ADRESS, command);
        
    }
}
module.exports = CoffeeMachine;