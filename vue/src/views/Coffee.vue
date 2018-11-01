<template>
  <div class="Coffee">
    <section class="container">

      <div class="field has-addons">
        <p class="control">
          <input class="input" v-model="manualCommand" type="text" placeholder="manual command">
        </p>
        <p class="control">
          <button @click="orderManual" class="button">
            send
          </button>
        </p>
      </div>
      <div class="notification">
        Manually send commands to the machine
      </div>
        <b-dropdown v-model="selectedCoffee">
        <button class="button is-primary" type="button" slot="trigger">
            <template v-for="(esp) in coffees" v-if="selectedCoffee==esp.command">
              {{esp.name}}
            </template>
            
            <font-awesome-icon icon="caret-down" />
        </button>
        
        <b-dropdown-item :value="esp.command"  v-for="(esp, i) in coffees" :key="i">
          <div class="media">
                <div class="media-content">
                    <h3>{{esp.name}}</h3>
                </div>
            </div>
        </b-dropdown-item>
    </b-dropdown>

    <button class="button is-success">Order</button>  
    </section>
  </div>
</template>

<script>
import axios from "axios";
export default {
  name: "Coffee",
  components: {},
  data: function() {
    return {
      selectedCoffee: "1",
      coffees:  [{name:"Espresso",command:"1"},{name:"Espressi",command:"2"},{name:"Esspersa",command:"3"}],
      manualCommand: "",
      server: "http://localhost:3000"
    };
  },
  mounted() {},
  methods: {
    orderManual() {
      axios
        .post(this.server + "/manual", {
          command: this.manualCommand
        })
        .then(function(response) {
          // handle success
          console.log(response);
      });
    }
  }
};
</script>