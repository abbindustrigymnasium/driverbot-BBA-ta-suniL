<template>
  <v-container id="box">
    <v-layout row>
      <v-flex class="justify-center mb-6">
        <!-- Om connected är false visas endast en heading och bild från angiven adress. -->
        <div class v-if="connected==false">
          <h1>Kontrolldon till Driverbot!</h1>
          <v-img class="img" src="https://media.giphy.com/media/l3vQYPi2ow7YWXQFW/giphy.gif" />
        </div>

        <!-- Om connected är true visas loggern (ett card med några knappar). -->
        <v-card v-else class="mx-auto" max-width="344" outlined>
          <h1>Logger</h1>
          <v-flex v-for="(item,index) in logging" :key="item+index">{{item}}</v-flex>
        <!-- Två knappar som anropar två olika funktioner. -->
          <v-card-actions>
            <v-btn text @click="Clear()">Clear</v-btn>
            <v-btn text @click="Disconnect()">Disconnect</v-btn>
          </v-card-actions>
        </v-card>
        <v-card></v-card>
      </v-flex>
    </v-layout>
  </v-container>
</template>

<script>
//Bibliotek som ska användas
export default {
  name: "Logger",
  props: {
    //Data som skickas in i komponenten
  },
  data() {
    //All data som ska finnas i komponenten
    return {
      car: "green",
      clientId: "notyetAssigned"
    };
  },
  computed: {
    connected() {
      let connect = this.$store.getters.connected;

      return connect;
    },
    logging() {
      let logger = this.$store.getters.logger;
      return logger;
    }
  },
  methods: {
    //Metoder för att rensa loggern och disconnecta från den.
    Clear() {
      this.$store.dispatch("clearLogger");
    },
    Disconnect() {
      this.$store.dispatch("Connect", false);
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
  /* position: fixed;
  bottom: 0;
  right: 0; */
  width: 300px;
  height: 300px;
  border: black dotted 2px;

  word-wrap: break-word;
}
</style>
