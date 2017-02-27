void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
class PID {
  public:
  float control_in;
  float gyro_in;
  float aim = 0;
  float prev_error;
  float sum_error;
  float output;
  uint64_t prev_time;
  void calculate(); 
  };
  void PID::calculate()
  {
    printf("hi");
   }

//[1]http://robot-kingdom.com/pid-controller-tutorial-for-robots/
