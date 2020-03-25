<template>
  <v-card 
    max-width="500" 
    flat color="teal"
    class="mx-auto"
  >
  <v-container fluid>
    <v-row>
      <v-col cols="12">
        <v-subheader class="pl-0">Speed</v-subheader>
        <v-slider
          v-model="slider"
          thumb-label
        ></v-slider>
      </v-col>
    </v-row>
  </v-container>
  <v-container>
    <v-row>
      <v-slider
        v-model="value"
        vertical
        label="Regular"
      ></v-slider>
      <v-range-slider
        v-model="value2"
        vertical
        label="Range"
      ></v-range-slider>
    </v-row>
  </v-container>
  </v-card>
</template>


<script>
//Bibliotek som ska användas
var mqtt = require("mqtt");
export default {
  name: "Buttons",
  props: {
    //Data som skickas in i komponenten
  },
  data() {
    //All data som ska finnas i komponenten
    return {
      connected: false,
      car: "green",
      clientId: "notyetAssigned",
      client: null,
      speed: 600,
      ticklabels: ["Långsamt", "Snabbare", "Snabbast"],
      options: {}
    };
  },
  computed: {
    Disconnect() {
      if (this.$store.getters.connected == false) {
        return true;
      }
      return false;
    }
  },
  watch: {
    //Om du vill logga någonting när det förändras i htmldelen. se exempel nedan
    Disconnect: {
      handler: function(newVal) {
        if (newVal == true) {
          this.connected = false;
          this.client = mqtt.disconnect;
          this.car = "green";
        }
      }
    }
  },
  methods: {
    //Metoder
    Connect() {
      //https://github.com/mqttjs/MQTT.js/blob/master/README.md
      var ref = this;
      if (this.connected == true) {
        return "";
      }
      let User = this.$store.getters.GetUser;
      this.clientId =
        "DriverControll" +
        Math.random()
          .toString(16)
          .substr(2, 8);
      var mqtt_url = User.adress;
      var url = "mqtt://" + mqtt_url;
      var options = {
        port: User.port,
        clientId: this.clientId,
        username: User.name,
        password: User.password
      };
      this.options = options;
      // console.log("connecting");
      this.client = mqtt.connect(url, options);
      // console.log("connected?");

      this.client
        .on("connect", function() {
          // console.log("success");
          ref.Connecting(true);
        })
        .on("error", function() {
          // console.log("error");
          ref.Connecting(false);
        })
        .on("close", function() {
          ref.Connecting(false);
          // console.log("closing");
        });
    },

    Connecting(connected) {
      this.connected = connected;
      this.$store.dispatch("Connect", connected);
      // console.log(this.connected)
      if (connected == false) {
        this.car = "red";
      } else {
        this.car = "blue";
        this.Send("drive", this.clientId + " har anslutits.");
      }
    },

    Send(adress, message) {
      // console.log(message);
      this.client.publish(
        this.options.username + "/" + "speed", //adress, //Exempel         "joakim.flink@abbindustrigymnasium.se"+"/" + "drive",
        message
      );

      this.$store.dispatch("addToLogger", message);
    }
  }
};
</script>

<!-- CSS -->
<style scoped>
.big {
  font-size: 25px;
}
#Cooltext {
  color: black;
  text-decoration: underline;
}
#box {
  width: 400px;
  height: 400px;
}
#hide{
 display: none;
}
#logger {
  position: fixed;
  bottom: 0;
  right: 0;
  width: 300px;
  height: 300px;
  border: black dotted 2px;

  word-wrap: break-word;
}
</style>
