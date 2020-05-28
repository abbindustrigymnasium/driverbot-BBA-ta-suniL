<template>
<!-- Ett card som innehåller 2 st sliders. -->
  <v-container>
    <v-card 
    width="700" 
    height="350"
    flat color="white"
    class="mx-auto">
  <!-- Den första slidern, kontrollerar hastighet. -->
  <!-- Går från -50 till 50, i steg om 10. Värdet som väljs skickas till MQTT-brokern -->
  <v-container fluid>
    <v-row>
      <v-col cols="12">
        <v-subheader class="pl-0">Speed</v-subheader>
        <v-slider
          min="-50"
          max="50"
          v-model="hastighet"  
          thumb-label
          step="10"
          @change="Send('h' + hastighet)"                    
        ></v-slider>
      </v-col>
    </v-row>
  </v-container>
  <!-- Den andra slidern, kontrollerar styrning. -->
  <!-- Går från -90 till 90, i steg om 30. Värdet som väljs skickas till MQTT-brokern -->
  <v-container fluid>
    <v-row>
      <v-col cols="12">
        <v-subheader class="pl-0">Steering</v-subheader>
        <v-slider
          min="-90"
          max="90"
          step="30"
          v-model="styrning"
          thumb-label
          v-on:change="Send('s' + styrning)"
        ></v-slider>
      </v-col>
    </v-row> 
  </v-container>
  </v-card>
  </v-container>
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
//Data som används i komponenten
    return {
      connected: false,
      clientId: "notyetAssigned",
      client: null,
      speed: 600,
      options: {},
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
   created() {
    //När komponenten är skapad
    this.Connect()
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
        topic: User.topic,
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
      if (connected == false) {
        this.car = "red";
      } else {
        this.car = "blue";
        //this.Send("drive", this.clientId + " har anslutits.");
      }
    },

//Funktion som skickar iväg meddelandet (styr-/körvärden) till MQTT, samt lägger skickar till loggern.
    Send(message) {
      this.client.publish(
        this.options.username + "/" + this.options.topic,
        message
      );
      this.$store.dispatch("addToLogger", message);
    }
  }
};
</script>

<!-- En massa CSS. Bestämmer hur hemsidan ska se ut; layout, färg etc. -->

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
