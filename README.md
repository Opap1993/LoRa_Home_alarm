LORAWAN HOME ALARM APPLICATION 

The application is based on a lorawan board based on the Atmel Atsamd21 microcontroller.
As an initial phase, the device checks only the main door of the house with a simple digital sensor and works as an interrupt in the code.
The application uses LoRaWan in order to maintain power in case of a powerloss, sends the data via a Helium console to the Raspberry pi based server that contains MQTT Mosquitto protocol and Node-Red.
The Payload is then delivered to the designated users via e-mail from the relevant Node-red extension.
