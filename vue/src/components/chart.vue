 <script>
import { Radar } from "vue-chartjs";

export default {
  extends: Radar,
  mounted() {
    // Overwriting base render method with actual data.
    this.renderChart({
      labels: [
        "Espresso",
        "Cappuchino",
        "Latte",
        "Coffee",
        "Large Coffee",
      ],
      datasets: [
        {
          label: "brews",
          backgroundColor: "#f87979",
          data: [40, 20, 12, 39, 10]
        }
      ]
    });
  }
};
</script>
 
