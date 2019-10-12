// Ultimo aggioramento 21/03/19
/******************************************************************
                             Librerie
 ******************************************************************/

#include <Servo.h>

/******************************************************************
                Definizione dei pin e delle variabili
 ******************************************************************/

/* Schede Supportate */
#define MI_PRIMER_KIT_DE_ROBOTICA_BLUETOOTH      1
#define BQ_ZUM_CORE_1                            2
#define BQ_ZUM_CORE_2                            3
#define BQ_ZUM_CORE_2_CON_BLUETOOTH_ESTERNO      4

/* Selezionare la scheda in uso */
//int boardSelected = MI_PRIMER_KIT_DE_ROBOTICA_BLUETOOTH;
int boardSelected = BQ_ZUM_CORE_1;
//int boardSelected = BQ_ZUM_CORE_2;
//int boardSelected = BQ_ZUM_CORE_2_CON_BLUETOOTH_ESTERNO;

/* Definizione dei PIN usati sulla scheda */
#define pinruotaSinistra         10/*  Sinistra servo Ruota       */
#define pinruotaDestra           9   /*  Destra servo Ruota         */


/* Definition of the values ​​that can take continuous rotation servo,
  that is, the ruotas */
#define ruotaStopValore 90
#define ruotaSinistraAvantiValore 180
#define ruotaSinistraIndietroValore 0
#define ruotaDestraAvantiValore 0
#define ruotaDestraIndietroValore 180

/* Size of the received data buffer */
#define bufferSize 5

/* Default delay */
#define defaultDelay        10

/* Creazione oggetti per la libreria Servo */
Servo ruotaSinistra;                    /*  Valores from 0 to 180  */
Servo ruotaDestra;                      /*  Valores from 0 to 180  */

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
  ruotaSinistra.write(ruotaStopValore);
  delay(defaultDelay);
  ruotaDestra.write(ruotaStopValore);
  delay(defaultDelay);
}

void goAvanti() {

  ruotaSinistra.write(ruotaSinistraAvantiValore);
  delay(defaultDelay);
  ruotaDestra.write(ruotaDestraAvantiValore);
  delay(defaultDelay);
}

void goIndietro() {

  ruotaSinistra.write(ruotaSinistraIndietroValore);
  delay(defaultDelay);
  ruotaDestra.write(ruotaDestraIndietroValore);
  delay(defaultDelay);
}

void goSinistra() {

  ruotaSinistra.write(ruotaStopValore);
  delay(defaultDelay);
  ruotaDestra.write(ruotaDestraAvantiValore);
  delay(defaultDelay);
}

void goDestra() {

  ruotaSinistra.write(ruotaSinistraAvantiValore);
  delay(defaultDelay);
  ruotaDestra.write(ruotaStopValore);
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
  if (boardSelected == BQ_ZUM_CORE_2_CON_BLUETOOTH_ESTERNO) {
    Serial.flush();
  }
  else {
    Serial.flush();
  }
}

void setup() {
  long vel_bluetooth;
  if (boardSelected == MI_PRIMER_KIT_DE_ROBOTICA_BLUETOOTH) {
    vel_bluetooth = 38400;
  }
  if (boardSelected == BQ_ZUM_CORE_1) {
    vel_bluetooth = 19200;
  }
  if (boardSelected == BQ_ZUM_CORE_2) {
    vel_bluetooth = 115200;
  }
  if (boardSelected == BQ_ZUM_CORE_2_CON_BLUETOOTH_ESTERNO) {
    vel_bluetooth = 9600;
  }

  /* Open the Bluetooth Serial and empty it */
  if (boardSelected == BQ_ZUM_CORE_2_CON_BLUETOOTH_ESTERNO) {
    Serial.begin(vel_bluetooth);
    Serial.flush();
  }
  else {
    Serial.begin(vel_bluetooth);
    Serial.flush();
  }

  /* Define the appropiate pin to each object */
  ruotaSinistra.attach(pinruotaSinistra);
  ruotaDestra.attach(pinruotaDestra);

  /* The robot is stopped at the beginning */
  stopRuote();
}

void loop() {

  /* If there is something in the Bluetooth serial port */
if (boardSelected == BQ_ZUM_CORE_2_CON_BLUETOOTH_ESTERNO) {
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
  else {

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
}
