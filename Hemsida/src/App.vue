<template>
  <v-app id="content">
    <!-- Toolbaren och dess rubrik -->
    <v-toolbar app id="toolbar">
      <v-toolbar-title class="headline text-uppercase">
        <span>Driver</span>
        <span class="font-weight-light">bot</span>
      </v-toolbar-title>

      <!-- Knapparna i toolbaren. När man klickar på "Anslutningsalternativ" ska en ruta komma upp. -->
      <!-- När man klickar på "Sliders"-knappen ändras sidan och knappens text ändras till "Buttons". -->
      <v-spacer></v-spacer>
      <v-btn color="primary" dark @click.stop="dialog = true">Anslutningsalternativ</v-btn>
      <v-btn target="_blank" @click="Switch=!Switch">
        <span v-if="Switch!==true" class="mr-2">Sliders</span>
        <span v-else class="mr-2">Buttons</span>
      </v-btn>
    </v-toolbar>

    <!-- Rutan som kommer upp när man klickar på "Anslutningsalternativ". Storlek bestäms samt rubriker till textfälten. -->
    <!-- Även spara-knappen läggs till. -->
    <v-content>
      <v-row align="center" justify="center">
        <v-dialog v-model="dialog" max-width="400" height="290">
          <v-card>
            <v-card-title class="headline">Anslutning till Server</v-card-title>

            <v-card-text style="height: 370px;">
              <v-text-field v-model="name" label="Name" filled></v-text-field>

              <v-text-field v-model="password" label="MQTT Password" filled></v-text-field>
              <v-text-field v-model="topic" label="MQTT topic" filled></v-text-field>

              <v-text-field v-model="port" label="Port" filled></v-text-field>
              <v-text-field v-model="adress" label="Adress" filled></v-text-field>
            </v-card-text>

            <v-card-actions>
              <v-spacer></v-spacer>

              <v-btn color="blue" text @click="save()">Spara</v-btn>
            </v-card-actions>
          </v-card>
        </v-dialog>

        <div>
          <Logger />

          <!-- Om knappens läge är "Switch" ska "Buttons"-sidan visas. Annars visas "Buttons2". -->
          <Buttons v-if="Switch" />
          <Buttons2 v-else />
        </div>
      </v-row>
    </v-content>
  </v-app>
</template>

<script>

//Komponenterna importeras.
import Buttons2 from "./components/Buttonsv2";
import Buttons from "./components/Buttons";
import Logger from "./components/Logger";

export default {
  name: "App",
  components: {
    Buttons,
    Buttons2,
    Logger
  },
  data() {
    return {
      Switch: false,
      dialog: false,
      name: "",
      password: "",
      topic: "",
      port: "",
      adress: ""
    };
  },
  created() {
    let User = this.$store.getters.GetUser;
    this.name = User.name;
    this.password = User.password;
    this.topic = User.topic;
    this.port = User.port;
    this.adress = User.adress;
  },

  methods: {
    save() {
      let User = {
        name: this.name,
        password: this.password,
        topic: this.topic,
        port: this.port,
        adress: this.adress
      };

      this.$store.dispatch("Save", User);

      this.dialog = false;
    }
  },
  computed: {}
};
</script>

<!-- En massa CSS. Bestämmer hur hemsidan ska se ut; layout, färg etc. -->

<style scoped>
.img {
  width: 40%;
  height: 40%;
  margin: auto;
  padding: 10px;
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

/* https://beautifuldingbats.com/pattern-generator/ https://css-tricks.com/using-svg/ */
#toolbar {
background:url("data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHhtbG5zOnhsaW5rPSJodHRwOi8vd3d3LnczLm9yZy8xOTk5L3hsaW5rIiB3aWR0aD0iMTAwJSIgaGVpZ2h0PSIxMDAlIj48ZGVmcz48cGF0dGVybiBpZD0icGF0dGVybiIgd2lkdGg9IjU3IiBoZWlnaHQ9IjU3IiB2aWV3Qm94PSIwIDAgNDAgNDAiIHBhdHRlcm5Vbml0cz0idXNlclNwYWNlT25Vc2UiIHBhdHRlcm5UcmFuc2Zvcm09InJvdGF0ZSgzMzIpIj48cmVjdCBpZD0icGF0dGVybi1iYWNrZ3JvdW5kIiB4PSIwIiB5PSIwIiB3aWR0aD0iNDAwJSIgaGVpZ2h0PSI0MDAlIiBmaWxsPSIjZTllY2VmIi8+ICA8IS0tLS0+IDwhLS0tLT4gPHBhdGggZD0ibSAwIDEwIGggNDAiIHN0cm9rZS13aWR0aD0iMjAiIHN0cm9rZT0iIzNiNWJkYiIgc2hhcGUtcmVuZGVyaW5nPSJhdXRvIiBzdHJva2UtbGluZWNhcD0iYnV0dCIgc3Ryb2tlLWxpbmVqb2luPSJyb3VuZCIgZmlsbD0idHJhbnNwYXJlbnQiLz4gICAgPCEtLS0tPiA8IS0tLS0+IDxwYXRoIGQ9Im0wLDAgTDQwLDQwIiBzdHJva2Utd2lkdGg9IjE4IiBzdHJva2U9IiMzMzlhZjAiIHNoYXBlLXJlbmRlcmluZz0iYXV0byIgc3Ryb2tlLWxpbmVjYXA9InNxdWFyZSIgc3Ryb2tlLWxpbmVqb2luPSJyb3VuZCIgZmlsbD0idHJhbnNwYXJlbnQiLz48cGF0aCBkPSJtNDAsMCBMODAsNDAiIHN0cm9rZS13aWR0aD0iMTgiIHN0cm9rZT0iIzMzOWFmMCIgc2hhcGUtcmVuZGVyaW5nPSJhdXRvIiBzdHJva2UtbGluZWNhcD0ic3F1YXJlIiBzdHJva2UtbGluZWpvaW49InJvdW5kIiBmaWxsPSJ0cmFuc3BhcmVudCIvPjxwYXRoIGQ9Im0tNDAsMCBMMCw0MCIgc3Ryb2tlLXdpZHRoPSIxOCIgc3Ryb2tlPSIjMzM5YWYwIiBzaGFwZS1yZW5kZXJpbmc9ImF1dG8iIHN0cm9rZS1saW5lY2FwPSJzcXVhcmUiIHN0cm9rZS1saW5lam9pbj0icm91bmQiIGZpbGw9InRyYW5zcGFyZW50Ii8+ICAgPC9wYXR0ZXJuPjwvZGVmcz4gPHJlY3QgZmlsbD0idXJsKCNwYXR0ZXJuKSIgaGVpZ2h0PSIxMDAlIiB3aWR0aD0iMTAwJSIgeT0iMCIgeD0iMCIvPjwvc3ZnPg==")
}
#content {
background:url("data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHhtbG5zOnhsaW5rPSJodHRwOi8vd3d3LnczLm9yZy8xOTk5L3hsaW5rIiB3aWR0aD0iMTAwJSIgaGVpZ2h0PSIxMDAlIj48ZGVmcz48cGF0dGVybiBpZD0icGF0dGVybiIgd2lkdGg9IjU3IiBoZWlnaHQ9IjU3IiB2aWV3Qm94PSIwIDAgNDAgNDAiIHBhdHRlcm5Vbml0cz0idXNlclNwYWNlT25Vc2UiIHBhdHRlcm5UcmFuc2Zvcm09InJvdGF0ZSgzMzIpIj48cmVjdCBpZD0icGF0dGVybi1iYWNrZ3JvdW5kIiB4PSIwIiB5PSIwIiB3aWR0aD0iNDAwJSIgaGVpZ2h0PSI0MDAlIiBmaWxsPSIjZTllY2VmIi8+ICA8IS0tLS0+IDwhLS0tLT4gPHBhdGggZD0ibSAwIDMwIGggNDAiIHN0cm9rZS13aWR0aD0iMTciIHN0cm9rZT0iIzNiNWJkYiIgc2hhcGUtcmVuZGVyaW5nPSJhdXRvIiBzdHJva2UtbGluZWNhcD0iYnV0dCIgc3Ryb2tlLWxpbmVqb2luPSJyb3VuZCIgZmlsbD0idHJhbnNwYXJlbnQiLz4gICAgPCEtLS0tPiA8IS0tLS0+IDxwYXRoIGQ9Im0gMCAxMCBoIDQwIiBzdHJva2Utd2lkdGg9IjIwIiBzdHJva2U9IiMzMzlhZjAiIHNoYXBlLXJlbmRlcmluZz0iYXV0byIgc3Ryb2tlLWxpbmVjYXA9ImJ1dHQiIHN0cm9rZS1saW5lam9pbj0icm91bmQiIGZpbGw9InRyYW5zcGFyZW50Ii8+ICAgPC9wYXR0ZXJuPjwvZGVmcz4gPHJlY3QgZmlsbD0idXJsKCNwYXR0ZXJuKSIgaGVpZ2h0PSIxMDAlIiB3aWR0aD0iMTAwJSIgeT0iMCIgeD0iMCIvPjwvc3ZnPg==")
}
</style>
