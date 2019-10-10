
  /*==================
   * AUTHOR:
   * =================
   * DESCRIPTION:
   * ===============
   * ABOUT
   * cade nelle trappole
                DESCRIZIONE DELLA VELOCITA DELLA RUOTA:
                valore 0: massima velocita dietro
                valore 180: massima velocita avanti
                valore 90 : ferma
                DURATA DELLA ROTAZIONE:
                180 gradi: 1028 microsecondi
                90 gradi: 514 microsecodi
                45 gradi : 257 microsecondi
                30 gradi : 200 microsecondi
     -leggi --> Attiva il sensore ultrasuoni inviando (trigger) il segnale e ricevendolo nell'echo, da questo tempo di invio-risposta è possibile calcoare la distanza 
      
    */
    #include <Servo.h>
    #include <NewPing.h>
    Servo leftWheel;                   
    Servo rightWheel;      
    #define pinLeftWheel            10   /*   Left servo Weel        quello azzurro 8  */
    #define pinRightWheel           9   /*   Right servo Weel         quello azzurro 9*/

    //Costanti descritte sopra
    #define StopValue 90

//60 dx __ 180 sx
   // #define ForwardValueRight 60
   // #define ForwardValueLeft 

      #define ForwardValueRight 180
    #define ForwardValueLeft 0





    #define BackValueRight 0   
    #define BackValueLeft 180   

    #define nodelay 0              
    #define delay_giro_180 1028
    //#define delay_giro_90 514   era 450
   // #define delay_giro_90 514
    #define delay_giro_90 550
    #define delay_giro_45 257
    #define delay_giro_60 300
    #define delay_giro_30 150
                                                                                               
    #define LarghezzaCorsia 23.5
    #define LarghezzaRobot 11.2

    #define TriggerDestro 6
    #define EchoDestro 5
    #define TriggerSinistro 8
    #define EchoSinistro 7
    #define TriggerFronte 11
    #define EchoFronte 12
   

    float SinistraUltimaDistanza=0;
    float DestraUltimaDistanza=0;
    float FronteUltimaDistanza=0;

    float DestraPenultimaDistanza;
    float SinistraPenultimaDistanza;

    float Limite=3;
    float LimitePerCorreggereLaDirezione=900;
    long TempoDallUltimoRaddrizzamentoVersoDestra=0;
    long TempoDallUltimoRaddrizzamentoVersoSinistra=0;

    long TempoUltimaCurva90=0;

    void stopWheels() {
      leftWheel.write(StopValue);
      rightWheel.write(StopValue);
    }
    
    void goForward()
    {
      rightWheel.write(ForwardValueRight);
        leftWheel.write(ForwardValueLeft);
    }
    
    void ForwardRight()
    {
         rightWheel.write(180);
          leftWheel.write(0);
          delay(250);
    }

    void ForwardLeft()
    {
      rightWheel.write(0);
        leftWheel.write(180);  
        delay(250);
    }

    void goForwards_() 
    {
     if ( FronteUltimaDistanza>20 && (DestraUltimaDistanza<=3 || SinistraUltimaDistanza>7) && ((millis()-TempoDallUltimoRaddrizzamentoVersoDestra)>LimitePerCorreggereLaDirezione) ) //Se sono troppo verso destra
      {
        //Essendo che il robot va verso destra, se va troppo verso quel lato devo raddrizzarlo
        goLeft(20);
        rightWheel.write(180);
        leftWheel.write(0);  
        delay(20);
       TempoDallUltimoRaddrizzamentoVersoDestra=millis();
      }
       else if( FronteUltimaDistanza>20 && (SinistraUltimaDistanza<=3 || DestraUltimaDistanza>7) && ((millis()-TempoDallUltimoRaddrizzamentoVersoSinistra)>LimitePerCorreggereLaDirezione) )//sono nella giusta distanza dal bordo destro
      {
          goRight(20);
          rightWheel.write(180);
          leftWheel.write(0);
          delay(20);
          TempoDallUltimoRaddrizzamentoVersoSinistra=millis();
      }

      else 
      {
        rightWheel.write(ForwardValueRight);
        leftWheel.write(ForwardValueLeft);
      } 
      
    }

    void goRight(int Rotation)
    {
        leftWheel.write(ForwardValueLeft);
        rightWheel.write(StopValue);
        delay(Rotation);
        Serial.println("===========================");
            Serial.println("GIRATO VERSO DESTRA");
            Serial.println("===========================");
    }
    void goLeft(int Rotation)
    {
      leftWheel.write(StopValue);
      rightWheel.write(ForwardValueRight);
      delay(Rotation);
      Serial.println("===========================");
            Serial.println("GIRATO VERSO SINISTRA");
            Serial.println("===========================");
    }
     
  
    NewPing* Sinistra;
    NewPing* Destra;
    NewPing* Fronte;
    
    void setup() 
    {
         
      //associazioni motori   
      leftWheel.attach(pinLeftWheel);
      rightWheel.attach(pinRightWheel);

      //associazionoe sensori
      Destra= new NewPing(TriggerDestro, EchoDestro);
      Sinistra= new NewPing(TriggerSinistro, EchoSinistro);
      Fronte= new NewPing(TriggerFronte, EchoFronte);
      Serial.begin(9600);
      
    }
        
    void loop() 
    { 
     
      FronteUltimaDistanza= Fronte->ping_cm();
      SinistraUltimaDistanza=Sinistra -> ping_cm();
      DestraUltimaDistanza= Destra -> ping_cm();

      Serial.print("SINISTRA: ");
      Serial.print(SinistraUltimaDistanza);
      Serial.print("FRONTE: ");        
      Serial.print(FronteUltimaDistanza);
      Serial.print("DESTRA: ");     
      Serial.println(DestraUltimaDistanza);  
      
      
        if
        (FronteUltimaDistanza > DestraUltimaDistanza && FronteUltimaDistanza > SinistraUltimaDistanza)//Vai dritto
       {
           goForwards_();
          delay(100);
          
       } 


      if(FronteUltimaDistanza<20)
      {
       
        if(SinistraUltimaDistanza>DestraUltimaDistanza+10)
              {
                 goLeft(delay_giro_90);
                 goForwards_();
                
                delay(250);
                TempoUltimaCurva90=millis();
                
              }     
              
        if(DestraUltimaDistanza >SinistraUltimaDistanza+10) 
          {
               goRight(delay_giro_90);
              goForwards_();
              delay(250);
               TempoUltimaCurva90=millis();
          }
          if((DestraUltimaDistanza<=4) && (millis()-TempoDallUltimoRaddrizzamentoVersoSinistra)>LimitePerCorreggereLaDirezione ) //o se sinistra >7 
          {
           goLeft(20);
           goForwards_();
           delay(20);
           TempoDallUltimoRaddrizzamentoVersoSinistra=millis();
        }    
      
         if((SinistraUltimaDistanza<=4 ) && (millis()-TempoDallUltimoRaddrizzamentoVersoDestra)>LimitePerCorreggereLaDirezione) // o se destra >7
        {
          goRight(20);
          goForwards_();
          delay(20);
          TempoDallUltimoRaddrizzamentoVersoDestra=millis();
        }   
         
        
      }  else if(FronteUltimaDistanza>20 && SinistraUltimaDistanza>FronteUltimaDistanza+25 && (millis()-TempoUltimaCurva90)>1000)
        {
            goLeft(delay_giro_45);
            goForwards_();
            delay(100);
        }
        else if (FronteUltimaDistanza>20 && DestraUltimaDistanza>FronteUltimaDistanza+25 && (millis()-TempoUltimaCurva90)>1000)
        {
          goRight(delay_giro_45);
          goForwards_();
          delay(100);
        }
     


          
           
    }
