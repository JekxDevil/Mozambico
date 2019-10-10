/******************************************************************
                             Librerie
 ******************************************************************/

#include <Servo.h>

/******************************************************************
                Definizione dei pin e delle variabili
 ******************************************************************/

/* Definizione dei PIN usati sulla scheda */

#define pinSinistraRuota         10   /*   Sinistra servo Ruota          */
#define pinDestraRuota           9   /*   Destra servo Ruota         */


/* Definition of the values ​​that can take continuous rotation servo,
  that is, the ruotas */
#define ruotaStopValore 90
#define sinistraRuotaAvantiValore 0
#define sinistraRuotaIndietroValore 180
#define destraRuotaAvantiValore 180
#define destraRuotaIndietroValore 0

/* Bauderate of the Bluetooth*/
#define MI_PRIMER_KIT_DE_ROBOTICA_BLUETOOTH    38400
#define BQ_ZUM_BLUETOOTH                       19200

/* Size of the received data buffer */
#define bufferSize 5

/* Default delay */
#define defaultDelay        10

/* Creazione oggetti per la libreria Servo */
Servo sinistraRuota;                    /*  Valores from 0 to 180  */
Servo destraRuota;                      /*  Valores from 0 to 180  */

/*  A char buffer to storage the received data from the Bluetooth
    Serial */
char dataBuffer[bufferSize];

/* Buffer iterator */
int i = 0;

/* Number of characters availables in the Serial */
int numChar = 0;


/******************************************************************
                       Definizione delle funzioni
 ******************************************************************/

void stopRuote() {
  sinistraRuota.write(ruotaStopValore);
  delay(defaultDelay);
  destraRuota.write(ruotaStopValore);
  delay(defaultDelay);
}

void goAvanti() {

  sinistraRuota.write(sinistraRuotaAvantiValore);
  delay(defaultDelay);
  destraRuota.write(destraRuotaAvantiValore);
  delay(defaultDelay);
}

void goIndietro() {

  sinistraRuota.write(sinistraRuotaIndietroValore);
  delay(defaultDelay);
  destraRuota.write(destraRuotaIndietroValore);
  delay(defaultDelay);
}

void goSinistra() {

  sinistraRuota.write(ruotaStopValore);
  delay(defaultDelay);
  destraRuota.write(destraRuotaAvantiValore);
  delay(defaultDelay);
}

void goDestra() {

  sinistraRuota.write(sinistraRuotaAvantiValore);
  delay(defaultDelay);
  destraRuota.write(ruotaStopValore);
  delay(defaultDelay);
}


/* Manage the buffer of data */
void checkData(char* data) {

  switch (data[0]) {

    /* Line follower mode button pressed */
    case 'I':
      break;

    /* Light avoider mode button pressed */
    case 'G':
      break;

    /* Obstacles avoider mode button pressed */
    case 'B':
      break;

    /* Manual control mode button pressed */
    case 'M':
      stopRuote();
      break;

    case'S':
      /* Stop button pressed */
      stopRuote();
      break;

    case 'U':
      /* Up button pressed */
      goAvanti();
      break;

    case 'D':
      /* Down button pressed */
      goIndietro();
      break;

    case 'L':
      /* Sinistra button pressed */
      goSinistra();
      break;

    case 'R':
      /* Destra button pressed */
      goDestra();
      break;
  }

  /* Empty the Serial */
  Serial.flush();
}

void setup() {

  /* Open the Bluetooth Serial and empty it */
  Serial.begin(BQ_ZUM_BLUETOOTH);
  //Serial.begin(MI_PRIMER_KIT_DE_ROBOTICA_BLUETOOTH);
  Serial.flush();

  /* Define the appropiate pin to each object */
  sinistraRuota.attach(pinSinistraRuota);
  destraRuota.attach(pinDestraRuota);

  /* The robot is stopped at the beginning */
  stopRuote();
}

void loop() {

  /* If there is something in the Bluetooth serial port */
  if (Serial.available() > 0) {

    /* Reset the iterator and clear the buffer */
    i = 0;
    memset(dataBuffer, 0, sizeof(dataBuffer));

    /* Wait for let the buffer fills up. Depends on the length of
       the data, 1 ms for each character more or less */
    delay(bufferSize);

    /* Number of characters availables in the Bluetooth Serial */
    numChar = Serial.available();

    /* Limit the number of characters that will be read from the
       Serial to avoid reading more than the size of the buffer */
    if (numChar > bufferSize) {
      numChar = bufferSize;
    }

    /* Read the Bluetooth Serial and store it in the buffer */
    while (numChar--) {
      dataBuffer[i++] = Serial.read();

      /* As data trickles in from your serial port you are
        grabbing as much as you can, but then when it runs out
        (as it will after a few bytes because the processor is
        much faster than a 9600 baud device) you exit loop, which
        then restarts, and resets i to zero, and someChar to an
        empty array.So please be sure to keep this delay */
      delay(3);
    }

    /* Manage the data */
    checkData(dataBuffer);
  }
}
