<template>
  <v-container id="box">
 <span id="hide">    {{Disconnect}}</span>
    <!-- Card som styrmodulen ligger på. -->
    <v-card class="elevation-12" color="grey lighten-1">
      <v-layout row>
        <v-flex class="justify-center mb-6">
          <!-- Knapp som lagrar speed-värdet som 25. -->
          <v-btn class="ma-2" v-if="connected" tile color="red" icon @click="speed=25">
            25
            <v-icon>directions_car</v-icon>
          </v-btn>

          <!-- Knapp som skickar styrvärdet 30. -->
          <v-btn
            class="ma-2 rotate-45"
            tile
            large
            color="teal"
            icon
            :disabled="!connected"
            v-on:click="Send('s30')"
          >
            <v-icon>keyboard_arrow_left</v-icon>
          </v-btn>
          <!-- Knapp för att köra framåt med vald hastighet. -->
          <v-btn
            class="ma-2"
            tile
            large
            color="teal"
            icon
            :disabled="!connected"
            @click="Send('h'+speed)"
          >
            <v-icon>keyboard_arrow_up</v-icon>
          </v-btn>
          <!-- Knapp som skickar styrvärdet -30. -->
          <v-btn
            class="ma-2 rotate-135"
            tile
            large
            color="teal"
            icon
            :disabled="!connected"
            @click="Send('s-30')"
          >
            <v-icon>keyboard_arrow_left</v-icon>
          </v-btn>
          <!-- Knapp som lagrar speed-värdet som 35. -->
          <v-btn class="ma-2" tile v-if="connected" color="blue" icon @click="speed=35">
            35
            <v-icon>directions_car</v-icon>
          </v-btn>
        </v-flex>
      </v-layout>
      <v-layout row>
        <v-flex md6>
          <!-- Knapp som skickar styrvärdet 60. -->
          <v-btn
            class="ma-2"
            tile
            large
            color="teal"
            icon
            :disabled="!connected"
            @click="Send('s60')"
          >
            <v-icon>keyboard_arrow_left</v-icon>
          </v-btn>
        </v-flex>
        <v-flex md6>
          <!-- Knapp för att starta modulen/loggern. -->
          <v-btn v-if="!connected" class="ma-2" tile large :color="car" icon @click="Connect()">
            <v-icon>directions_car</v-icon>
          </v-btn>
          <!-- Knapp som stannar bilen. -->
          <v-btn v-else class="ma-2" tile large :color="car" icon @click="Send('stop')">
            <v-icon>pause</v-icon>
          </v-btn>
        </v-flex>
        <v-flex md6>
          <!-- Knapp som skickar styrvärdet -60. -->
          <v-btn
            class="ma-2"
            tile
            large
            color="teal"
            icon
            :disabled="!connected"
            @click="Send('s-60')"
          >
            <v-icon>keyboard_arrow_right</v-icon>
          </v-btn>
        </v-flex>
      </v-layout>
      <v-layout row>
        <v-flex class="justify-center mb-6">
          <!-- Knapp som lagrar speed-värdet som 40. -->
          <v-btn class="ma-2" v-if="connected" tile color="green" icon @click="speed=40">
            40
            <v-icon>directions_car</v-icon>
          </v-btn>
          <!-- Knapp som skickar styrvärdet 90. -->
          <v-btn
            class="ma-2 rotate-45"
            tile
            large
            color="teal"
            icon
            :disabled="!connected"
            @click="Send('s90')"
          >
            <v-icon>keyboard_arrow_down</v-icon>
          </v-btn>
          <!-- Knapp för att backa med vald hastighet. -->
          <v-btn
            class="ma-2"
            tile
            large
            color="teal"
            icon
            :disabled="!connected"
            @click="Send('h-'+speed)"
          >
            <v-icon>keyboard_arrow_down</v-icon>
          </v-btn>
          <!-- Knapp som skickar styrvärdet -90. -->
          <v-btn
            class="ma-2 rotate-135"
            tile
            large
            color="teal"
            icon
            :disabled="!connected"
            @click="Send('s-90')"
          >
            <v-icon>keyboard_arrow_up</v-icon>
          </v-btn>
          <!-- Knapp som lagrar speed-värdet som 50. -->
          <v-btn class="ma-2" tile v-if="connected" color="purple" icon @click="speed=50">
            50
            <v-icon>directions_car</v-icon>
          </v-btn>
        </v-flex>
      </v-layout>
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
    //All data som ska finnas i komponenten
    return {
      connected: false,
      car: "green",
      clientId: "notyetAssigned",
      client: null,
      options: {},
      speed: 500
    };
  },
  created() {
    //När komponenten är skapad
  },
  mounted() {
    //När komponenten är mountad (inladdad)
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
    //Funktion som hämtar värden (namn, lösenord, port osv.) från Store.
    Connect() {
      let ref = this;
      if (this.connected == true) {
        return "";
      }

      let User = this.$store.getters.GetUser;
      this.clientId =
        "DriverControll" +
        Math.random()
          .toString(16)
          .substr(2, 8);
      var mqtt_url = "maqiatto.com";
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

      this.connected = true;
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

      this.$store.dispatch("Connect", this.connected);

      if (this.connected == false) {
        this.car = "red";
      } else {
        this.car = "blue";
        //this.Send("drive", this.clientId + " har anslutits.");
      }
    },

//Funktion som ändrar färg på bil-knappen. Röd om man inte är ansluten, annars blå.
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

//Funktion som skickar iväg meddelandet (styr-/körvärden) till MQTT, samt lägger skickar till loggern.
    Send(message) {
      // console.log(message);
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
#logger {
  position: fixed;
  bottom: 0;
  right: 0;
  width: 300px;
  height: 300px;
  border: black dotted 2px;

  word-wrap: break-word;
}
#hide{
 display: none;
}
.rotate-45 {
  filter: progid:DXImageTransform.Microsoft.BasicImage(rotation=0.5);
  -webkit-transform: rotate(45deg);
  -moz-transform: rotate(45deg);
  -ms-transform: rotate(45deg);
  -o-transform: rotate(45deg);
  transform: rotate(45deg);
}
.rotate-135 {
  filter: progid:DXImageTransform.Microsoft.BasicImage(rotation=1.5);
  -webkit-transform: rotate(135deg);
  -moz-transform: rotate(135deg);
  -ms-transform: rotate(135deg);
  -o-transform: rotate(135deg);
  transform: rotate(135deg);
}
</style>
