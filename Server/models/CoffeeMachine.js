const ADRESS = "https://jsonplaceholder.typicode.com/posts"
const axios = require('axios');
class CoffeeMachine {

    static startMachine() {

    }
    static stopMachine() {

    }

    static brew(order) {

    }

    static order() {

    }

    static async  manual(command) {
        let res = await axios.post(ADRESS, command);
        
    }
}
module.exports = CoffeeMachine;