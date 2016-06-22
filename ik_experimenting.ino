#include <Servo.h>

#define pi   3.1415926535897932
#define pi_2 1.5707963267948966

#define base 0
#define shoulder 1
#define elbow 2
#define wrist 3

/*
        /\
       /  \ carpal
ulna  /    \____
     |          | pen
     | humerus
     |
============
   base
 */

#define humerus 48.25 //mm from base to first joint
#define ulna 111.40 //mm from first joint to second
#define carpal 48.25 //mm from second joint to third

#define xoffset 65 //mm offset from third joint to pen tip
#define yoffset 90

Servo servo[4];
int cnt = 0;
boolean decrementing = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  servo[base].attach(6);
  servo[shoulder].attach(9);
  servo[elbow].attach(10);
  servo[wrist].attach(11);
  goto_xyz(100, 50, 0);
}

void loop() {
  //goto_xyz(100, 50, 0);
  /*
  for(double i = 0; i < 20; i++) {
    for(double j = 0; j < 20; j++) {
      for(double k = 0; k < 10; k++) {
        goto_xyz(i, k, j);
        delay(100);
      }
    }
  }*/
  for(int i = 0; i < 5; i++) {
    servo[i].write(0);
  }/*
  if (cnt == 0) {
    decrementing = false;
    cnt++;
  } else if (cnt < 36 && (!decrementing)) {
    cnt++;
  } else if (decrementing) {
    cnt--;
  } else {
    decrementing = true;
  }
  delay(50);*/
}

void goto_xyz(double x, double y, double z) {
  Serial.print("Moving to ");
  Serial.print(x);
  Serial.print(", ");
  Serial.print(y);
  Serial.print(", ");
  Serial.println(z);
  double omega = atan(z/x);
  Serial.println((sq(y - humerus) + sq(x) - sq(ulna) - sq(carpal))/(2*ulna*carpal));
  double beta = asin((sq(y - humerus) + sq(x) - sq(ulna) - sq(carpal))/(2*ulna*carpal));
  if (beta == NAN) {
    return;
  }
  //double alpha1 = atan2(y - humerus, x);
  //double alpha2 = asin(carpal*cos(beta)/sqrt(sq(y - humerus) + sq(x)));
  //double alpha = alpha1 + alpha2
  double alpha = atan((y - humerus)/ x) + asin(carpal*cos(beta)/sqrt(sq(y - humerus) + sq(x)));
  double gamma = pi_2 + acos((x - ulna*cos(alpha))/carpal);
  Serial.print("Alpha=");
  Serial.println(alpha);
  Serial.print("Beta=");
  Serial.println(beta);
  Serial.print("Gamma=");
  Serial.println(gamma);
  Serial.print("Omega=");
  Serial.println(omega);
  servo[base].write(map(omega, -pi_2, pi_2, 0, 180));
  servo[shoulder].write(map(alpha, 0, pi, 0, 180));
  servo[elbow].write(map(beta, 0, pi, 0, 180));
  servo[wrist].write(map(gamma, 0, pi, 0, 180));
}

