<template>
  <div class="Coffee">
    <section class="hero is-small is-dark is-bold">
      <div class="hero-body">
        <div class="container">
          <h1 class="title">
            Jura remote controll
          </h1>
          <h2 class="subtitle">
            order your coffee here
          </h2>
        </div>
      </div>
    </section>
    <section class="container controll">
      <div class="columns is-multiline is-mobile status">
        <!-- All other tile elemnts -->
        <div class="column is-half">
          <button class="button is-primary is-success is-rounded is-medium" @click="startMachine">Start Machine</button>
          <div class="spacer"></div>
          <button class="button is-primary is-danger is-rounded is-medium">Stop Machine</button>
        </div>
        <div class="column is-half">
          <p class="is-size-4">Machine:  <span class="has-text-danger"><font-awesome-icon icon="toggle-off" /> off</span> </p>
          <p class="is-size-4">Status: <span class="has-text-warning">not running</span></p>
        </div>

      </div>
      <nav class="level">
        <!-- Left side -->
        <div class="level-left">
          <div class="field has-addons">
            <p class="control">
              <b-dropdown v-model="selectedCoffee">
                <button class="button is-primary" type="button" slot="trigger">
                  <template v-for="(esp) in coffees" v-if="selectedCoffee==esp.command">
                    {{esp.name}}
                  </template>
                  <font-awesome-icon icon="caret-down" />
                </button>
                <b-dropdown-item :value="esp.command" v-for="(esp, i) in coffees" :key="i">
                  <div class="media">
                    <div class="media-content">
                      <h3>{{esp.name}}</h3>
                    </div>
                  </div>
                </b-dropdown-item>
              </b-dropdown>
            </p>
            <p class="control">
              <button class="button is-success">Order</button>
            </p>
          </div>
        </div>

        <!-- Right side -->
        <div class="level-right">
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
        </div>
      </nav>
      <p class="is-size-3">Commands: </p>
      <div class="notification">
        AN:01 coffeemaker on<br>
        AN:02 coffeemaker off<br>
        AN:03 display test<br>
        AN:04 test touch sensor<br>
        AN:05 key test<br>
        AN:06 key test<br>
        AN:0C UCHI test<br>
        AN:0D production date?<br>
        FA:01 product 1<br>
        FA:02 product 2<br>
        FA:03 product 3<br>
        FA:04 product 4<br>
        FA:05 product 5<br>
        FA:06 product 6 <br>
        FA:07 product 7<br>
        FA:08 hot water?<br>
        FA:09 steam/09<br>
        FN:01 pump on<br>
        FN:02 pump off<br>
        FN:03 heating on<br>
        FN:04 heating off<br>
        FN:05 steam heating on <br>
        FN:06 steam heating off Mühle links dauermalen ein FN:07<br>
        FN:08 Mühle links dauermalen aus <br>
        FN:09 Mühle rechts dauermalen ein <br>
        FN:0A Mühle rechts dauermalen aus <br>
        FN:0B Pumpe Dampf ein <br>
        FN:0C Pumpe Dampf aus <br>
        FN:0D Init. Brühgruppe <br>
        FN:0F Brühgr. In Mahlposition <br>
        FN:10<br>
        FN:11 Brühgruppe? <br>
        FN:12 Brühgruppe? <br>
        FN:13 Brühgruppe Brühstellung <br>
        FN:14 Produkt 1 mahlen <br>
        FN:15 Produkt 1 pumpen <br>
        FN:16 Produkt 3 mahlen <br>
        FN:17 Produkt 3 pumpen <br>
        FN:18<br>
        FN:19<br>
        FN:1A<br>
        FN:1B <br>
        FN:1C <br>
        FN:1C <br>
        FN:1D Drain.ventil ein<br>
        FN:1E Drain.ventil aus <br>
        FN:20<br>
        FN:21<br>
        FN:22 Brühgruppe? <br>
        FN:23<br>
        FN:24 Ablassventil ein <br>
        FN:25 Ablassventil aus <br>
        FN:26 Dampfventil ein <br>
        FN:27 Dampfventil aus <br>
        FN:28 Cappuch.ventil ein <br>
        FN:29 Cappuch.ventil aus <br>
        FN:2A<br>
        FN:2B<br>
        FN:2C<br>
        FN:2D<br>
        FN:2E<br>
        FN:2F<br>
        FN:0E Brühgruppe Trester auswerfen <br>
        RE:XX Lese Wort, Wort angeben <br>
        RT:XX Lese Zeile in EEPROM (16 Worte) Adresse angeben <br>
        TY:XX Wer da? <br>
        WE:XX Schreibe Wort: <br>
        RE:31 Maschinentyp lesen <br>
        IC: Eingänge lesen <br>
        RR:03 Status, C045-> no_tray, C404 -> nowater, C444 ->no_tray_and_no_water, 4005-> on <br>
        RR:0B<br>
        RR:04 Heizungswerte? <br>
        RR:1D<br>
      </div>
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
      coffees: [
        {
          name: "Espresso",
          command: "1"
        },
        {
          name: "Espressi",
          command: "2"
        },
        {
          name: "Esspersa",
          command: "3"
        }
      ],
      manualCommand: "",
      server: "http://localhost:3000"
    };
  },
  mounted() {
  },
  methods: {
    orderManual() {
      axios
        .post(this.server + "/manual", {
          command: this.manualCommand
        })
        .then(function(response) {
          // handle success
        });
    },
    startMachine() {
      //launch toast
      let heatingDuration = 5000;
      let heatTimer = 5000 / 1000;
      let toast = this.$toast.open({
        duration: heatingDuration,
        position: "is-bottom",
        message: `Please wait ${heatTimer}s until your machine is heated`,
        type: "is-info"
      });

      let timerId = setInterval(() => {
        if (heatTimer == -1) {
          clearTimeout(timerId);
        } else {
          heatTimer--;
          toast.message = `Please wait ${heatTimer}s until your machine is heated`;
        }
      }, 1000);
    }
  }
};
</script>

<style>
.spacer {
  padding: 5px;
}
.controll {
  padding-left: 100px;
  padding-right: 100px;
}
.status {
  padding: 10px;
}
</style>