











#include <Arduino.h>
#include <DHT.h>                           //Abbiamo creato una classe, includendo una libreria per l'utilizzo del trasduttore che ci evita la programmazione della trasmissione dei dati dello stesso
#include <ESP8266WiFi.h>

byte ledPin = LED_BUILTIN;                 //led interno alla break-out board dichiarato e pronto per essere usato

char ssid[] = "FRITZ!Box 4040 JJ";         //SSID della rete WiFi (una stringa è un'array di caratteri in 8 bit convertito in codice americano Aschi) (char è la funzione con cui noi diciamo al programmatore di creare un'array di caratteri)
char pass[] = "69427333460009232869";      //password della rete WiFi

WiFiServer server(80);                     //attiva il server sulla porta 80

IPAddress ip      ( 10, 250, 250, 121);    //indirizzo IP del server
IPAddress gateway ( 10,   3,  27, 200);    //gateway della rete
IPAddress subnet  (255,   0,   0,   0);    //subnet mask della rete
                         
#define DHTPIN D4                          //Con questa riga impostiamo una costante ovvero impostiamo il pin D4 del ESP come ricevitore dei dati
#define DHTTYPE DHT22                      //Definiamo il tipo di trasduttore che useremo

DHT dht(DHTPIN, DHTTYPE);                  //Creazione della virtualizzazione nel programma del trasduttore

float h, t;

#define DHTtimeout 2000
unsigned long DHTtimer = 0;

String answer;

void setup() {

Serial.begin(115200);                      // Classe=Serial Metodo=begin 115200=Velocità di trasmissione BIT/sec (Il metodo è il nome che prende una funzione legato a una classe) (La proprietà è una variabile legata a una classe)
Serial.println("Booting");
pinMode( ledPin, OUTPUT);

dht.begin();

WiFi.config (ip, gateway, subnet);
WiFi.begin (ssid, pass);                  //si collega al WiFi
while ( WiFi.status() != WL_CONNECTED ){  //status va a leggere un registro del microcontrollore scritto dalla parte radio del microcontrollore fino a quando  (!=Not)
  Serial.print( ".");
  delay (500);
}
server.begin();                           //avvia il server TCP

  Serial.println( "Connesso al Wifi" );                 //notifica i parametri di rete
  Serial.print( "stato: \t\t" );      Serial.println( WiFi.status() );
  Serial.print( "Indirizzo IP: \t" ); Serial.println( WiFi.localIP() );
  Serial.print( "Subnet Mask: \t" );  Serial.println( WiFi.subnetMask() );
  Serial.print( "Gateway: \t" );      Serial.println( WiFi.gatewayIP() );
  Serial.print( "SSID: \t\t" );       Serial.println( WiFi.SSID() );
  Serial.print( "Segnale \t" );       Serial.println( WiFi.RSSI() );
  Serial.print( "Reti: 1t\t" );       Serial.println( WiFi.scanNetworks() );

delay(2000);
}

void loop() {
  if( millis() > DHTtimer + DHTtimeout ){
 h = dht.readHumidity();            //Viene dichiarata la variabile "h" e gli assegno un valore ricavato da un metodo(funzione)
 t = dht.readTemperature();         //Viene dichiarata la variabile "t" e gli assegno un valore ricavato da un metodo(funzione)
DHTtimer = millis();

Serial.printf("temp: %.2f\t", t);        //printf=print formattato, ovvero stampo una stringa che viene modificata in base ad alcuni caratteri speciali come % o \(codici di escape con cui interpreta il carattere successivo e svolge un'azione)
Serial.printf("hum: %.2f\n", h);         //.2 permette di visualizzare 2 cifre decimali dopo lo 0                           
 }                                       //delay=indica un ritardo in millisecondi MENTRE delay microsecond=indica un ritardo in microsecondi

WiFiClient client = server.available();  //crea il client WiFi
if ( client ){

 if ( client.connected() ){              //BUFFER=
  digitalWrite( ledPin, LOW );
  String request = client.readStringUntil ( '\r' );
  client.flush ();                       //FLUSH=pulizia del client
  client.println ( String( t, 1) );      //STRING=trasforma in stringa un valore numerico
  digitalWrite( ledPin, HIGH);
 }

 client.stop();                          //chiude il collegamento con il client
}
}
