/*
X tengely középpont: 331
X tengely maximum  : 675
X tengely minimum  : 0
Y tengely középpont: 346
Y tengely maximum  : 675
Y tengely minimum  : 0


FULL előre:   X:331 Y:675
FULL HÁTRA:   X:331 Y: 0
FULL JOBBRA:  X:675 Y:346
FÉLIG JOBBRA: X:675 Y:675
FULL BALRA:   X:0   Y:346
FÉLIG BALRA:  X:0   Y:675
HÁTRA BALRA:  x:0   Y:0
HÁTRA JOBBRA: X:675 Y:0

*/

int pinX =A0;
int pinY =A1;
int pinS = 8;
int pinA = 2;
int pinB = 3;
int pinC = 4;
int pinD = 5;
int pinE = 6;
int pinF = 7;

void setup() {
  delay(200);
  Serial.begin(9600);
  pinMode(pinS, INPUT_PULLUP);
  pinMode(pinA, INPUT_PULLUP);
  pinMode(pinB, INPUT_PULLUP);
  pinMode(pinC, INPUT_PULLUP);
  pinMode(pinD, INPUT_PULLUP);
  pinMode(pinE, INPUT_PULLUP);
  pinMode(pinF, INPUT_PULLUP);
}

void loop() {
  int  xValue = analogRead(pinX);
  int  yValue = analogRead(pinY);
  bool sPressed = (digitalRead(pinS) == LOW);
  bool aPressed = (digitalRead(pinA) == LOW);
  bool bPressed = (digitalRead(pinB) == LOW);
  bool cPressed = (digitalRead(pinC) == LOW);
  bool dPressed = (digitalRead(pinD) == LOW);
  bool ePressed = (digitalRead(pinE) == LOW);
  bool fPressed = (digitalRead(pinF) == LOW);

  Serial.print( "X: " );
  Serial.print( xValue );
  Serial.print( " Y: " );
  Serial.print( yValue );
  Serial.print( " | Y: " );
  Serial.print( sPressed );
  Serial.print( " A: " );
  Serial.print( aPressed );
  Serial.print( " B: " );
  Serial.print( bPressed );
  Serial.print( " C: " );
  Serial.print( cPressed );
  Serial.print( " D: " );
  Serial.print( dPressed );
  Serial.print( " E: " );
  Serial.print( ePressed );
  Serial.print( " F: " );
  Serial.println( fPressed );

  delay(200);
}
