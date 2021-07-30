/*  
 *   Programa Delfín - Verano de investigación
 *   Proyecto de investigación 
 *   
 *   Mide el tiempo de ejecución de un programa/operación matemática 
 *   en software y hardware.
 *   
 *   El tema del proyecto de investigación es comparar tiempos de ejecución
 *   de diferentes funciones matemáticas con códigos o algoritmos diferentes
 *   en la plataforma Arduino (un solo hardware). 
 *   Para medir el tiempo usando el hardware, se usan los pulsos de voltaje
 *   para señalar la inicialización y terminación del programa. Se mide el 
 *   tiempo usando un osciloscopio.
 *   Para medir el tiempo usando el software, se usan las funciones de micros()
 *   para obtener la diferencia de tiempo. Esta misma operación se repite un 
 *   total de 10000 veces para obtener un promedio con una muestra relativamente
 *   grande.
 *   
 *   Links de referencia/codigos distinos:
 *   https://forum.arduino.cc/t/method-for-measuring-execution-time-of-function-is-it-accurate/571162/4
 *   https://forum.arduino.cc/t/measuring-code-execution-period/44139
 *   https://www.arduino.cc/reference/en/language/functions/time/micros/
 *   https://stackoverflow.com/questions/36254652/arduino-tcnt1-to-count-clock-cycles-between-interrupts
 *   https://figshare.com/articles/preprint/Accurate_measurement_of_Arduino_code_execution_time/11955030
 *   https://blog.wokwi.com/how-to-measure-the-speed-of-arduino-code/
 *   https://arduino.stackexchange.com/questions/45834/the-exact-time-for-functions-or-cpu-cycles-for-any-function
 *   
 *   Elaborado por Andrés Ortega González
 *   Creado en 28 de junio del 2021
 *   Editado en 14 de julio del 2021
 *   
 *   Tipos de funciones: trigonométricas, básicos (por determinar por el investigador)
 *   
 *   Cosas que hacer: hacer que funcione TestFunction() con contador < 12.
 *   
 */

int contador = 12;   //  < 12 falla idk why
float testVariable = 0;
float testMatrix[3];
unsigned long totalCycles = 0;
unsigned long startDebug = 0;
unsigned long stopDebug = 0;
volatile unsigned long overflowCount = 0;

//  Declarar al compilador Arduino/C++ de estas funciones para reducir
//  el tiempo de llamada.
inline void TimerStart() __attribute__((always_inline));
inline void TimerStop() __attribute__((always_inline));

void setup() {
  // inicializar monitor serial
  Serial.begin(115200);  //  alto baud para tiempo critico
  DDRB = 0b00100001;    //  configurar pin 8 y 13 como salida
  TIMSK1 = 0b00000001;  //  habilitar overflow interrupts (pag 144)
}

void loop() {
/*  
 *  La función TestFunction(var) incluye todas las funciones que serán 
 *  sujetos a pruebas de tiempo. Las entradas de la función son:
 *    var = 1 --> suma              PASS  12  100
 *    var = 2 --> multiplicación    PASS  12  100
 *    var = 3 --> raíz cuadrada     PASS  12  100
 *    var = 4 --> seno              PASS  12  100
 *    var = 5 --> coseno            PASS  12  100
 *    var = 6 --> multiplicación de matrices          PASS  1
 *    var = 7 --> integración por método trapezoidal  PASS  1
 *    var = 8 --> multiplicacion matrices con trig    IDK
 */
  overflowCount = 0;
  
  startDebug = micros();
  TimerStart();
  TestFunction(6);
  TimerStop();
  stopDebug = micros() - startDebug;
  SecondCounter();
  
  totalCycles = GetCycles() + overflowCount*65536;
  
//  Serial.print("# de ciclos: ");
//  Serial.print(GetCycles() + overflowCount*65536);
//  Serial.print("\t # de OVF: ");
//  Serial.print(overflowCount);  
//  Serial.print("\t Tiempo con ciclos [ms]: ");
//  Serial.print((float)(totalCycles/16000.0),6);    //  imprimir tiempo promedio con 5 decimales
//  Serial.print("\t Tiempo con micros() [ms]: ");
//  Serial.println((float)(stopDebug*1e-3),3);    //  imprimir tiempo promedio con 5 decimales
//  Serial.println("------------------------------------------------------------------------------------------------------------------");

//  exportar datos a Excel usando Microsoft Data Streamer
  Serial.print(GetCycles() + overflowCount*65536);
  Serial.print(",");
  Serial.print(overflowCount);  
  Serial.print(",");
  Serial.print((float)(totalCycles/16000.0),6);    //  imprimir tiempo promedio con 5 decimales
  Serial.print(",");
  Serial.print((float)(stopDebug*1e-3),3);    //  imprimir tiempo promedio con 5 decimales
  Serial.println();
}

/*
 *  Las siguientes dos funciones toman la tarea de reestablecer e iniciar el contador, y parar
 *  el contador respectivamente. Las funciones usan los puertos y registros dentro del 
 *  microcontrolador ATmega para 
 */

void TimerStart() {
  TCCR1A = 0x00;    //  establecer el modo timer1 como normal
  TCCR1B = 0x00;    //  deshabilitar el reloj del timer1
  TCNT1  = 0x0000;  //  reestablecer el valor del timer1
  TIFR1 = 0x00;     //  reestablecer Timer Overflow Flag
  TCCR1B = 0x01;   //  inicializar timer1 con el reloj de cristal (16 MHz)
  PORTB = 0b00100000;   //  poner el pin 13 en alto
}

void TimerStop() {
  TCCR1B = 0x0000;   //  detener timer1
  PORTB = 0b00000000;   //  poner el pin 13 en bajo
}

//  TCNT1 tiene un valor maximo de 65,536 (16-bit Timer), que da como
//  consecuencia un tiempo máximo de 4.096 ms
uint16_t GetCycles(void) {
  return TCNT1 - 3;      //  obtener los ciclos con correccion
}

//  El segundo contador debe ser establecido a un tiempo predeterminado de tal manera
//  que el LED este apagado por 1 segundo. Esto se puede hacer mediante la funcion
//  delay() con input de 1000 milisegundos.
void SecondCounter(){
  delay(1000);
}

void TestFunction(int testType) {
  
  testVariable = 0;
  
  for(int m = 0; m < contador; m++){
    switch(testType){
    case 1:
      Suma(m);
      break;
    case 2:
      Multi(m);
      break;
    case 3:
      Raiz(m);
      break;
    case 4:
      Seno(m);
      break;
    case 5:
      Coseno(m);
      break;
    case 6:
      MatrixFunc();
      break;
    case 7:
      IntTrap();
      break;
    case 8:
      MatTrig();
      break;
    }
  }
}

void Suma(int a){
  testVariable += a;
}

void Multi(int a){
  testVariable *= a;
}

void Raiz(int a){
  testVariable += sqrt(a);
}

void Seno(int a){
  testVariable += sin(a);
}

void Coseno(int a){
  testVariable += cos(a);
}

void MatrixFunc() {
    int a[3], b[3][3], mul[3], i, j, k, r = 3, c = 3, count = 0;

    for (i = 0; i < r; i++){
        testMatrix[i] = 0;
    }
    
    //  1x3 matrix
    for (i = 0; i < r; i++){
        a[i] = i;
    }

    //  3x3 matrix
    for (i = 0; i < r; i++){
        for (j = 0; j < c; j++){
            count++;
            b[i][j] = count;
        }
    }

    //  multiplicacion
    for (i = 0; i < r; i++){
        mul[i] = 0;
        for (j = 0; j < c; j++){
            mul[i] += a[j] * b[i][j];
        }
    }

    for (i = 0; i < r; i++){
        testMatrix[i] += mul[i];
    }
}

//  algoritmo para integración numerica con el método trapezoidal
void IntTrap() {
  int n = 50, i;
  double a = 0, b = M_PI, h, x, sum = 0, integral;
  //testVariable = 0;

  /*Begin Trapezoidal Method: */
  h = fabs(b - a) / n;

  for (i = 1; i < n; i++){
    x = a + i * h;
    sum = sum + f(x);
  }
  integral = (h / 2) * (f(a) + f(b) + 2 * sum);
  testVariable += integral;
}

//  funcion que será integrada numericamente
double f(double x){
  return sin(x);
}

void MatTrig() {
    int a[3], b[3][3], mul[3], i, j, k, r = 3, c = 3, count = 0;

    for (i = 0; i < r; i++){
        testMatrix[i] = 0;
    }
    
    //  1x3 matrix
    for (i = 0; i < r; i++){
        a[i] = i;
    }

    //  3x3 matrix
    for (i = 0; i < r; i++){
        b[1][i] = sin(20*(i+1));
        b[2][i] = cos(20*(i+1));
        b[3][i] = 20*(i+1);
    }

    //  multiplicacion
    for (i = 0; i < r; i++){
        mul[i] = 0;
        for (j = 0; j < c; j++){
            mul[i] += a[j] * b[i][j];
        }
    }

    for (i = 0; i < r; i++){
        testMatrix[i] += mul[i];
    }
}

//  TOV1 se resetea cuando se ejecuta TIMER1_OVF_vector (pag 146)
ISR(TIMER1_OVF_vect){
  overflowCount++;
}
