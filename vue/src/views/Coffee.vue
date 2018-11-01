<template>
  <div class="Coffee">
    <button @click="orderManual"></button>
  </div>
</template>

<script>
import axios from 'axios'
export default {
  name: "Coffee",
  components: {},
  data: function() {
    return {
      server: "http://localhost:3000"
    };
  },
  mounted() {},
  methods: {
    orderManual() {
      axios.post(this.server + "/manual", {command:"code"});
    }
  }
};
</script>