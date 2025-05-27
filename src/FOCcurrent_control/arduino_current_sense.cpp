// #include <Arduino.h>
// #define CURRENT_RECORDING_LENGTH 10000

// // put function declarations here:
// int myFunction(int, int);


// float phase_current_a[CURRENT_RECORDING_LENGTH];
// float phase_current_b[CURRENT_RECORDING_LENGTH];
// void setup() {
//     // put your setup code here, to run once:
//     analogReadResolution(12);
//     pinMode(LED_BUILTIN,OUTPUT);
//     digitalWrite(LED_BUILTIN,HIGH);
//     delay(1000);
// }

// void loop() {
//     int start = micros();
//     static int c = 0;
//     // put your main code here, to run repeatedly:
//     phase_current_a[c] = 3.3 * analogRead(22) / 4069;
//     phase_current_b[c] = 3.3 * analogRead(18) / 4069;
//     c++;

//     if (c >= CURRENT_RECORDING_LENGTH) {
//         digitalWrite(LED_BUILTIN,LOW);

//         c = 0;
//         Serial.printf("12341234,.1,.1\n");

//         for(int i = 0; i < CURRENT_RECORDING_LENGTH; i++){
//             // Serial.printf(">a:%f\n>b:%f\n>c:%f\n",phase_a[i],phase_b[i],phase_c[i]);
//             // Serial.printf(">a:%f\n>b:%f\n>c:%f\n",phase_currents[i].a,phase_currents[i].b,phase_currents[i].c);
//             Serial.printf("%d,%f,%f\n",i,phase_current_a[i],phase_current_b[i]);
//         }
//         delay(200);
//         Serial.printf("12341234,.1,.1\n");
//         digitalWrite(LED_BUILTIN,HIGH);
//         delay(5);
//         digitalWrite(LED_BUILTIN,LOW);

//         delay(1000);
//         digitalWrite(LED_BUILTIN,HIGH);


//     }


//     while(micros()-start < 150) {} // do nothing 
    
// }
